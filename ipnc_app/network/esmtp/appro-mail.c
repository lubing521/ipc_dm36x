#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
//#include <iconv.h>

#include <ApproDrvMsg.h>
#include <Appro_interface.h>
#include <Msg_Def.h>
#include <sys_msg_drv.h>
#include <sysctrl.h>

#define attachAmount 20
#define BUFFER_SIZE 3072
#define BASE64_BUFFER (BUFFER_SIZE/24 + BUFFER_SIZE*4/3)

void base64Encode_file(int fd , FILE *pipe_fd , int fileSize);
void base64Encode(char *in , char *out,int len ,int *dlen);

const char *Base64Table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//static int qid;

int RemoveFile(char *pFileName)
{
	int ret;
	char cmd[64];
	sprintf(cmd,"rm -f %s\n", pFileName);
	ret = system(cmd);
	return ret;
}

int CreateTestFile(char *strTestFileName)
{
	int ret = 0;
	char strPath[40];
	FILE* fp;
	if(strTestFileName != NULL){
		strcpy(strTestFileName,"test.cfg");
		sprintf(strPath, "/tmp/%s", strTestFileName);
		if((fp = fopen(strPath, "wt")) != NULL){
			fprintf(fp, "1\n123.txt\n");
			fclose(fp);
			if((fp = fopen("/tmp/123.txt", "wt")) != NULL){
				fprintf(fp, "Test File\n");
				fclose(fp);
			}else{
				ret = -1;
				RemoveFile(strPath);
			}
		}else
			ret = -1;
	}else
		ret = -1;
	return ret;
}

