#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(){
	int fd1,n;
	FILE *fd;
	char *buf;
/*
	buf = (char *)malloc(163022);
	fd = fopen("./test.jpg", "rb");
	if(fd == NULL) printf("open file error!\n");
	n = fread(&buf ,sizeof( char ), 163022 , fd);
	if(n == -1) printf("read error!\n");
	printf("read %u bytes\n",n);
	fclose(fd);*/

	buf = (char *)malloc(163022);
	fd1 = open("./test.jpg", O_RDONLY);
	n = read(fd1, buf, 163022);
	if(n == -1) printf("read error!\n");
	printf("read %u bytes\n",n);
	close(fd1);
	return 0;
}
