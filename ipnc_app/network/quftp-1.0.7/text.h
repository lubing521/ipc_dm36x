/* Text Functions Definitions
   $Revision: 1.2 $
   $Date: 2002/06/13 07:00:33 $
*/

#ifndef __TEXT_H__
#define __TEXT_H__

#define GNW_START		0
#define GNW_NEXT		1
#define GNW_REST		-1

char *getnextword(char *text, int state);
char *ftp_strip_illegal(char *text);
int parseurl(char *url, struct ftpconnection *connection);
char *getpassword(char *username);
char *expand_home(char *s);
char *strupper(char *string);
char *strlower(char *string);
char *getpath(const char *path);
char *getfilename(char *path);

#endif

