/*
 * Basic xml parser for validating and processing inbound data
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "psia.h"

/* Element state */
#define XST_GET_START_TAG	1
#define XST_GET_VALUE		2
#define XST_GET_END_TAG		3
#define XST_VALIDATE		4
#define XST_DONE		5

/* Start tag state */
#define ST_GET_TAG		0
#define ST_GET_ATTR_NAME	1
#define ST_GET_QUOTE		2
#define ST_GET_ATTR_VALUE	3
#define ST_GET_SPACE		4
#define ST_GET_SLASH		5
#define ST_DONE			6

static int TagError;

static int validate_start_tag(char *str, int len)
{
	//fprintf(stderr,"\r\nInside validate_start with str[0]:%c & str[len-1]:%c\n",str[0],str[len-1]);
	if((str[0] != '<') || (str[len-1] != '>'))
		return 1;
	return 0;
}

static int validate_attributes(char *str, int len)
{
	return 0;
}

static int validate_value(char *str, int len)
{
	return 0;
}

static int validate_end_tag(char *str, int len)
{
	if(len) {  // could be empty
		if((str[0] != '<') || (str[len-1] != '>'))
			return 1;
		if(str[1] != '/')
			return 1;
	}
	return 0;
}

static int validate_tag(tag_t *tag)
{
	int start_len, end_len;
	char *str;
	char start_name[128];
	char end_name[128];
	//int i,j;
	//char *ch,*ch1;

	//fprintf(stderr,"\r\nInside validate_tag with tag->start_len:%d & tag->start_name\n",tag->start_len);
	//ch = tag->start_name;
	//for (i=0;i<tag->start_len;i++ )
	//{
	// fprintf(stderr,"%c",*ch++);
	//}

	start_len = tag->start_len-2; // ignore open and closing braces

	if(tag->attr_len) // ignore space + attributes
		start_len = start_len - (tag->attr_len+1);
	strncpy(start_name, tag->start_name + 1, start_len);
	start_name[start_len] = 0x00;
	/* start tag must be valid here */
	if(validate_start_tag(tag->start_name, tag->start_len))
		return -1;

	//fprintf(stderr,"\r\nInside validate_tag with tag->end_len:%d & tag->end_name\n",tag->end_len);
	//ch1 = tag->end_name;
	//for (j=0;j<tag->end_len;j++ )
	//{
	//  fprintf(stderr,"%c",*ch1++);
	//}

	if(!tag->empty) {
		end_len = tag->end_len-3; // ignore braces and /
		strncpy(end_name, tag->end_name+2, end_len);
		end_name[end_len] = 0x00;

		if(tag->end_name[1] != '/') {
			if(tag->value_len == 0)
				return 1;	/* no match, treat as a child */
			return -6;		/* value with no end tag is incorrect */
		}
		if(strcmp(start_name, end_name))
			return -5;   /* start an end tag don't match */
	}

	if(validate_attributes(tag->attr_name, tag->attr_len))
		return -2;

	if(validate_value(tag->value_name, tag->value_len))
		return -3;

	if(validate_end_tag(tag->end_name, tag->end_len))
		return -4;

	if(start_len) {
		str = malloc(start_len+1);
		if(!str) return -5;
		memcpy(str, start_name, start_len);
		str[start_len] = 0x00;
		tag->name = str;
	}

	if(tag->value_len) {
		str = malloc(tag->value_len+1);
		if(!str) return -6;
		memcpy(str, tag->value_name, tag->value_len);
		str[tag->value_len] = 0x00;
		tag->value = str;
	}

	if(tag->attr_len) {
		str = malloc(tag->attr_len+1);
		if(!str) return -7;
		memcpy(str, tag->attr_name, tag->attr_len);
		str[tag->attr_len] = 0x00;
		tag->attr = str;
	}

	return 0;  /* match */
}

static tag_t *create_tag(request *req, tag_t *parent)
{
	tag_t *tag, *empty;

	tag = malloc(sizeof(tag_t));
	if(!tag) {
		TagError = 1;
		return NULL;
	}
	memset(tag, 0, sizeof(tag_t));
	tag->marker = TAG_MARKER;
	tag->count = req->tag_count;
	tag->parent = parent;

	/* add tag to parent's child list */
	if(parent) {
		if(!parent->first_child) {
			parent->first_child = tag;
		}
		else {
			empty = parent->first_child;
			while(empty->next)
				empty = empty->next;
			empty->next = tag;
		}
	}
	req->tag_count++;
	return tag;
}

static int nest = 0;

static int set_xstate(int state)
{
	return state;
}

