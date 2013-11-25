// web_translate.h
//
// Last changed:   January 7, 2008
//

#ifndef __WEB_TRANSLATE_H__
#define __WEB_TRANSLATE_H__

#include <asm/types.h>
#include <netinet/in.h>
#include "boa.h"
#include <ipnc_ver.h>

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
typedef union __NET_IPV4 {
	__u32	int32;
	__u8	str[4];
} NET_IPV4;

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

#define MIN_HTTP_PORT		1
#define MAX_HTTP_PORT		65535


//************************************************************
#define MIN_LANCAM_TITLE_LEN    	3




/***************************************************************************
 *                                                                         *
 ***************************************************************************/
typedef struct __HTML_ARGUMENT
{
  char *name;
  int (*handler) (char *, char *);
  AUTHORITY authority;

  struct __HTML_ARGUMENT *next;

} HTML_ARGUMENT;

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
#define MAX_ARG_HASH_SIZE		64

typedef struct __ARG_HASH_TABLE {
	HTML_ARGUMENT *harg;

} ARG_HASH_TABLE;

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int arg_hash_table_init(void);
int TranslateWebPara(AUTHORITY authority, char *target, char *para, char *subpara);
void arg_hash_table_cleanup();

#endif   /* __WEB_TRANSLATE_H__ */

