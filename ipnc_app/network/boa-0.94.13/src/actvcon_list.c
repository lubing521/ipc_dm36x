#include "web_translate.h"
#include <sys_env_type.h>
#include "file_msg_drv.h"
#include <net_config.h>
#include <sysctrl.h>
#include "file_list.h"
#include<stdlib.h>

char html_actlist_header[] = "<HTML><META HTTP-EQUIV=\"CACHE-CONTROL\" CONTENT=\"NO-CACHE\"><TITLE>Event: Connection List </TITLE><style>"
"html {scrollbar-face-color: #182439;scrollbar-shadow-color: #182439;scrollbar-highlight-color: #182439;scrollbar-3dlight-color: #182439;scrollbar-darkshadow-color: #182439;scrollbar-track-color: 182439;scrollbar-arrow-color: #182439;}"
".scrClr{scrollbar-face-color: #1E558C;scrollbar-shadow-color: #182439;scrollbar-highlight-color: #182439;scrollbar-3dlight-color: #182439;scrollbar-darkshadow-color: #182439;scrollbar-track-color: 123456;scrollbar-arrow-color: #182439;}"
"BODY {	PADDING-RIGHT: 0px; MARGIN-TOP: 10px; PADDING-LEFT: 10px; MARGIN-LEFT: auto; WIDTH: auto; MARGIN-RIGHT: auto; background-color:#182439; FONT-FAMILY: Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:18px; color:#FFFFFF}A.linktxt:link {PADDING-RIGHT: 0px; PADDING-LEFT: 0px; FONT-WEIGHT: normal; FONT-SIZE: 18px; PADDING-BOTTOM: 0px; MARGIN: 0px; COLOR: #FFFFFF; PADDING-TOP: 0px; TEXT-DECORATION: underline}.linktxt:visited {PADDING-RIGHT: 0px; PADDING-LEFT: 0px; FONT-WEIGHT: normal; FONT-SIZE: 18px; PADDING-BOTTOM: 0px; MARGIN: 0px; COLOR: #FFFFFF; PADDING-TOP: 0px; TEXT-DECORATION: underline} A.linktxt:active { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; FONT-WEIGHT: normal; FONT-SIZE: 18px; PADDING-BOTTOM: 0px; MARGIN: 0px; COLOR: #FFFFFF; PADDING-TOP: 0px; TEXT-DECORATION: underline} A.linktxt:hover { FONT-WEIGHT: normal; FONT-SIZE: 18px; COLOR: #FFFFFF; TEXT-DECORATION: underline}.heading{ FONT-FAMILY: Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:18pt; color:#FFFFFF } .normaltxt{ 	FONT-FAMILY: Courier New, Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:18px; color:#FFFFFF }.tableheadtxt{ 	FONT-FAMILY: Courier New, Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:18px; color:#000000} </style><BODY><PRE style='font-family:\"Courier New\"; font-size:18px'><PRE style='font-family:\"Courier New\"; font-size:18px'><span class=heading>Event: Connection List</span><br><br>"
"<div class='scrClr' STYLE=\"position:relative;width:620;height:80%;overflow-y:scroll;z-index:10;\"><table border=0 cellspacing=1 cellpadding=1 class=normaltxt width=600><tr align=center bgcolor=#EEEEEE class=tableheadtxt><td>Item No</td><td>Date and Time</td><td>Events</td></tr><script>var z=\"sddel.htm\";function F(b){var o=\"\";var i;for(i=0;i<b;i++){o+=' ';}return o;}"
"function A(a,b,c){var o=\'';o+='<tr valign=middle><td>'+a+'</td><td>';o+=F(15-a.length)+b+' </td><td> '+c+' </td></tr><tr><td colspan=5><hr size=1 color=#3030A0></td></tr>';document.write(o);}";

extern char html_content[1024];

int ActvCon_List_To_html(void *pOutMem, int MaxSize)
{
       char http_ip_addr[100];
	 char	tempbuff[100];
	 char streamType[32], codecCombo[32], resolution[32], dResolution[32];
	 int i;
	GetIP_Addr(http_ip_addr);
	fprintf(stderr,"para_netip %s \n",http_ip_addr);

	strncpy( pOutMem, html_actlist_header, MaxSize );

	if( MaxSize > strlen(pOutMem) )
	       strncat(pOutMem, "</SCRIPT>\n</table>\n</DIV>\n<HR size=1 color=#C0C0C0 width=620 align=left>\n", MaxSize-strlen(pOutMem));
	else
		fprintf(stderr,"MaxSize is not enough!\n");

       sprintf(tempbuff,"<HR size=1 color=#C0C0C0 width=620 align=left>Currently Not Available <BR></PRE></BODY></HTML>");

	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, tempbuff, MaxSize-strlen(pOutMem));
	else
		fprintf(stderr,"MaxSize is not enough!\n");
}
void cleanActiveListLog()
{
	return 1;
}
