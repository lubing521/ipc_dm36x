/* RC File Functions
   $Revision: 1.1.1.1 $
   $Date: 2000/04/01 00:56:17 $
*/

#ifndef __RC_H__
#define __RC_H__

struct variable_list {
	char *key;
	char *value;
	struct variable_list *next;
};

struct alias_list {
	char *alias;
	char *expanded;
	struct alias_list *next;
};

char *config(struct variable_list *list, char *key);
int setconfig(struct variable_list **list, char *key, char *data);
char *alias(struct alias_list *list, char *alias);
int setalias(struct alias_list **list, char *alias, char *expanded);

#endif