int main(int argc, char *argv[]){
	FILE *out,*script;
	int in;
	char *wday[]={"=E6=98=9F=E6=9C=9F=E6=97=A5","=E6=98=9F=E6=9C=9F=E4=B8=80","=E6=98=9F=E6=9C=9F=E4=BA=8C","=E6=98=9F=E6=9C=9F=E4=B8=89","=E6=98=9F=E6=9C=9F=E5=9B=9B","=E6=98=9F=E6=9C=9F=E4=BA=94","=E6=98=9F=E6=9C=9F=E5=85=AD"};
	time_t timep;
	struct timeval tv;
	struct tm *p;

	char *buffer;
	char *buffer_jpeg;
	unsigned short serverPort = 25;
	char subject[1024] = "";
	char text[2048] = "";
	char cc[1024] = "";
	char from[128] = "";
	char receivers[1024] = "";
	char username[64] = "";
	char password[64] = "";
	char serverIP[256] = "";
	char ufileName[256];
	char certificate_passphrase[32] = "appropho";
	char html_style = 0;
	char auth = 0;
	int dlen = 0,file_size = 0,ufile_size = 0;
	int x ,y,ret;
	char attachments = 5;
	char *data1[attachAmount],*data;
	char filename[attachAmount][256];
	AV_DATA av_data;
	int last_sreial = -1;
	char **filenamearray = NULL;
	int filenum = 0;
	/* change home for esmtp check SSL */
	setenv("HOME", "/root/", 1);

	if(SysDrvInit(SYS_SMTP_MSG) < 0){
		printf("SysDrvInit Fail\n");
		return 0;
	}
	if( ControlSystemData(SFIELD_GET_SMTP_SERVER_IP, (void *)&serverIP, sizeof(serverIP)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_PORT, (void *)&serverPort, sizeof(serverPort)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_AUTHENTICATION, (void *)&auth, sizeof(auth)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_USERNAME, (void *)&username, sizeof(username)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_PASSWORD, (void *)&password, sizeof(password)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_SENDER_EMAIL_ADDRESS, (void *)&from, sizeof(from)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_RECEIVER_EMAIL_ADDRESS, (void *)&receivers, sizeof(receivers)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_CC, (void *)&cc, sizeof(cc)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_SUBJECT, (void *)&subject, sizeof(subject)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_TEXT, (void *)&text, sizeof(text)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_ATTACHMENTS, (void *)&attachments, sizeof(attachments)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( ControlSystemData(SFIELD_GET_SMTP_VIEW, (void *)&html_style, sizeof(html_style)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	if( attachments < 1 || attachments > attachAmount ){
		printf("attachAmout error!\n");
		SysDrvExit();
		exit(1);
	}
	if((buffer = malloc(512)) == NULL){
		printf("Memory allocate error!\n");
		SysDrvExit();
	  	exit(-1);
	}
	memset(buffer,0,512);
	/*           write config file           */
	if(argc > 1 && strcmp("-t", argv[1]) == 0 && (script = fopen("/root/.esmtprc","r")) != NULL ){
		fclose( script );
	} else if( (script = fopen("/root/.esmtprc","w")) != NULL ){
		sprintf(buffer,"hostname = %s:%d\n",serverIP,serverPort);
		fwrite(buffer,sizeof(char),strlen(buffer),script);
		sprintf(buffer,"username = %c%s%c\n",'"',username,'"');
		fwrite(buffer,sizeof(char),strlen(buffer),script);
		sprintf(buffer,"password = %c%s%c\n",'"',password,'"');
		fwrite(buffer,sizeof(char),strlen(buffer),script);
//		sprintf(buffer,"starttls disabled\n");
		if(auth)
			sprintf(buffer,"starttls required\n");
		else
			sprintf(buffer,"starttls enabled\n");
		fwrite(buffer,sizeof(char),strlen(buffer),script);
		sprintf(buffer,"certificate_passphrase = \"%s\"\n",certificate_passphrase);
		fwrite(buffer,sizeof(char),strlen(buffer),script);

		fclose( script );
		system("chmod 0710 /tmp/.esmtprc\n");
	}else	{
		printf("/root/.esmtprc file open error!\n");
		SysDrvExit();
		exit(1);
	}
	//system("cp /root/.esmtprc /\n");
#if 0
//	CMEM_init();
	qid = Msg_Init(MSG_KEY);

	if(qid < 0){
		printf("Messsage Init Fail\n");
//		CMEM_exit();
		return 0;
	}
	/* Save Qid in ApproDrvMsg */
	if(ApproDrvInit(SMTP_MSG_TYPE) < 0){
		printf("ApproDrvInit Fail\n");
		exit(1);
	}
	if(func_get_mem(&qid) != 0){
		CMEM_exit();
		exit(1);
	}
#endif
	for( x = 0 ; x < strlen(receivers) ; x++){
		if( receivers[x] == ',' || receivers[x] == ';' )
			receivers[x] = ' ';
	}
	if(argv[1] && strcmp("-t", argv[1]) == 0)
		sprintf(buffer,"./esmtp %s -f %s -C /tmp/.esmtprc -X /tmp/log.txt",receivers,from);
	else
		sprintf(buffer,"./esmtp %s -f %s -C /tmp/.esmtprc",receivers,from);
	/*  write to msmtp   */
printf("%s\n",buffer);
	out = popen(buffer,"w");

	if( argv[1] ){
		FILE *cfgfile;
		if(strcmp("-t", argv[1]) == 0){
			if(CreateTestFile(ufileName) != 0){
				printf("Can't create test files!!!\n");
				SysDrvExit();
				exit(-1);
			}
		}else
			strcpy(ufileName,argv[1]);
		sprintf(buffer,"/tmp/");
		strcat(buffer,ufileName);
		if( (cfgfile = fopen(buffer, "rb")) == NULL ){
			printf("File %s open error!\n",buffer);
			exit(1);
		}
		printf("opening config file %s\n",buffer);
		filenum = atoi(fgets(buffer, 16, cfgfile));
		printf("\n===CFG: total %d files===\n", filenum);
		filenamearray = calloc(filenum, sizeof(void *));
		if(filenamearray == NULL){
			printf("Memory allocate error!\n");
			SysDrvExit();
		  	exit(-1);
		}
		/*filebodyarray = calloc(filenum, sizeof(void *));
		if(filebodyarray == NULL){
			printf("Memory allocate error!\n");
		  	exit(-1);
		}*/
		for (x = 0 ; x < filenum ; x++)
		{
			fgets(ufileName, 256, cfgfile);
			if (ufileName[strlen(ufileName) - 1] == '\n')
			{
				ufileName[strlen(ufileName) - 1] = '\0';
			}
			printf("%2d. /tmp/%s\n", x+1, ufileName);
			filenamearray[x] = malloc(strlen(ufileName) + 1);
			if(filenamearray[x] == NULL)
			{
				printf("Memory allocate error!\n");
				SysDrvExit();
			  	exit(-1);
			}
			strcpy(filenamearray[x], ufileName);
		}
		fclose(cfgfile);
#if 0
		strcpy(ufileName,argv[1]);
		sprintf(buffer,"/tmp/");
		strcat(buffer,ufileName);
		RemoveFile(buffer);
#endif
	} else {
		for( x = 0 ; x < attachments ; x++ ){
			//GetAVData(AV_OP_WAIT_NEW_MJPEG_SERIAL, -1, &av_data );
			GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &av_data );
			while(last_sreial == av_data.serial){
				usleep(5000);
				GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &av_data );
			}
			last_sreial = av_data.serial;
			ret = GetAVData(AV_OP_LOCK_MJPEG, av_data.serial, &av_data );
			printf("image: %3d\n",x+1);
			printf("av_data.serial = %d\n",av_data.serial);
			if(ret != RET_SUCCESS){
				printf("Error : %d\n", ret);
				SysDrvExit();
				return ret;
			}
			time( &timep );
			gettimeofday(&tv,NULL);
			p = localtime( &timep );

			sprintf(filename[x],"%d_%02d_%02d_=?UTF-8?Q?%s?=_%02d_%02d_%02d_%02ld.jpg",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday,wday[p->tm_wday],p->tm_hour,p->tm_min,p->tm_sec,tv.tv_usec/10000);
			//printf("debug point: GetAVData(AV_OP_LOCK_MJPEG)\n");
			file_size = av_data.size;
			//080609 unused malloc
			//buffer_jpeg = malloc(file_size);
			//memset(buffer_jpeg,0,file_size);
			int temp_size = file_size*4/3 + file_size/54 + 4;
			printf("av_data.size : %d\n",av_data.size);
			data = malloc(temp_size);
			if(data == NULL){
				printf("Memory allocate error!\n");
				SysDrvExit();
			  	exit(-1);
			}
			memset(data,0,temp_size);
			buffer_jpeg = (char *)av_data.ptr;
			base64Encode( buffer_jpeg , data, file_size , &dlen);
			data1[x] = malloc(dlen);
			if(data1[x] == NULL){
				printf("Memory allocate error!\n");
				SysDrvExit();
			  	exit(-1);
			}
			memset(data1[x],0,dlen);
			for( y = 0 ; y < dlen ; y++ ){
				data1[x][y] = data[y];
			}
			free(data);
			GetAVData(AV_OP_UNLOCK_MJPEG, av_data.serial, &av_data);
			//usleep(120000);  //delay us
	//printf("debug point: GetAVData(AV_OP_UNLOCK_MJPEG)\n");
		}
	}
	memset(buffer,0,512);
	for( x = 0 ; x < strlen(receivers) ; x++){
		if( receivers[x] == ' ' )
			receivers[x] = ',';
	}
	/*  starting write header   */
	sprintf(buffer,"To: %s\r\n",receivers);
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"Cc: %s\r\n",cc);
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"From: %s\r\n",from);
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"MIME-Version: 1.0\r\n");
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"Content-Transfer-Encoding: 7bit\r\n");
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"Subject: %s\r\n",subject);
//	sprintf(buffer,"Subject:=?UTF-8?Q?=E2=89=A0?=\r\n");
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"Content-Type: multipart/mixed; boundary=%c_----------=_10167391557129230%c\r\n",'"','"');
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"X-Mailer: MIME::Lite 2.106  (B2.11; Q2.03)\r\n\r\n");
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"--_----------=_10167391557129230\r\n");
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	sprintf(buffer,"Content-Transfer-Encoding: binary\r\n");
	fwrite(buffer,sizeof(char),strlen(buffer),out);

	if( html_style && (!argv[1]) ){
		sprintf(buffer,"Content-Type: text/html; charset=%cUTF-8%c\r\n",'"','"');
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		sprintf(buffer,"Content-Transfer-Encoding: quoted-printable\r\n\r\n");
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		sprintf(buffer,"%s",text);
//		sprintf(buffer,"<font size=10>%s=AA`=B7N=A1G=C2=E0=B1H=ABH=A5=F3=A4w=A7=A8=B1a</font><br>",text);
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		for( x = 1 ; x <= attachments ; x++ ){
			sprintf(buffer,"<img src=%ccid:%03d%c>\r\n",'"', x ,'"');
			fwrite(buffer,sizeof(char),strlen(buffer),out);
		}
	}else {
		sprintf(buffer,"Content-Type: text/plain; charset=%cUTF-8%c\r\n",'"','"');
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		sprintf(buffer,"Content-Transfer-Encoding: quoted-printable\r\n\r\n");
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		sprintf(buffer,"%s",text);
//		sprintf(buffer,"=AA`=B7N=A1G=C2=E0=B1H=ABH=A5=F3=A4w=A7=A8=B1a\r\n");
//		sprintf(buffer,"This is a alarm mail.\r\n");
		fwrite(buffer,sizeof(char),strlen(buffer),out);
	}
	/*  starting write data   */
	if( argv[1] ){
		for( x = 1 ; x <= filenum ; x++ ){
			sprintf(buffer,"\r\n\r\n--_----------=_10167391557129230\r\n");
			fwrite(buffer,sizeof(char),strlen(buffer),out);
			sprintf(buffer,"Content-Transfer-Encoding: base64\r\n");
			fwrite(buffer,sizeof(char),strlen(buffer),out);
			sprintf(buffer,"Content-id: <%03d>\r\n",x);
			fwrite(buffer,sizeof(char),strlen(buffer),out);
			sprintf(buffer,"Content-Type: application/x-msdownload; name=%c%s%c\r\n\r\n",'"',filenamearray[x-1],'"');
			fwrite(buffer,sizeof(char),strlen(buffer),out);
			//fwrite(filebodyarray[x-1],sizeof(char),strlen(filebodyarray[x-1]),out);
			//free(filebodyarray[x-1]);
			sprintf(buffer,"/tmp/");
			strcat(buffer,filenamearray[x-1]);
			if( (in = open(buffer,O_RDONLY)) == -1 ){
				printf("File %s open error!\n",buffer);
				SysDrvExit();
				exit(1);
			}
			if( (ufile_size = lseek(in, 0, SEEK_END)) == -1 ) {
				printf("fseek SEEK_END error");
				SysDrvExit();
				return -1;
			}
			ufile_size += 20;
			if( lseek(in, 0, SEEK_SET) == -1 ) {
				printf("fseek SEEK_SET error");
				SysDrvExit();
				return -1;
			}
			base64Encode_file(in, out, ufile_size);
			free(filenamearray[x-1]);
			close(in);
//			RemoveFile(buffer);
		}
		free(filenamearray);
	} else {
		for( x = 1 ; x <= attachments ; x++ ){
		sprintf(buffer,"\r\n\r\n--_----------=_10167391557129230\r\n");
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		sprintf(buffer,"Content-Transfer-Encoding: base64\r\n");
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		sprintf(buffer,"Content-id: <%03d>\r\n",x);
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		sprintf(buffer,"Content-Type: image/jpeg; name=%c%s%c\r\n\r\n",'"',filename[x-1],'"');

//sprintf(buffer,"Content-Type: image/jpeg; name=%c=?big5?Q?=ACP=B4=C1?=%d.jpg%c\r\n\r\n",'"',x,'"');
		fwrite(buffer,sizeof(char),strlen(buffer),out);
		fwrite(data1[x-1],sizeof(char),strlen(data1[x-1]),out);
		free(data1[x-1]);
		}
	}
	/*   ending   */
	sprintf(buffer,"\r\n\r\n--_----------=_10167391557129230--\r\n\r\n.\r\n");
	fwrite(buffer,sizeof(char),strlen(buffer),out);
	//close( fd_jpeg );
