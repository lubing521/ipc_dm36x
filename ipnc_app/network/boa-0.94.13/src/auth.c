/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <psp@well.com>
 *  Authorization "module" (c) 1998,99 Martin Hinner <martin@tdp.cz>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#include <stdio.h>
#include "boa.h"
#include <sysctrl.h>
#include <sys_env_type.h>

#ifdef DAVINCI_IPCAM

static union {
	login_data_t userpass;
	get_user_authority_t authority;
} auth;

/*
 * Name: base64decode
 *
 * Description: Decodes BASE-64 encoded string
 */
int base64decode(void *dst,char *src,int maxlen)
{
 int bitval,bits;
 int val;
 int len,x,y;

 len = strlen(src);
 bitval=0;
 bits=0;
 y=0;

 for(x=0;x<len;x++)
  {
   if ((src[x]>='A')&&(src[x]<='Z')) val=src[x]-'A'; else
   if ((src[x]>='a')&&(src[x]<='z')) val=src[x]-'a'+26; else
   if ((src[x]>='0')&&(src[x]<='9')) val=src[x]-'0'+52; else
   if (src[x]=='+') val=62; else
   if (src[x]=='-') val=63; else
    val=-1;
   if (val>=0)
    {
     bitval=bitval<<6;
     bitval+=val;
     bits+=6;
     while (bits>=8)
      {
       if (y<maxlen)
        ((char *)dst)[y++]=(bitval>>(bits-8))&0xFF;
       bits-=8;
       bitval &= (1<<bits)-1;
      }
    }
  }
 if (y<maxlen)
   ((char *)dst)[y++]=0;
 return y;
}

static char base64chars[64] = "abcdefghijklmnopqrstuvwxyz"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";

/*
 * Name: base64encode()
 *
 * Description: Encodes a buffer using BASE64.
 */
void base64encode(unsigned char *from, char *to, int len)
{
  while (len) {
    unsigned long k;
    int c;

    c = (len < 3) ? len : 3;
    k = 0;
    len -= c;
    while (c--)
      k = (k << 8) | *from++;
    *to++ = base64chars[ (k >> 18) & 0x3f ];
    *to++ = base64chars[ (k >> 12) & 0x3f ];
    *to++ = base64chars[ (k >> 6) & 0x3f ];
    *to++ = base64chars[ k & 0x3f ];
  }
  *to++ = 0;
}


int auth_authorize(request * req)
{
	char *pwd;

	if (req->authorization)
	{
		if (strncasecmp(req->authorization,"Basic ",6))
		{
			send_r_bad_request(req);
			return 0;
		}

		base64decode(auth.userpass.id, req->authorization+6, 0x100);

		if ( (pwd = strchr(auth.userpass.id,':')) == 0 )
		{
			send_r_bad_request(req);
			return 0;
		}

		*pwd++=0;
		strcpy(auth.userpass.password, pwd);
		strncpy(auth.userpass.remote_ip, req->remote_ip_addr, IP_STR_LEN);

		if (ControlSystemData(SFIELD_DO_LOGIN, (void *)&auth.userpass, sizeof(login_data_t)) < 0 )
		{
			send_r_unauthorized(req,server_name);
			return 0;
		}
		if (ControlSystemData(SFIELD_GET_USER_AUTHORITY, (void *)&auth.authority, sizeof(get_user_authority_t)) >= 0){
			strcpy(req->user_id, auth.authority.user_id);
			req->authority = auth.authority.authority;
		}
//		dbg("user authority = %d\n", req->authority);
		return 1;
	}
	else
	{
		send_r_unauthorized(req,server_name);
		return 0;
	}

}

#endif
