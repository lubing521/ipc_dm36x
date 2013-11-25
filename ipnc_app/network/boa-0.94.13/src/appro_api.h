// command.h
//
// Last changed:   November 26, 2007
//

#ifndef __APPRO_API_H__
#define __APPRO_API_H__

#define TRUE		1
#define FALSE	0

#define HTTP_OPTION_CHANGED		0x0001
  
#define OPTION_OK		"OK "
#define OPTION_NG		"NG "
#define OPTION_NS		"NS "
#define OPTION_UW		"UW "
#define OPTION_UA		"UA "

#define MIN_HTTP_PORT		1
#define MAX_HTTP_PORT		65535

int hash_table_init(void);
void http_run_command(request *req, COMMAND_ARGUMENT *arg, int num);
int html_ini_cmd(AUTHORITY authority, char *addr, int max_size);
int html_keyword_cmd(AUTHORITY authority, char *addr, int max_size);
int html_cmdlist_cmd(AUTHORITY authority, char *addr, int max_size); 
int http_sdget_cmd(request *req, COMMAND_ARGUMENT *argm, char *addr, int max_size, char *sdpath);
void http_sddel_cmd(request *req, COMMAND_ARGUMENT *argm);
int http_Paraget_cmd(char *addr, int max_size);
#endif /* __APPRO_API_H__ */

