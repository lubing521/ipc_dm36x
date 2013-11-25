/*
 * Functions for adding and deleting nodes from a tree
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <psia.h>
#include <defines.h>

#define PSIA_MEDIA

static int BuildTreeError = 0;
static int NodeCount = 0;

node_t *add_node(node_t *parent, const char *name, int methods, int type, const char *version, void *fn)
{
	node_t *node, *empty;

	/* must have a parent, unless this is root */
	if((!parent) && (NodeCount)) {
		BuildTreeError = 2;
		return NULL;
	}

	node = malloc(sizeof(node_t));
	if(!node) {
		BuildTreeError = 1;
		return NULL;
	}

	memset(node, 0, sizeof(node_t));
	node->len = strlen(name);
	node->name = malloc(node->len+1);

	if(!node->name) {
		free(node);
		BuildTreeError = 3;
		return NULL;
	}

	sprintf(node->name, "%s", name);
	node->marker = NODE_MARKER;
	node->count = NodeCount;
	node->methods = methods;
	node->type = type;
	node->process = fn;
	node->parent = parent;
	node->version = version;

	/* add node to parent's child list */
	if(parent) {
		if(!parent->first_child) {
			parent->first_child = node;
		}
		else {
			empty = parent->first_child;
			while(empty->next)
				empty = empty->next;
			empty->next = node;
		}
	}
	NodeCount++;
	return node;
}

static int DeleteCount = 0;
void delete_tree(node_t *node)
{
	node_t *next;

	if((node) && (node->marker == NODE_MARKER)) {
		do {
			next = node->next;
			if(node->first_child)
				delete_tree(node->first_child);

			/* delete node */
			if((node) && (node->marker == NODE_MARKER)) {
				DeleteCount++;
				if(node->name)
					free(node->name);
				free(node);
			}

			node = next;
		} while(node);
	}
}

int build_tree_error(void)
{
	return ((BuildTreeError ? R_SERVICE_UNAV : R_REQUEST_OK));
}

/* process all resources in the branch */
static int bnest = 0;
int process_branch(request *req, node_t *parent)
{
	char *cmd;
	node_t *node;
	int sslen;
	int branch;
	char sstr[64];
	char *nextsstr;
	int status = R_NOT_FOUND;

	/* special case for recursive index from upper level */
	if(req->indexr_flag) {
		cmd = (char *)"/indexr";
	}
	else {
		/* assume there is only one occurence of the parent name in the command */
		cmd = strstr(req->command, parent->name);
		if(!cmd) return status;
		cmd += parent->len;
	}

	/* process the next substring, skip the leading '/' and look for the next one... */
	nextsstr = strstr(cmd+1, "/");
	if(!nextsstr) {
		/* at the lowest level, the cmd should be a child of this parent */
		sslen = strlen(cmd);
		branch = 0;
	}
	else {
		/* need to branch to a lower level */
		sslen = nextsstr - cmd;
		branch = 1;
	}
	strncpy(sstr, cmd, sslen);
	sstr[sslen] = 0x00;

	bnest++;
	node = parent->first_child;
	while(node) {
		if (branch) {
			/* process service nodes if there is more in the command buffer */
			if((node->type == NODE_SERVICE) && (sslen == node->len) && (!strcmp(node->name, sstr))) {
				status = node->process(req, node);
				goto Done;
			}
			if(node->first_child) {
				status = process_branch(req, node);
				if(status != R_NOT_FOUND)
					goto Done;
			}
		}
		else {
			if((sslen == node->len) && (!strcmp(node->name, sstr))) {
				status = node->process(req, node);
				goto Done;
			}
		}

		node = node->next;
	}
Done:
	bnest--;
	return status;
}