/* basic xml parser */
static int xml_parse(request *req, tag_t *parent, char *xstr, int len)
{
	int i;
	int next_ch = -1;
	int last_ch = -1;
	int this_ch = -1;
	int rval;
	int state;
	tag_t *tag = NULL;

	state = set_xstate(XST_DONE);
	nest++;
	i = 0;
	while(i < len) {
		last_ch = xstr[i-1];
		this_ch = xstr[i];
		next_ch = xstr[i+1];
		if(state == XST_DONE) {

			/* done processing at this level, return processed count to previous level */
			if(this_ch == '<' && next_ch == '/')
				return i;

			state = set_xstate(XST_GET_START_TAG);
			tag = create_tag(req, parent);
		}
		switch (state) {
			case XST_GET_START_TAG:
				if(!tag->start_name) {
					tag->start_name = &xstr[i];
				}
				tag->start_len++;
				/* process any tag attributes */
				if(!tag->attr_name) {
					if((this_ch == ' ') && (next_ch != '/'))
						tag->attr_name = &xstr[i+1];
				}
				else {
					if(this_ch != '>')
						tag->attr_len++;
				}

				if (this_ch == '>') {
					if(last_ch == '?') {
						if(!req->prolog_tag) {
							req->prolog_tag = tag;
							state = set_xstate(XST_DONE);
							goto NextChar;
						}
						else
							return -1;
					}
					else if(!req->root_tag) {
						req->root_tag = tag;
					}

					if(last_ch == '/')	{ // empty tag
						state = set_xstate(XST_VALIDATE);
						tag->empty = 1;
					}
					else
						state = set_xstate(XST_GET_VALUE);
				}
			break;

			case XST_GET_VALUE:
				if (this_ch != '<') {
					if(!tag->value_name)
						tag->value_name = &xstr[i];
					tag->value_len++;
					break;
				}
				else
					state = set_xstate(XST_GET_END_TAG);
				/* fall through is deliberate */

			case XST_GET_END_TAG:
				if(!tag->end_name)
					tag->end_name = &xstr[i];
				tag->end_len++;

				if (this_ch == '>')
					state = set_xstate(XST_VALIDATE);
			break;
		}
		if(state == XST_VALIDATE) {
			state = set_xstate(XST_DONE);
			rval = validate_tag(tag);
			//fprintf(stderr,"\r\After validate_tag with retval:%d\n",rval);

			if (rval == 0) {
				tag = NULL;
				goto NextChar;
			}
			/* tag is invalid */
			else if (rval < 0) {
				return -1;
			}

			// tag mismatch, adjust len and process child
			i = i - tag->end_len;
			rval = xml_parse(req, tag, tag->end_name, len-i);
			if(rval < 0)
				return rval;
			tag->end_name = 0;  // clear end tag
			tag->end_len = 0;
			nest--;
			state = set_xstate(XST_GET_END_TAG);
			i = i+rval;	// advance processed offset
		}
NextChar:
		i++;
	};
	//fprintf(stderr,"\r\nAfter xml_parse\n");
	return i;
}

int xml_validate(request *req, char *xstr, int len)
{
	int rval;
	tag_t *parent = NULL;
	//int i;
	//char *ch;

	if(xstr == NULL)
		return -1;

	//ch = xstr;
	//fprintf(stderr,"\r\nInside xml_validate with len:%d & input string: \n",len);
	//for (i=0; i<len ;i++)
	//{
	//	fprintf(stderr,"%c",*ch++);
	//}

	rval = xml_parse(req, parent, xstr, len);
	return rval;
}

unsigned long xml_required_field(int bit)
{
	if((bit < 1) || (bit > 32))
		return 0;
	return(0x01 << (bit-1));
}

unsigned long xml_required_mask(int count)
{
	unsigned long mask = 0;
	int f;

	for(f = 1; f <= count; f++)
		mask |= xml_required_field(f);
	return mask;
}

static int DeleteCount = 0;
static void delete_tag_tree(tag_t *tag)
{
	tag_t *next;

	if((tag) && (tag->marker == TAG_MARKER)) {
		do {
			next = tag->next;
			if(tag->first_child)
				delete_tag_tree(tag->first_child);

			/* delete node */
			if((tag) && (tag->marker == TAG_MARKER)) {
				DeleteCount++;
				if(tag->name)
					free(tag->name);
				if(tag->value)
					free(tag->value);
				if(tag->attr)
					free(tag->attr);
				free(tag);
			}
			tag = next;
		} while(tag);
	}
}

int xml_init(request *req)
{
	TagError = 0;
	req->prolog_tag = NULL;
	req->root_tag = NULL;
	req->tag_count = 0;
	return R_REQUEST_OK;
}

void xml_cleanup(request *req)
{
	delete_tag_tree(req->prolog_tag);
	delete_tag_tree(req->root_tag);
}