//	CMEM_exit();
	pclose( out );
	free(buffer);
	SysDrvExit();
	return 0;
}

void base64Encode_file(int fd , FILE *pipe_fd , int fileSize){
	char temp;
	char *in;  //for read()
	char *out;
	unsigned int source_index = 0,target_index = 0;
	int bits_8_group;
  unsigned int x = 0 , y = 0;
  //unsigned int buf_byte_num = BUFFER_SIZE , tmp;

  in = (char *)malloc(BUFFER_SIZE);
  out = (char *)malloc(BASE64_BUFFER);
  bits_8_group = fileSize/3;

  read(fd, in, BUFFER_SIZE);

	while( x < bits_8_group ){
		temp = in[source_index] >> 2;
		out[target_index++] = Base64Table[temp & 0x3F];
		temp = in[source_index] << 4;
		temp = temp & 0x30;
		temp = (in[source_index + 1] >> 4) | temp;
		out[target_index++] = Base64Table[temp & 0x3F];
		temp = in[source_index + 1] & 0x0f;
		temp <<= 2;
		temp = temp & 0x3c;
		temp = (in[source_index + 2] >> 6) | temp;
		out[target_index++] = Base64Table[temp & 0x3F];
		temp = in[source_index + 2];
		out[target_index++] = Base64Table[temp & 0x3F];

		source_index += 3;
		x++;
		y++;
		if( y == 18 ){
			y = 0;
			out[target_index++] = '\n';
		}
		if(source_index == BUFFER_SIZE){
			fwrite(out,sizeof(char),target_index,pipe_fd);
			//tmp = read(fd, in, BUFFER_SIZE);
			read(fd, in, BUFFER_SIZE);
			source_index = 0;
			target_index = 0;
		}
	}
	fwrite(out,sizeof(char),target_index,pipe_fd);
	source_index = 0;
	target_index = 0;

	switch( fileSize%3 ){
		case 1:
			temp = in[source_index] >> 2;
			out[target_index++] = Base64Table[temp & 0x3F];
			temp = in[source_index] << 4;
			temp = temp & 0x30;
			temp = (in[source_index + 1] >> 4) | temp;
			out[target_index++] = Base64Table[temp & 0x3F];
			out[target_index++] = '=';
			out[target_index++] = '=';
			fwrite(out,sizeof(char), 3 ,pipe_fd);
			break;
		case 2:
			temp = in[source_index] >> 2;
			out[target_index++] = Base64Table[temp & 0x3F];
			temp = in[source_index] << 4;
			temp = temp & 0x30;
			temp = (in[source_index + 1] >> 4) | temp;
			out[target_index++] = Base64Table[temp & 0x3F];
			temp = in[source_index + 1] & 0x0f;
			temp <<= 2;
			temp = temp & 0x3c;
			out[target_index++] = Base64Table[temp & 0x3F];
			out[target_index++] = '=';
			fwrite(out,sizeof(char), 3 ,pipe_fd);
			break;
	}
}

