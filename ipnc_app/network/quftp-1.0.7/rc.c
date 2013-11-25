/*
 * RC File Functions
 *
 * $Revision: 1.2 $
 * $Date: 2002/06/13 07:00:33 $
 */

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include "ftp.h"
#include "quftp.h"
#include "text.h"
#include "rc.h"

//#define free(address) printf("free  0x%x at %d in %s\n", address, __LINE__, __FILE__);free(address)

char *config(struct variable_list *list, char *key) {
	struct variable_list *item;
	static char *temp = (char *)NULL;
	if(temp){
		free(temp);
		temp = (char *)NULL;
	}
	if (!key) return NULL;
	item = list;
	while (item) {
		if (strcasecmp(item->key, key) == 0){
			temp = strdup(item->value);
//			printf("Use strdup 0x%x at %d in file %s\n", temp, __LINE__, __FILE__);
			return temp;
		}
		item = item->next;
	}
	return NULL;
}

int setconfig(struct variable_list **list, char *key, char *data) {
	struct variable_list *item, *lastitem;
	if (!key) return 0;
	item = *list;
	lastitem = NULL;
	while (item) {
		if (strcmp(item->key, key) == 0) {
			if (item->value) {free(item->value);}
			if (!data) {					/* Remove key */
				if (lastitem) lastitem->next = item->next;
				else *list = item->next;
				free(item->key);
				free(item);
				return 1;
			}
			item->value = strdup(data);
//			printf("Use strdup 0x%x at %d in file %s\n", item->value, __LINE__, __FILE__);
			return 1;
		}
		lastitem = item;
		item = item->next;
	}
	if (!data) return 0;
	item = (struct variable_list *)malloc(sizeof(struct variable_list));
//	printf("malloc  0x%x at %d in %s\n", item, __LINE__, __FILE__);
	memset(item, 0, sizeof(*item));
	item->key = strdup(key);
//	printf("Use strdup 0x%x at %d in file %s\n", item->key, __LINE__, __FILE__);
	item->value = strdup(data);
//	printf("Use strdup 0x%x at %d in file %s\n", item->value, __LINE__, __FILE__);
	item->next = NULL;
	if (lastitem) lastitem->next = item;
	else *list = item;
	return 1;
}

char *alias(struct alias_list *list, char *alias) {
	struct alias_list *item;
	char *temp;
	item = list;
	while (item) {
		if (strcasecmp(item->alias, alias) == 0){
			temp = strdup(item->expanded);
//			printf("Use strdup 0x%x at %d in file %s\n", temp, __LINE__, __FILE__);
			return temp;
		}
		item = item->next;
	}
	return NULL;
}

int setalias(struct alias_list **list, char *alias, char *expanded) {
	struct alias_list *item, *lastitem;
	item = *list;
	lastitem = NULL;
	while (item) {
		if (strcasecmp(item->alias, alias) == 0) {
			if (item->expanded) {free(item->expanded);}
			item->expanded = strdup(expanded);
//			printf("Use strdup 0x%x at %d in file %s\n", item->expanded, __LINE__, __FILE__);
			return 1;
		}
		lastitem = item;
		item = item->next;
	}
	item = (struct alias_list *)malloc(sizeof(struct alias_list));
//	printf("malloc  0x%x at %d in %s\n", item, __LINE__, __FILE__);
	memset(item, 0, sizeof(*item));
	item->alias = strdup(alias);
//	printf("Use strdup 0x%x at %d in file %s\n", item->alias, __LINE__, __FILE__);
	item->expanded = strdup(expanded);
//	printf("Use strdup 0x%x at %d in file %s\n", item->expanded, __LINE__, __FILE__);
	item->next = NULL;
	if (lastitem) lastitem->next = item;
	else *list = item;
	return 1;
}

