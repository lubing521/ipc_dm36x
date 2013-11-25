/*
 * Process the /description resource 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "psia.h"
#include "tags.h"
#include "iface.h"

void desc_get_query(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->get_query_str = str;
}

void desc_get_inbound(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->get_inbound_str = str;
}

void desc_get_function(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->get_function_str = str;
}

void desc_get_result(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->get_result_str = str;
}

void desc_get_notes(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->get_notes_str = str;
}

void desc_put_query(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->put_query_str = str;
}

void desc_put_inbound(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->put_inbound_str = str;
}

void desc_put_function(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->put_function_str = str;
}

void desc_put_result(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->put_result_str = str;
}

void desc_put_notes(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->put_notes_str = str;
}

void desc_post_query(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->post_query_str = str;
}

void desc_post_inbound(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->post_inbound_str = str;
}

void desc_post_function(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->post_function_str = str;
}

void desc_post_result(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->post_result_str = str;
}

void desc_post_notes(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->post_notes_str = str;
}

void desc_delete_query(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->del_query_str = str;
}

void desc_delete_inbound(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->del_inbound_str = str;
}

void desc_delete_function(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->del_function_str = str;
}

void desc_delete_result(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->del_result_str = str;
}

void desc_delete_notes(node_t *node, const char *str)
{
	if((node) && (node->marker == NODE_MARKER))
		node->del_notes_str = str;
}

int process_description(request *req, node_t *me)
{
	node_t *node, *next;

	if(req->method != M_GET)
		return R_METHOD_NA;

	if(!me->parent)
		return R_BAD_REQUEST;

	xadd_stag_attr(req, TAG_ResourceDescription, "version=\"%s\" xmlns=\"urn:psialliance-org\"", me->version);

	node = (me->parent->type != NODE_SERVICE) ? me->parent : me->parent->first_child;
	while(node) {
		next = (me->parent->type != NODE_SERVICE) ? NULL : node->next;
		xadd_elem(req, TAG_name, &node->name[1]);
		xadd_elem(req, TAG_version, node->version);
		if(node->type == NODE_SERVICE)
			xadd_elem(req, TAG_type, "service");
		else
			xadd_elem(req, TAG_type, "resource");
		if(node->methods & M_GET) {
			xadd_stag(req, TAG_get);
			if(node->get_query_str) 	xadd_elem(req, TAG_QueryStringParameterList, node->get_query_str);
			if(node->get_inbound_str)	xadd_elem(req, TAG_inboundData, node->get_inbound_str);
			if(node->get_function_str)	xadd_elem(req, TAG_function, node->get_function_str);
			if(node->get_result_str)	xadd_elem(req, TAG_returnResult, node->get_result_str);
			if(node->get_notes_str)		xadd_elem(req, TAG_notes, node->get_notes_str);
			xadd_etag(req, TAG_get);
		}
		if(node->methods & M_PUT) {
			xadd_stag(req, TAG_put);
			if(node->put_query_str) 	xadd_elem(req, TAG_QueryStringParameterList, node->put_query_str);
			if(node->put_inbound_str)	xadd_elem(req, TAG_inboundData, node->put_inbound_str);
			if(node->put_function_str)	xadd_elem(req, TAG_function, node->put_function_str);
			if(node->put_result_str)	xadd_elem(req, TAG_returnResult, node->put_result_str);
			if(node->put_notes_str)		xadd_elem(req, TAG_notes, node->put_notes_str);
			xadd_etag(req, TAG_put);
		}
		if(node->methods & M_POST) {
			xadd_stag(req, TAG_post);
			if(node->post_query_str) 	xadd_elem(req, TAG_QueryStringParameterList, node->post_query_str);
			if(node->post_inbound_str)	xadd_elem(req, TAG_inboundData, node->post_inbound_str);
			if(node->post_function_str)	xadd_elem(req, TAG_function, node->post_function_str);
			if(node->post_result_str)	xadd_elem(req, TAG_returnResult, node->post_result_str);
			if(node->post_notes_str)	xadd_elem(req, TAG_notes, node->post_notes_str);
			xadd_etag(req, TAG_post);
		}
		if(node->methods & M_DELETE) {
			xadd_stag(req, TAG_delete);
			if(node->del_query_str) 	xadd_elem(req, TAG_QueryStringParameterList, node->del_query_str);
			if(node->del_inbound_str)	xadd_elem(req, TAG_inboundData, node->del_inbound_str);
			if(node->del_function_str)	xadd_elem(req, TAG_function, node->del_function_str);
			if(node->del_result_str)	xadd_elem(req, TAG_returnResult, node->del_result_str);
			if(node->del_notes_str)		xadd_elem(req, TAG_notes, node->del_notes_str);
			xadd_etag(req, TAG_delete);
		}
		node = next;
	}
	xadd_etag(req, TAG_ResourceDescription);
	req->status_code = STATUS_OK;
	return (get_http_status(req));
}