void base64Encode(char *in , char *out ,int len ,int *dlen){
        char temp;
        int source_index = 0,target_index = 0;
        int bits_8_group;
        int x = 0 , y = 0;

        bits_8_group = len/3;

        while( x < bits_8_group ){
                temp = in[source_index] >> 2;
                out[target_index++] = Base64Table[temp & 0x3F];
                temp = in[source_index] << 4;
                temp = temp & 0x30;
                temp = (in[source_index + 1] >> 4) | temp;
                out[target_index++] = Base64Table[temp & 0x3F];
                temp = in[source_index + 1] & 0x0f;
                temp <<= 2;
                temp = temp & 0x3c;
                temp = (in[source_index + 2] >> 6) | temp;
                out[target_index++] = Base64Table[temp & 0x3F];
                temp = in[source_index + 2];
                out[target_index++] = Base64Table[temp & 0x3F];

                source_index += 3;
                x++;
                y++;
                if( y == 18 ){
                        y = 0;
                        out[target_index++] = '\n';
                }
        }
        switch( len % 3 ){
        case 1:
                temp = in[source_index] >> 2;
                out[target_index++] = Base64Table[temp & 0x3F];
                temp = in[source_index] << 4;
                temp = temp & 0x30;
                temp = (in[source_index + 1] >> 4) | temp;
                out[target_index++] = Base64Table[temp & 0x3F];
                out[target_index++] = '=';
                out[target_index++] = '=';
                break;
        case 2:
                temp = in[source_index] >> 2;
                out[target_index++] = Base64Table[temp & 0x3F];
                temp = in[source_index] << 4;
                temp = temp & 0x30;
                temp = (in[source_index + 1] >> 4) | temp;
                out[target_index++] = Base64Table[temp & 0x3F];
                temp = in[source_index + 1] & 0x0f;
                temp <<= 2;
                temp = temp & 0x3c;
                out[target_index++] = Base64Table[temp & 0x3F];
                out[target_index++] = '=';
                break;
        }
        *dlen = target_index;
}
