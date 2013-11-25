/*
 * Process the /index and /indexr resources 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "psia.h"
#include "tags.h"
#include "iface.h"

static int index_level = 0;
static void get_index(request *req, node_t *me)
{
	node_t *node, *next;
	int idx;
	char path[256];

	index_level++;
	if(index_level > 10) {
		debug_printf("%s: nested too deep\n", __FUNCTION__);
		return;
	}

	/* build the full path for the URL in reverse order */
	node = me->parent;
	idx = sizeof(path)-1;
	path[idx] = 0x00;
	while(node) {
		idx -= node->len;
		memcpy(&path[idx], node->name, node->len);
		node = node->parent;
	}
	if(index_level == 1)
		xadd_stag_attr(req, TAG_ResourceList, "version=\"%s\" xmlns=\"urn:psialliance-org\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"", me->version);
	else
		xadd_stag(req, TAG_ResourceList);
	node = me->parent->first_child;
	while(node) {
		next = node->next;
		xadd_stag_attr(req, TAG_Resource, "xlink:type=\"simple\" xlink:href=\"%s%s\"", &path[idx], node->name);
		xadd_elem(req, TAG_name, &node->name[1]);
		if(node->type == NODE_SERVICE)
			xadd_elem(req, TAG_type, "service");
		else
			xadd_elem(req, TAG_type, "resource");

		/* process service and resource child nodes recursively */
		if(req->indexr_flag) {
			if (node->type==NODE_SERVICE)
				node->process(req, node);
			else if (node->first_child)
				get_index(req, node->first_child);
		}
		xadd_etag(req, TAG_Resource);
		node = next;
	}
	xadd_etag(req, TAG_ResourceList);
	index_level--;
}
/*
int process_index(request *req, node_t *me)
{
	if(req->method != M_GET)
		return R_METHOD_NA;
	get_index(req, me);
	req->status_code = STATUS_OK;
	return (get_http_status(req));
}

int process_indexr(request *req, node_t *me)
{
	if(req->method != M_GET)
		return R_METHOD_NA;
	req->indexr_flag = 1;		// set recursive index flag
	get_index(req, me);
	req->status_code = STATUS_OK;
	return (get_http_status(req));
}

*/