/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996,97 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996-2002 Jon Nelson <jnelson@boa.org>
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

/* $Id: request.c,v 1.112.2.3 2002/07/24 03:03:59 jnelson Exp $*/
#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include "boa.h"
#include <stddef.h> /* for offsetof */
#include "iface.h"
#include "tags.h"


#ifdef SERVER_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#ifdef DAVINCI_IPCAM
#include "appro_api.h"
#endif

//#define PRINT_REQ

#ifdef SERVER_SSL
extern int server_ssl;			/*the socket to listen for ssl requests*/
extern SSL_CTX *ctx;			/*The global connection context*/
extern int do_ssl;				/*do ssl sockets??*/
extern int do_sock;
#endif /*SERVER_SSL*/

int total_connections;
struct status status;

static int sockbufsize = SOCKETBUF_SIZE;

/* function prototypes located in this file only */
static void free_request(request ** list_head_addr, request * req);

#ifdef PSIA_MEDIA
#include <psia.h>
#define RDBFLEN    3000
typedef struct __PSIA_URI_HASH_TABLE {
	PSIA_URI *entry;

} PSIA_URI_HASH_TABLE;

PSIA_URI_HASH_TABLE *psia_uri_hash;

static int process_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_indexr(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_indexr(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_storage_volumes(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_storage_volumes(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_storage_volumes_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_storage_volumes_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_storage_volumes_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_storage_volumes_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_storage_volumes_id_status(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_storage_volumes_id_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_storage_volumes_id_format(request *req, node_t *node)
{
	int ret;
	if(req->method == M_PUT) {
		req->status_code = update_system_storage_volumes_id_format(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_storage_volumes_id_files(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_storage_volumes_id_files(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_storage_volumes_id_files(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_storage_volumes_id_files_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_storage_volumes_id_files_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_storage_volumes_id_files_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_storage_volumes_id_data(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_storage_volumes_id_data(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_storage_build_tree(node_t *parent)
{
	node_t *node, *nw_node, *id_node,*file_node;
	int n;
	char idstr[64];

	node = add_node(parent, "/volumes", M_GET, NODE_RESOURCE, "1.0", &system_storage_volumes);
	desc_get_function(node, "Access the device storage volumes");
	desc_get_result(node, TAG_StorageVolumeList);
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	nw_node = node;
	for(n = 0; n < get_storage_count(); n++) {
		get_storage_url(n, idstr);
		node = add_node(nw_node, idstr, M_GET, NODE_RESOURCE, "1.0", &system_storage_volumes_id);
		desc_get_function(node, "Get a particular storage volume");
		desc_get_result(node, TAG_StorageVolume);
		add_std_resources(node, &process_index, &process_indexr, &process_description, &system_storage_volumes_id_capabilities);

		id_node = node;
		node = add_node(id_node, "/status", M_GET, NODE_RESOURCE, "1.0", &system_storage_volumes_id_status);
		desc_get_function(node, "Get the status of a particular storage");
		desc_get_result(node, TAG_StorageVolumeStatus);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);

		node = add_node(id_node, "/format", M_PUT, NODE_RESOURCE, "1.0", &system_storage_volumes_id_format);
		desc_put_function(node, "Format a storage volume");
		desc_put_result(node, TAG_ResponseStatus);
		desc_put_notes(node, "ResponseStatus is returned after formatting of the storage volume");
		add_std_resources(node, &process_index, NULL, &process_description, NULL);

		node = add_node(id_node, "/files", M_GET | M_DELETE, NODE_RESOURCE, "1.0", &system_storage_volumes_id_files);
		desc_get_function(node, "Get the list of files stored on a particular storage");
		desc_get_result(node, TAG_StorageFileList);
		desc_delete_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);

		file_node = node;
	    for(n = 0; n < get_storage_count(); n++) {
		  get_storage_url(n, idstr);
		  node = add_node(file_node, "/ID", M_GET | M_DELETE, NODE_RESOURCE, "1.0", &system_storage_volumes_id_files_id);
		  desc_get_function(node, "Access and manipulate a file");
		  desc_get_result(node, TAG_StorageFile);
		  desc_delete_result(node, TAG_ResponseStatus);
		  add_std_resources(node, &process_index, NULL, &process_description, NULL);
        }

		node = add_node(id_node, "/data", M_GET, NODE_RESOURCE, "1.0", &system_storage_volumes_id_data);
		desc_get_function(node, "Access and manipulate a file");
		desc_get_result(node,"Raw file data");
		add_std_resources(node, &process_index, NULL, &process_description, NULL);
	}
	return (build_tree_error());
}

int process_system_storage(request *req, node_t *parent)
{
	int status;

	status = system_storage_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int system_network_interfaces(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_network_interfaces(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_network_interfaces_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_network_interfaces_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_network_interfaces_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_network_interfaces_id_ipaddress(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_network_interfaces_id_ipaddress(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_network_interfaces_id_ipaddress(req, node);
		add_response_status(req, node);

		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_network_interfaces_id_discovery(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_network_interfaces_id_discovery(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_network_interfaces_id_discovery(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int process_system_network_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_system_network_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_system_network_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_system_network_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_network_build_tree(node_t *parent)
{
	node_t *node, *nw_node, *id_node;
	int n;
	char idstr[64];

	node = add_node(parent, "/interfaces", M_GET, NODE_RESOURCE, "1.0", &system_network_interfaces);
	desc_get_function(node, "Access the device network interfaces");
	desc_get_result(node, TAG_NetworkInterfaceList);
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	nw_node = node;
	for(n = 0; n < get_network_count(); n++) {
		get_network_url(n, idstr);
		node = add_node(nw_node, idstr, M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_network_interfaces_id);
		desc_get_function(node, "Get the network interface configuration");
		desc_get_result(node, TAG_NetworkInterface);
		desc_put_function(node, "Set the network interface configuration");
		desc_put_inbound(node, TAG_NetworkInterface);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

		id_node = node;
		node = add_node(id_node, "/ipAddress", M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_network_interfaces_id_ipaddress);
		desc_get_function(node, "Get the device's IP address configuration");
		desc_get_result(node, TAG_IPAddress);
		desc_put_function(node, "Set the device's IP address configuration");
		desc_put_inbound(node, TAG_IPAddress);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);

		node = add_node(id_node, "/discovery", M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_network_interfaces_id_discovery);
		desc_get_function(node, "Get the device's discovery settings");
		desc_get_result(node, TAG_Discovery);
		desc_put_function(node, "Set the device's discovery settings");
		desc_put_inbound(node, TAG_Discovery);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);
	}
	return (build_tree_error());
}

int process_system_network(request *req, node_t *parent)
{
	int status;

	status = system_network_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int system_video_inputs(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_inputs(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_video_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_video_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_video_channels_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_overlays(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_overlays(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_video_channels_id_overlays(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_video_channels_id_overlays(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_privacymask(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_privacymask(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_video_channels_id_privacymask(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_privacymask_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_privacymask_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_overlays_text(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_overlays_text(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_video_channels_id_overlays_text(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_system_video_channels_id_overlays_text(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_video_channels_id_overlays_text(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_overlays_image(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_overlays_image(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_video_channels_id_overlays_image(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_video_channels_id_overlays_image(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_overlays_text_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_overlays_text_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_video_channels_id_overlays_text_id(req, node, req->root_tag);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_video_channels_id_overlays_text_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_overlays_text_id_capabilites(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_overlays_text_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_overlays_image_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_overlays_image_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_video_channels_id_overlays_image_id(req, node, req->root_tag);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_video_channels_id_overlays_image_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_video_channels_id_overlays_image_id_capabilites(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_video_channels_id_overlays_image_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_system_video_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_system_video_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_system_video_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_system_video_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_video_build_tree(node_t *parent)
{
	node_t *node, *nw_node, *overlay_node,*text_node,*textid_node, *imageid_node;
	int n,c;
	char idstr[64];

	node = add_node(parent, "/inputs", M_GET, NODE_RESOURCE, "1.0", &system_video_inputs);
	desc_get_function(node, "Access the video input channels");
	desc_get_result(node, TAG_VideoInput);
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	node = add_node(parent, "/channels", M_GET, NODE_RESOURCE, "1.0", &system_video_channels);
	desc_get_function(node, "Access the video input channelist");
	desc_get_result(node, TAG_VideoInputChannelList);
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	nw_node = node;
	for(n = 0; n < get_channel_count(); n++) {
		get_channel_url(n, idstr);
		node = add_node(nw_node, idstr, M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_video_channels_id);
		desc_get_function(node, "Get video input channel properties");
		desc_get_result(node, TAG_VideoInputChannel);
		desc_put_function(node, "Set video input channel properties");
		desc_put_inbound(node, TAG_VideoInputChannel);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, &system_video_channels_id_capabilities);

		overlay_node = node;
		node = add_node(overlay_node, "/overlays", M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &system_video_channels_id_overlays);
		desc_get_function(node, "Set an OSD overlay");
		desc_get_result(node, TAG_VideoOverlay);
		desc_put_inbound(node, TAG_VideoOverlay);
		desc_put_result(node, TAG_ResponseStatus);
		desc_delete_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);

        text_node = node;
		node = add_node(text_node, "/text", M_GET | M_PUT | M_POST | M_DELETE, NODE_RESOURCE, "1.0", &system_video_channels_id_overlays_text);
	    desc_get_function(node, "Overlay text configuration");
	    desc_get_result(node, TAG_TextOverlayList);
	    desc_put_inbound(node, TAG_TextOverlayList);
	    desc_put_result(node, TAG_ResponseStatus);
	    desc_post_inbound(node, TAG_TextOverlay);
	    desc_post_result(node, TAG_ResponseStatus);
	    desc_delete_result(node, TAG_ResponseStatus);
	    add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

		textid_node = node;
		for(c = 0; c < get_channel_count(); c++) {
		 get_channel_url(c, idstr);
		 node = add_node(textid_node, "/ID", M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &system_video_channels_id_overlays_text_id);
		 desc_get_function(node, "Access an Overlay text configuration");
		 desc_get_result(node, TAG_TextOverlay);
		 desc_put_inbound(node, TAG_TextOverlay);
		 desc_put_result(node, TAG_ResponseStatus);
		 desc_delete_result(node, TAG_ResponseStatus);
		 add_std_resources(node, &process_index, NULL, &process_description, &system_video_channels_id_overlays_text_id_capabilites);
		}

		node = add_node(text_node, "/image", M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &system_video_channels_id_overlays_image);
		desc_get_function(node, "Overlay image configuration");
		desc_get_result(node, TAG_ImageOverlayList);
		desc_put_inbound(node, TAG_ImageOverlay);
		desc_put_result(node, TAG_ResponseStatus);
		desc_delete_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);

		imageid_node = node;
		for(c = 0; c < get_channel_count(); c++) {
		 node = add_node(imageid_node, "/ID", M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &system_video_channels_id_overlays_image_id);
		 desc_get_function(node, "Access an Overlay text configuration");
		 desc_get_result(node, TAG_ImageOverlay);
		 desc_put_inbound(node, TAG_ImageOverlay);
		 desc_put_result(node, TAG_ResponseStatus);
		 desc_delete_result(node, TAG_ResponseStatus);
		 add_std_resources(node, &process_index, NULL, &process_description, &system_video_channels_id_overlays_image_id_capabilites);
		}

		node = add_node(overlay_node, "/privacyMask", M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_video_channels_id_privacymask);
		desc_get_function(node, "Get the privacy mask configuration");
		desc_get_result(node, TAG_PrivacyMask);
		desc_put_function(node, "Set the privacy mask configuration");
		desc_put_inbound(node, TAG_PrivacyMask);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, &system_video_channels_id_privacymask_capabilities);
	}
	return (build_tree_error());
}

int process_system_video(request *req, node_t *parent)
{
	int status;

	status = system_video_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int system_audio_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_audio_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_audio_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_audio_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_audio_channels_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_audio_channels_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_audio_channels_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}


static int system_audio_build_tree(node_t *parent)
{
	node_t *node, *nw_node;
	int n;
	char idstr[64];

	node = add_node(parent, "/channels", M_GET, NODE_RESOURCE, "1.0", &system_audio_channels);
	desc_get_function(node, "Access the audio input channelist");
	desc_get_result(node, TAG_AudioChannelList );
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	nw_node = node;
	for(n = 0; n < get_channel_count(); n++) {
		get_channel_url(n, idstr);
		node = add_node(nw_node, idstr, M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_audio_channels_id);
		desc_get_function(node, "Get audio input channel properties");
		desc_get_result(node, TAG_AudioChannel);
		desc_put_function(node, "Set audio input channel properties");
		desc_put_inbound(node, TAG_AudioChannel);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, &system_audio_channels_id_capabilities);
	}
	return (build_tree_error());
}

int process_system_audio(request *req, node_t *parent)
{
	int status;

	status = system_audio_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int system_reboot(request *req, node_t *node)
{
	int ret;
	if(req->method == M_PUT) {
		req->status_code = do_system_reboot(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		sleep(2);

		reboot_ipcam();
		return 0;
	}
	return R_METHOD_NA;
}

static int system_updatefirmware(request *req, node_t *node)
{
	int ret;
	if(req->method == M_PUT) {
		req->status_code = do_system_updatefirmware(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
        //sleep(2);

		//reboot_ipcam();
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_maint_update_filesys(request *req, node_t *node)
{
	int ret;
	if(req->method == M_PUT) {
		req->status_code = do_custom_maint_update_filesys(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_maint_restore_firmware(request *req, node_t *node)
{
	int ret;
	if(req->method == M_PUT) {
		req->status_code = do_custom_maint_restore_firmware(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_configurationdata(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_configurationdata(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_configurationdata(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_factoryreset(request *req, node_t *node)
{
	int ret;
	if(req->method == M_PUT) {
		req->status_code = do_system_factory_reset(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_deviceinfo(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_deviceinfo(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_deviceinfo(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_supportreport(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_supportreport(req, node);
		send_r_psia_supportReport(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_status(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_time(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_time(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_time(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_time_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_time_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_time_localtime(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_time_localtime(req, node);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_time_localtime(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_time_timezone(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_time_timezone(req, node);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_time_timezone(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_time_ntpservers(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_time_ntpservers(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_time_ntpservers(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_system_time_ntpservers(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_time_ntpservers(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_time_ntpservers_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_time_ntpservers_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_time_ntpservers_id(req, node, req->root_tag);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_system_time_ntpservers_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int system_logging(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_logging(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_logging(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_logging_messages(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_logging_messages(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_system_log(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_system_log(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_system_log(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_access_log(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_access_log(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_access_log(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_active_list(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_active_list(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_active_list(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_param_list(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_param_list(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_system_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_system_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_system_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_system_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_build_tree(node_t *parent)
{
	node_t *node, *time_node, *ntp_node;
	int n;
	char idstr[64];

	node = add_node(parent, "/reboot", M_PUT, NODE_RESOURCE, "1.0", &system_reboot);
	desc_put_function(node, "Reboot the device");
	desc_put_result(node, TAG_ResponseStatus);
	desc_put_notes(node, "ResponseStatus is returned before the device proceeds to reboot");
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(parent, "/updateFirmware", M_PUT, NODE_RESOURCE, "1.0", &system_updatefirmware);
	desc_put_function(node, "Update the firmware of the device");
	desc_put_result(node, TAG_ResponseStatus);
	desc_put_inbound(node, "New firmware");
	desc_put_notes(node, "After successful completion, ResponseStatus is returned and the device proceeds to reboot");
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(parent, "/configurationData", M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_configurationdata);
	desc_get_function(node, "Set the device configuration");
	desc_get_result(node, "Opaque data");
	desc_put_function(node, "Get the device information");
	desc_put_inbound(node, "Opaque data");
	desc_put_result(node, TAG_ResponseStatus);
	desc_put_notes(node, "May reboot device after configuration data is applied");
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(parent, "/factoryReset", M_PUT, NODE_RESOURCE, "1.0", &system_factoryreset);
	desc_put_function(node, "Reset the device's configuration to the factory default");
	desc_put_result(node, TAG_ResponseStatus);
	desc_put_notes(node, "May reboot device after reset");
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(parent, "/deviceInfo", M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_deviceinfo);
	desc_get_function(node, "Get the device information");
	desc_get_result(node, TAG_DeviceInfo);
	desc_put_function(node, "Set the device information");
	desc_put_inbound(node, TAG_DeviceInfo);
	desc_put_result(node, TAG_ResponseStatus);
	desc_put_notes(node, "Only fields requiring a change need to be uploaded. Read-only fields are ignored.");
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(parent, "/supportReport", M_GET, NODE_RESOURCE, "1.0", &system_supportreport);
	desc_get_function(node, "Retrieve current configuration and log file");
	desc_get_result(node, "Support data");
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(parent, "/status", M_GET, NODE_RESOURCE, "1.0", &system_status);
	desc_get_function(node, "Get the status of the device");
	desc_get_result(node, TAG_DeviceStatus);
	desc_get_notes(node, "Not all fields in DeviceStatus may be present");
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(parent, "/time", M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_time);
	desc_get_function(node, "Access the time information");
	desc_get_result(node, TAG_Time);
	desc_put_query(node, "timeMode,localTime,timeZone");
	desc_put_inbound(node, TAG_Time);
	desc_put_result(node, TAG_ResponseStatus);
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	time_node = node;
	node = add_node(time_node, "/localTime", M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_time_localtime);
	desc_get_function(node, "Access the local time information");
	desc_get_result(node, "ISO 8601 Date-Time string");
	desc_put_inbound(node, "ISO 8601 Date-Time string");
	desc_put_result(node, TAG_ResponseStatus);
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(time_node, "/timeZone", M_GET | M_PUT, NODE_RESOURCE, "1.0", &system_time_timezone);
	desc_get_function(node, "Access the time zone");
	desc_get_result(node, "Time zone string");
	desc_put_inbound(node, "Time zone string");
	desc_put_result(node, TAG_ResponseStatus);
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(time_node, "/ntpServers", M_GET | M_PUT | M_POST | M_DELETE, NODE_RESOURCE, "1.0", &system_time_ntpservers);
	desc_get_function(node, "Access the NTP servers");
	desc_get_result(node, TAG_NtpServerList);
	desc_put_inbound(node, TAG_NtpServerList);
	desc_put_result(node, TAG_ResponseStatus);
	desc_post_inbound(node, TAG_NtpServer);
	desc_post_result(node, TAG_ResponseStatus);
	desc_delete_result(node, TAG_ResponseStatus);
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	ntp_node = node;
	for(n = 0; n < get_ntpserver_count(); n++) {
		get_ntpserver_url(n, idstr);
		node = add_node(ntp_node, idstr, M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &system_time_ntpservers_id);
		desc_get_function(node, "Access an NTP server");
		desc_get_result(node, TAG_NtpServer);
		desc_put_inbound(node, TAG_NtpServer);
		desc_put_result(node, TAG_ResponseStatus);
		desc_delete_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);
	}

	node = add_node(parent, "/Storage", M_NULL, NODE_SERVICE, "1.0", &process_system_storage);
	desc_get_function(node, "Storage service");
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	node = add_node(parent, "/Network", M_NULL, NODE_SERVICE, "1.0", &process_system_network);
	desc_get_function(node, "Network service");
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	node = add_node(parent, "/Video", M_NULL, NODE_SERVICE, "1.0", &process_system_video);
	desc_get_function(node, "Video service");
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	node = add_node(parent, "/Audio", M_NULL, NODE_SERVICE, "1.0", &process_system_audio);
	desc_get_function(node, "Audio service");
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	return (build_tree_error());
}

int process_system(request *req, node_t *parent)
{
	int status;

	status = system_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int security_aaa_users(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retieve_security_aaa_users(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_security_aaa_users(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_security_aaa_users(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_security_aaa_users(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int security_aaa_users_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retieve_security_aaa_users_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_security_aaa_users_id(req, node, req->root_tag);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_security_aaa_users_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int security_aaa_users_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_security_aaa_users_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_security_aaa_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_security_aaa_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_security_aaa_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_security_aaa_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int security_aaa_build_tree(node_t *parent)
{
	node_t *node, *user_node;
	int u;
	char idstr[64];

	node = add_node(parent, "/users", M_GET | M_PUT | M_POST | M_DELETE, NODE_RESOURCE, "1.0", &security_aaa_users);
	desc_get_function(node, "Retrieve user list");
	desc_get_result(node, TAG_UserList);
	desc_put_function(node, "Update user list");
	desc_put_inbound(node, TAG_UserList);
	desc_put_result(node, TAG_ResponseStatus);
	desc_post_function(node, "Add new user to list");
	desc_post_inbound(node, TAG_User);
	desc_post_result(node, TAG_ResponseStatus);
	desc_delete_function(node, "Delete user from list");
	desc_delete_result(node, TAG_ResponseStatus);
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	user_node = node;
	for(u = 0; u < get_user_count(); u++) {
		//get_user_url(u, idstr);
		node = add_node(user_node, idstr, M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &security_aaa_users_id);
		desc_get_function(node, "Retrieve user");
		desc_get_result(node, TAG_User);
		desc_put_function(node, "Update user");
		desc_put_inbound(node, TAG_User);
		desc_put_result(node, TAG_ResponseStatus);
		desc_put_function(node, "Delete user");
		desc_delete_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, NULL, &process_description, &security_aaa_users_id_capabilities);
	}
	return (build_tree_error());
}

static int process_security_aaa(request *req, node_t *parent)
{
	int status;

	status = security_aaa_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int process_security_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_security_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_security_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_security_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int security_build_tree(node_t *parent)
{
	node_t *node;

	node = add_node(parent, "/AAA", M_NULL, NODE_SERVICE, "1.0", &process_security_aaa);
	desc_get_function(node, "Authentication, authorization and auditing service");
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);
	return (build_tree_error());
}

int process_security(request *req, node_t *parent)
{
	int status;

	status = security_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int streaming_status(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_streaming_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int streaming_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_streaming_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_streaming_channels(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_streaming_channels(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		//send_r_psia_not_implemented(req);

		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_streaming_channels(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		//send_r_psia_not_implemented(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int streaming_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_streaming_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_streaming_channels_id(req, node, req->root_tag);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_streaming_channels_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		//send_r_psia_not_implemented(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_video_advanced_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_video_advanced_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_video_advanced_channels(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
/*	else if(req->method == M_POST) {
		req->status_code = create_custom_video_advanced_channels(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_video_advanced_channels(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
*/	return R_METHOD_NA;
}

static int custom_video_advanced_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_video_advanced_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_video_advanced_channels_id(req, node, req->root_tag);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
/*	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_video_advanced_channels_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
*/	return R_METHOD_NA;
}

static int custom_video_advanced_channels_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_video_advanced_channels_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int streaming_channels_id_status(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_streaming_channels_id_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int streaming_channels_id_http(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_streaming_channels_id_http(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int streaming_channels_id_picture(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_streaming_channels_id_picture(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int streaming_channels_id_http_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_streaming_channels_id_http_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int streaming_channels_id_picture_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_streaming_channels_id_picture_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_streaming_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_streaming_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_streaming_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_streaming_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

int streaming_build_tree(node_t *parent)
{
	node_t *node, *ch_node, *id_node;
	int c;
	char idstr[64];

	node = add_node(parent, "/status", M_GET, NODE_RESOURCE, "1.0", &streaming_status);
	desc_get_function(node, "Device streaming status");
	desc_get_result(node, TAG_StreamingStatus);
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	node = add_node(parent, "/channels", M_GET, NODE_RESOURCE, "1.0", &streaming_channels);
	desc_get_function(node, "Streaming channels");
	desc_get_result(node, TAG_StreamingChannelList);
	add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

	ch_node = node;
	for(c = 0; c < get_channel_count(); c++) {
		get_channel_url(c, idstr);
		node = add_node(ch_node, idstr, M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &streaming_channels_id);
		desc_get_function(node, "Get the streaming channel configuration");
		desc_get_result(node, TAG_StreamingChannel);
		desc_put_function(node, "Set the streaming channel configuration");
		desc_put_inbound(node, TAG_StreamingChannel);
		desc_put_result(node, TAG_ResponseStatus);
		desc_delete_function(node, "Delete a streaming channel configuration");
	    desc_delete_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

		id_node = node;

		node = add_node(ch_node, idstr, M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &custom_video_advanced_channels);
		desc_get_function(node, "Get the streaming channel advanced configuration");
		desc_get_result(node, TAG_VideoAdvancedSetting);
		desc_put_function(node, "Set the streaming channel advanced configuration");
		desc_put_inbound(node, TAG_VideoAdvancedSetting);
		desc_put_result(node, TAG_ResponseStatus);
		desc_delete_function(node, "Delete a streaming channel advanced configuration");
	    desc_delete_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, &custom_video_advanced_channels_id_capabilities);

		node = add_node(id_node, "/status", M_GET, NODE_RESOURCE, "1.0", &streaming_channels_id_status);
		desc_get_function(node, "Get the list of streaming sessions for this channel");
		desc_get_result(node, TAG_StreamingSessionStatusList);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);

		node = add_node(id_node, "/http", M_GET | M_POST, NODE_RESOURCE, "1.0", &streaming_channels_id_http);
		desc_get_query(node, "none");
		desc_get_function(node, "Access a live stream via http");
		desc_get_result(node, "Stream over http");
		desc_post_query(node, "none");
		desc_post_function(node, "Access a live stream via http");
		desc_post_inbound(node, TAG_Video);
		desc_post_result(node, "Stream over http");
		add_std_resources(node, &process_index, NULL, &process_description, &streaming_channels_id_http_capabilities);

		node = add_node(id_node, "/picture", M_GET | M_POST, NODE_RESOURCE, "1.0", &streaming_channels_id_picture);
		desc_get_query(node, "none");
		desc_get_function(node, "Get a snapshot of the current image");
		desc_get_result(node, "Picture over http");
		desc_post_query(node, "none");
		desc_post_function(node, "Get a snapshot of the current image");
		desc_post_inbound(node, TAG_Video);
		desc_post_result(node, "Picture over http");
		add_std_resources(node, &process_index, NULL, &process_description, &streaming_channels_id_picture_capabilities);
	}
	return (build_tree_error());
}

int process_streaming(request *req, node_t *parent)
{
	int status;

	status = streaming_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int system_io(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_io(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_io_inputs(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_io_inputs(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_io_inputs_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_io_inputs_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_io_inputs_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_io_inputs_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_io_inputs_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_io_outputs(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_io_outputs(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_io_outputs_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_io_outputs_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_io_outputs_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_io_outputs_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_io_outputs_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_serial_ports(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_serial_ports(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int system_serial_ports_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_serial_ports_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_system_serial_ports_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int system_serial_ports_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_system_serial_ports_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_event(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}
static int custom_event_triggers(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_triggers(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_triggers(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_custom_event_triggers(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_triggers(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_triggers_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_triggers_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_triggers_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_triggers_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_triggers_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_triggers_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_event_triggers_id_notifications(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_triggers_id_notifications(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_triggers_id_notifications(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_custom_event_triggers_id_notifications(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_triggers_id_notifications(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_triggers_id_notifications_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_triggers_id_notifications_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_triggers_id_notifications_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_triggers_id_notifications_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_triggers_id_notifications_id_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_triggers_id_notifications_id_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_event_schedule(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_schedule(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_schedule(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_event_notification(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_notification(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_notification(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_event_notification_capabilities(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_notification_capabilities(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_event_notification_mailing(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_notification_mailing(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_notification_mailing(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_custom_event_notification_mailing(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_notification_mailing(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_notification_mailing_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_notification_mailing_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_notification_mailing_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_notification_mailing_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_notification_ftp(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_notification_ftp(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_notification_ftp(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_custom_event_notification_ftp(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_notification_ftp(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_notification_ftp_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_notification_ftp_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_notification_ftp_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_notification_ftp_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_notification_httpHost(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_notification_httpHost(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_notification_httpHost(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_custom_event_notification_httpHost(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_notification_httpHost(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_event_notification_httpHost_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_event_notification_httpHost_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_event_notification_httpHost_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_event_notification_httpHost_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_analytics_motiondetection(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_motiondetection(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}
static int custom_analytics_motiondetection_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_motiondetection_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_analytics_motiondetection_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_analytics_motiondetection_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_motiondetection_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_analytics_facedetection(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_facedetection(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_analytics_facedetection_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_facedetection_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_analytics_facedetection_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_analytics_facedetection_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_facedetection_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_analytics_facerecognition(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_facerecognition(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_analytics_facerecognition_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_facerecognition_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_analytics_facerecognition_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_analytics_facerecognition_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_facerecognition_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_motiondetection_id_regions(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_analytics_motiondetection_id_regions(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_POST) {
		req->status_code = create_custom_analytics_motiondetection_id_regions(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_analytics_motiondetection_id_regions(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_analytics_motiondetection_id_regions_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_motiondetection_id_regions_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_analytics_motiondetection_id_regions_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);
		ret = get_http_status(req);
		return 0;
	}
	else if(req->method == M_DELETE) {
		req->status_code = delete_custom_analytics_motiondetection_id_regions_id(req, node);
		add_response_status(req, node);
		return (get_http_status(req));
	}
	return R_METHOD_NA;
}

static int custom_analytics_motiondetection_id_regions_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_motiondetection_id_regions_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_analytics_motiondetection_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_motiondetection_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_analytics_motiondetection_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_analytics_motiondetection_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

int custom_build_tree(node_t *parent)
{
	node_t *node, *ch_node, *id_node, *nw_node;
	int c,n;
	char idstr[64];

	node = add_node(parent, "/motionDetection", M_GET, NODE_RESOURCE, "1.0", &custom_analytics_motiondetection);
	desc_get_function(node, "Motion Detection Configuration");
	desc_get_result(node, TAG_MotionDetectionList);
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

	ch_node = node;
	for(c = 0; c < get_channel_count(); c++) {
		get_channel_url(c, idstr);
		node = add_node(ch_node, idstr, M_GET | M_PUT, NODE_RESOURCE, "1.0", &custom_analytics_motiondetection_id);
		desc_get_function(node, "Get the motion detection configuration");
		desc_get_result(node, TAG_MotionDetection);
		desc_put_function(node, "Set the motion detection configuration");
		desc_put_inbound(node, TAG_MotionDetection);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

		id_node = node;
		node = add_node(id_node, "/regions", M_GET | M_PUT | M_POST | M_DELETE, NODE_RESOURCE, "1.0", &custom_analytics_motiondetection_id_regions);
	    desc_get_function(node, "List of regions for motion detection");
	    desc_get_result(node, TAG_MotionDetectionRegionList );
	    desc_put_inbound(node, TAG_MotionDetectionRegionList);
	    desc_put_result(node, TAG_ResponseStatus);
	    desc_post_inbound(node, TAG_MotionDetectionRegion);
	    desc_post_result(node, TAG_ResponseStatus);
	    desc_delete_result(node, TAG_ResponseStatus);
	    add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);

		nw_node = node;
		for(n = 0; n < get_region_count(); n++) {
		get_channel_url(n, idstr);
		node = add_node(nw_node, idstr, M_GET | M_PUT | M_DELETE, NODE_RESOURCE, "1.0", &custom_analytics_motiondetection_id_regions_id);
		desc_get_function(node, "Retrieve region of motion detection");
		desc_get_result(node, TAG_MotionDetectionRegion);
		desc_put_function(node, "Update region of motion detection");
		desc_put_inbound(node, TAG_MotionDetectionRegion);
		desc_put_result(node, TAG_ResponseStatus);
		desc_put_function(node, "Delete region of motion detection");
		desc_delete_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, NULL, &process_description, NULL);
	   }
	}

	node = add_node(parent, "/faceDetection", M_GET, NODE_RESOURCE, "1.0", &custom_analytics_facedetection);
	desc_get_function(node, "Face Detection Configuration");
	desc_get_result(node, TAG_FaceDetectionList);
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

    ch_node = node;
	for(c = 0; c < get_channel_count(); c++) {
		get_channel_url(c, idstr);
		node = add_node(ch_node, idstr, M_GET | M_PUT, NODE_RESOURCE, "1.0", &custom_analytics_facedetection_id);
		desc_get_function(node, "Get the face detection configuration");
		desc_get_result(node, TAG_FaceDetection);
		desc_put_function(node, "Set the face detection configuration");
		desc_put_inbound(node, TAG_FaceDetection);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, NULL);
	}

	node = add_node(parent, "/faceRecognition", M_GET, NODE_RESOURCE, "1.0", &custom_analytics_facerecognition);
	desc_get_function(node, "Face Recognition Configuration");
	desc_get_result(node, TAG_FaceRecognitionList);
	add_std_resources(node, &process_index, NULL, &process_description, NULL);

    ch_node = node;
	for(c = 0; c < get_channel_count(); c++) {
		get_channel_url(c, idstr);
		node = add_node(ch_node, idstr, M_GET | M_PUT, NODE_RESOURCE, "1.0", &custom_analytics_facerecognition_id);
		desc_get_function(node, "Get the face recognition configuration");
		desc_get_result(node, TAG_FaceRecognition);
		desc_put_function(node, "Set the face recognition configuration");
		desc_put_inbound(node, TAG_FaceRecognition);
		desc_put_result(node, TAG_ResponseStatus);
		add_std_resources(node, &process_index, &process_indexr, &process_description, &retrieve_custom_analytics_facerecognition_id_capabilities);
	}

	return (build_tree_error());
}

int custom_analytics(request *req, node_t *parent)
{
	int status;

	status = custom_build_tree(parent);
	if(status == R_REQUEST_OK)
		status = process_branch(req, parent);
	return status;
}

static int custom_live_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_live_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_live_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_live_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_live_channels_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_live_channels_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_live_channels_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_users(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_users(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_users_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_users_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_users_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_video_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_video_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_video_channels(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_video_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_video_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_video_channels_id(req, node, req->root_tag);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_video_channels_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_video_channels_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_camera_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_camera_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_camera_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_camera_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_camera_channels_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_camera_channels_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_camera_channels_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_audio_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_audio_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_audio_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_audio_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_audio_channels_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_datetime_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_datetime_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_datetime_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_datetime_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_datetime_channels_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_datetime_channels_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_datetime_channels_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_network_channels(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_network_channels(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_network_channels_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_network_channels_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_network_channels_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_network_channels_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_network_channels_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_alarm_triggers(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_alarm_triggers(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_alarm_triggers_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_alarm_triggers_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_alarm_triggers_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_alarm_triggers_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_alarm_triggers_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_storage(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_storage(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_storage_id(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_storage_id(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	else if(req->method == M_PUT) {
		req->status_code = update_custom_storage_id(req, node);
		add_response_status(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return 0;
	}
	return R_METHOD_NA;
}

static int custom_storage_id_capabilities(request *req, node_t *node)
{
	int ret;

	if(req->method == M_GET) {
		req->status_code = retrieve_custom_storage_id_capabilities(req, node);

		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int custom_reload(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_custom_reload(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_custom_index(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_custom_index(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}

static int process_custom_description(request *req, node_t *node)
{
	int ret;
	if(req->method == M_GET) {
		req->status_code = retrieve_process_custom_description(req, node);
		send_r_psia_get(req);

		ret = get_http_status(req);
		return req->status_code;
	}
	return R_METHOD_NA;
}
#endif

#ifdef DAVINCI_IPCAM

typedef struct __URI_HASH_TABLE {
	HTTP_URI *entry;

} URI_HASH_TABLE;

URI_HASH_TABLE *uri_hash;

int uri_jpeg(request * req)
{
  printf(" BOA: JPEG\n");

	req->http_stream = URI_STREAM_JPEG;
	req->status = WRITE;
	if (GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &req->av_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on operation: AV_OP_GET_MJPEG_SERIAL\n");
		return 0;
	}
	if (GetAVData(AV_OP_LOCK_MJPEG, req->av_data.serial, &req->av_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on operation: AV_OP_LOCK_MJPEG\n");
		return 0;
	}
	req->data_mem = (char*)req->av_data.ptr;
	req->filesize = req->av_data.size+16;
	send_request_ok_jpeg(req);
	return 1;
}

int uri_mjpeg(request * req)
{
  printf(" BOA: MJPEG\n");

	req->http_stream = URI_STREAM_MJPEG;
	req->status = WRITE;
	if (GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &req->av_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on operation: AV_OP_GET_MJPEG_SERIAL\n");
		return 0;
	}
	if (GetAVData(AV_OP_LOCK_MJPEG, req->av_data.serial, &req->av_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on operation: AV_OP_LOCK_MJPEG\n");
		return 0;
	}
	req->data_mem = (char*)req->av_data.ptr;
	req->filesize = req->av_data.size+16;

	req->serial_lock = req->av_data.serial;
	req->serial_book = req->av_data.serial+1;

	if (strncasecmp(req->request_uri, "/PSIA", 5) == 0)
	   send_r_psia_ok_mjpeg(req);
	else
	   send_request_ok_mjpeg(req);

	return 1;
}

int uri_mpeg4(request * req)
{
	AV_DATA vol_data;

  printf(" BOA: MPEG4\n");

	req->http_stream = URI_STREAM_MPEG4;
	req->status = WRITE;
	if(GetAVData(AV_OP_LOCK_MP4_VOL, -1, &vol_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on Get Vol data\n");
		return 0;
	}

	if(GetAVData(AV_OP_LOCK_MP4_IFRAME, -1, &req->av_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on AV_OP_LOCK_MP4_IFRAME\n");
		return 0;
	}

	req->filesize = req->av_data.size+vol_data.size;

	if (strncasecmp(req->request_uri, "/PSIA", 5) == 0)
	   send_r_psia_ok_mpeg4(req);
	else
	   send_request_ok_mpeg4(req);

	req_copy(req, (char*)vol_data.ptr, vol_data.size);
	GetAVData(AV_OP_UNLOCK_MP4_VOL, -1, NULL);

	req->data_mem = (char*)req->av_data.ptr;
	req->filesize = req->av_data.size+16;

	req->serial_lock = req->av_data.serial;
	req->serial_book = req->av_data.serial+1;
	return 1;
}

int uri_mpeg4cif(request * req)
{
	AV_DATA vol_data;

  printf(" BOA: MPEG4 CIF\n");

	req->http_stream = URI_STREAM_MPEG4CIF;
	req->status = WRITE;
	if(GetAVData(AV_OP_LOCK_MP4_CIF_VOL, -1, &vol_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on Get Vol data\n");
		return 0;
	}

	if(GetAVData(AV_OP_LOCK_MP4_CIF_IFRAME, -1, &req->av_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on AV_OP_LOCK_MP4_CIF_IFRAME\n");
		return 0;
	}
	req->filesize = req->av_data.size+vol_data.size;

	if (strncasecmp(req->request_uri, "/PSIA", 5) == 0)
	   send_r_psia_ok_mpeg4(req);
	else
	   send_request_ok_mpeg4(req);

	req_copy(req, (char*)vol_data.ptr, vol_data.size);
	GetAVData(AV_OP_UNLOCK_MP4_CIF_VOL, -1, NULL);

	req->data_mem = (char*)req->av_data.ptr;
	req->filesize = req->av_data.size+16;

	req->serial_lock = req->av_data.serial;
	req->serial_book = req->av_data.serial+1;
	return 1;
}

int uri_avc(request * req)
{
	AV_DATA vol_data;

    printf(" BOA: H264\n");

	req->http_stream = URI_STREAM_AVC;
	req->status = WRITE;
	if(GetAVData(AV_OP_LOCK_MP4_VOL, -1, &vol_data) != RET_SUCCESS) {
		send_r_error(req);
		printf("Error on Get Vol data\n");
		return 0;
	}

	if(GetAVData(AV_OP_LOCK_MP4_IFRAME, -1, &req->av_data) != RET_SUCCESS) {
		send_r_error(req);
		printf("Error on AV_OP_LOCK_MP4_IFRAME\n");
		return 0;
	}

	req->filesize = req->av_data.size+vol_data.size;

	if (strncasecmp(req->request_uri, "/PSIA", 5) == 0)
	   send_r_psia_ok_avc(req);
	else
	   send_request_ok_avc(req);

	req_copy(req, (char*)vol_data.ptr, vol_data.size);
	GetAVData(AV_OP_UNLOCK_MP4_VOL, -1, NULL);

	req->data_mem = (char*)req->av_data.ptr;
	req->filesize = req->av_data.size+16;

	req->serial_lock = req->av_data.serial;
	req->serial_book = req->av_data.serial+1;
	return 1;
}

int uri_avccif(request * req)
{
	AV_DATA vol_data;

  printf(" BOA: H264 CIF\n");

	req->http_stream = URI_STREAM_AVCCIF;
	req->status = WRITE;
	if(GetAVData(AV_OP_LOCK_MP4_CIF_VOL, -1, &vol_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on Get Vol data\n");
		return 0;
	}

	if(GetAVData(AV_OP_LOCK_MP4_CIF_IFRAME, -1, &req->av_data) != RET_SUCCESS) {
		send_r_error(req);
		dbg("Error on AV_OP_LOCK_MP4_CIF_IFRAME\n");
		return 0;
	}
	req->filesize = req->av_data.size+vol_data.size;

	if (strncasecmp(req->request_uri, "/PSIA", 5) == 0)
	   send_r_psia_ok_avc(req);
	else
	   send_request_ok_avc(req);

	req_copy(req, (char*)vol_data.ptr, vol_data.size);
	GetAVData(AV_OP_UNLOCK_MP4_CIF_VOL, -1, NULL);

	req->data_mem = (char*)req->av_data.ptr;
	req->filesize = req->av_data.size+16;

	req->serial_lock = req->av_data.serial;
	req->serial_book = req->av_data.serial+1;
	return 1;
}

int uri_vb_htm(request * req)
{
	req->is_cgi = CGI;
        if (uri_decoding(req, req->query_string) < 0) {
            send_r_bad_request(req);
            return 0;
        }
        SQUASH_KA(req);
        http_run_command(req, req->cmd_arg, req->cmd_count);
        req->status = DONE;
        return 1;
}

int uri_aevt_htm(request * req)
{
        if (uri_decoding(req, req->query_string) >= 0) {
	        http_run_command(req, req->cmd_arg, req->cmd_count);
              reset_output_buffer(req);
        }
        return -1;
}

int uri_ini_htm(request * req)
{
        char *addr = (char *)malloc(MAX_HTML_LENGTH);
        if (addr == NULL) {
            send_r_error(req);
            return 0;
        }
        SQUASH_KA(req);
        req->mem_flag |= MFLAG_IS_MEMORY;
        req->data_mem = addr;
        req->filesize = html_ini_cmd(req->authority, addr, MAX_HTML_LENGTH);
        send_r_request_ok(req);     /* All's well */
        req->status = WRITE;
        return 1;
}

int uri_keyword_htm(request * req)
{
        char *addr = (char *)malloc(MAX_HTML_LENGTH);
        if (addr == NULL) {
            send_r_error(req);
            return 0;
        }
        SQUASH_KA(req);
        req->mem_flag |= MFLAG_IS_MEMORY;
        req->data_mem = addr;
        req->filesize = html_keyword_cmd(req->authority, addr, MAX_HTML_LENGTH);
        send_r_request_ok(req);     /* All's well */
        req->status = WRITE;
        return 1;
}

int uri_cmdlist_htm(request * req)
{
        char *addr = (char *)malloc(MAX_HTML_LENGTH);
        if (addr == NULL) {
            send_r_error(req);
            return 0;
        }
        SQUASH_KA(req);
        req->mem_flag |= MFLAG_IS_MEMORY;
        req->data_mem = addr;
        req->filesize = html_cmdlist_cmd(req->authority, addr, MAX_HTML_LENGTH);
        send_r_request_ok(req);     /* All's well */
        req->status = WRITE;
        return 1;
}

int uri_sdget_htm(request * req)
{
        char *addr, sdpath[128] = {0};
        if (req->query_string && uri_decoding(req, req->query_string) < 0) {
			send_r_bad_request(req);
            return 0;
        }

        addr = (char *)malloc(MAX_SDLIST_LENGTH);
        if (addr == NULL) {
            send_r_error(req);
            return 0;
        }
        req->filesize = http_sdget_cmd(req, req->cmd_arg, addr, MAX_SDLIST_LENGTH, sdpath);
		if (req->filesize > 0) {
            SQUASH_KA(req);
            req->mem_flag |= MFLAG_IS_MEMORY;
            req->data_mem = addr;
            send_request_ok_sdget(req);
            req->status = WRITE;
            return 1;
        }
        free(addr);
        if (req->pathname){
            free(req->pathname);
		}
        strcat(sdpath, "/");
        if(req->cmd_arg[0].value==NULL){
			send_r_bad_request(req);
			dbg("req->cmd_arg[0].value is NULL, no SD card\n");
			return 0;
		}
		strcat(sdpath, req->cmd_arg[0].value);
        req->pathname = strdup(sdpath);
        if (!req->pathname) {
			send_r_error(req);
            WARN("unable to strdup buffer onto req->pathname");
            return 0;
        }
		strncpy(req->request_uri, req->cmd_arg[0].value, MAX_HEADER_LENGTH);
		return -1;

}

int uri_sddel_htm(request * req)
{
        SQUASH_KA(req);
        if (uri_decoding(req, req->query_string) < 0) {
            send_r_bad_request(req);
            return 0;
        }
        http_sddel_cmd(req, req->cmd_arg);
        req->status = DONE;
        return 1;
}

int uri_sysget_htm(request * req)
{
        char *addr = (char *)malloc(MAX_HTML_LENGTH);
        if (addr == NULL) {
            send_r_error(req);
            return 0;
        }
        SQUASH_KA(req);
        req->mem_flag |= MFLAG_IS_MEMORY;
        req->data_mem = addr;
        req->filesize = http_sysget_cmd(addr, MAX_HTML_LENGTH);
        send_r_request_ok(req);     /* All's well */
        req->status = WRITE;
        return 1;

}

int uri_accessget_htm(request * req)
{
        char *addr = (char *)malloc(MAX_HTML_LENGTH);
        if (addr == NULL) {
            send_r_error(req);
            return 0;
        }
        SQUASH_KA(req);
        req->mem_flag |= MFLAG_IS_MEMORY;
        req->data_mem = addr;
        req->filesize = http_accessget_cmd(addr, MAX_HTML_LENGTH);
        send_r_request_ok(req);     /* All's well */
        req->status = WRITE;
        return 1;

}

int uri_currlistget_htm(request * req)
{
        char *addr = (char *)malloc(MAX_HTML_LENGTH);
        if (addr == NULL) {
            send_r_error(req);
            return 0;
        }
        SQUASH_KA(req);
        req->mem_flag |= MFLAG_IS_MEMORY;
        req->data_mem = addr;
        req->filesize = http_actvconlistget_cmd(addr, MAX_HTML_LENGTH);
        send_r_request_ok(req);     /* All's well */
        req->status = WRITE;
        return 1;

}

int uri_paraget_htm(request * req)
{
        char *addr = (char *)malloc(MAX_HTML_LENGTH);
        if (addr == NULL) {
            send_r_error(req);
            return 0;
        }
        SQUASH_KA(req);
        req->mem_flag |= MFLAG_IS_MEMORY;
        req->data_mem = addr;
        req->filesize = http_paraget_cmd(addr, MAX_HTML_LENGTH);
        send_r_request_ok(req);     /* All's well */
        req->status = WRITE;
        return 1;

}

HTTP_URI HttpUri [] =
{
    {JPEG_FRAME_URI		,uri_jpeg		,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/ipcam/mjpeg.cgi"		,uri_mjpeg		,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/ipcam/mjpegcif.cgi"	,uri_mjpeg		,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/ipcam/mpeg4.cgi"		,uri_mpeg4		,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/ipcam/mpeg4cif.cgi"	,uri_mpeg4cif	,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/ipcam/avc.cgi"		,uri_avc		,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/ipcam/avccif.cgi"	,uri_avccif		,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/vb.htm"			,uri_vb_htm			,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/ini.htm"			,uri_ini_htm		,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/keyword.htm"		,uri_keyword_htm	,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/cmdlist.htm"		,uri_cmdlist_htm	,AUTHORITY_VIEWER   ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/sdget.htm"		,uri_sdget_htm		,AUTHORITY_VIEWER ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/sddel.htm"		,uri_sddel_htm		,AUTHORITY_VIEWER ,URI_FLAG_VIRTUAL_PAGE ,NULL },
    {"/sysget.htm"		,uri_sysget_htm		,AUTHORITY_VIEWER ,URI_FLAG_VIRTUAL_PAGE ,NULL },   //rajiv
    {"/accessget.htm"	,uri_accessget_htm		,AUTHORITY_VIEWER ,URI_FLAG_VIRTUAL_PAGE ,NULL },   //rajiv
    {"/actvlistget.htm"		,uri_currlistget_htm		,AUTHORITY_VIEWER ,URI_FLAG_VIRTUAL_PAGE ,NULL },   //rajiv
    {"/paraget.htm"		,uri_paraget_htm		,AUTHORITY_VIEWER ,URI_FLAG_VIRTUAL_PAGE ,NULL },   //rajiv
    {"/"				,NULL				,AUTHORITY_VIEWER   ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/aenable.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/aevt.htm"                ,uri_aevt_htm		,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/armio.htm"               ,NULL			,AUTHORITY_ADMIN    ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/axtext.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/bwcntl.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_armio.htm"             ,NULL			,AUTHORITY_ADMIN    ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_axtext.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_bwcntl.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_dv840output.htm"       ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_help.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_lang.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_mcenter.htm"           ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/cms.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_ndhcpsvr.htm"          ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_nftphost.htm"          ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_nupnp.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_sccd.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_svideo.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_tailpage.htm"          ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/c_update.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/datetime.htm"            ,NULL			,AUTHORITY_VIEWER   ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/dv840output.htm"         ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/faq_cs_cz.html"          ,NULL			,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_de_de.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_en_us.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_es_es.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_fi_fi.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_fr_fr.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_hu_hu.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_it_it.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_nl_nl.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_pl_pl.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_pt_pt.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_ro_ro.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_sv_se.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_zh_cn.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/faq_zh_tw.html"          ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/help.htm"                ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/image.htm"               ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/imgdaynight.htm"         ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/img_h264.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/img.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/imgmask.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/imgtune.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/index.htm"               ,NULL			,AUTHORITY_VIEWER   ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/index.html"              ,NULL			,AUTHORITY_VIEWER   ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/K_dragon.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/K_fastrax.htm"           ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/K_lilin.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/K_seeku.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/K_sensor.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/lang.htm"                ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/logout.htm"		,NULL			,AUTHORITY_VIEWER   ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/main.htm"                ,NULL			,AUTHORITY_VIEWER   ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/mcenter.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/motion.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/nddns.htm"               ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/ndhcpsvr.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/net.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/nftphost.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/nftp.htm"                ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/nipfilter.htm"           ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/npppoe.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/nsmtp.htm"               ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/nsntp.htm"               ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/multicast.htm"           ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/nupnp.htm"               ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/nwireless.htm"           ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/pda.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/pppoedes_cs_cz.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_de_de.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_en_us.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_es_es.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_fi_fi.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_fr_fr.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_hu_hu.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_it_it.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_nl_nl.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_pl_pl.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_pt_pt.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_ro_ro.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_sv_se.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_zh_cn.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/pppoedes_zh_tw.html"     ,NULL                   ,AUTHORITY_OPERATOR ,0 ,NULL },
    {"/p_pelcod.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/p_pelcop.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/ptz.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/ptz_left.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/renable.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/rsch.htm"                ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/saudio.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/sccd.htm"                ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/sdigital.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/sdt.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/setcard.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/setftp.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/setsmtp.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/setvid.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/srs.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/sseq.htm"                ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/sts.htm"                 ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/suser.htm"               ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/svideo.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/tailpage.htm"            ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/update.cgi"		,NULL 			,AUTHORITY_OPERATOR ,0                     ,NULL },
    {"/update.htm"              ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/version.htm"             ,NULL			,AUTHORITY_OPERATOR ,URI_FLAG_NEED_PARSE   ,NULL },
    {"/var.js"			,NULL			,AUTHORITY_VIEWER   ,URI_FLAG_NEED_PARSE   ,NULL },
};

#define URI_HASH_SIZE	(sizeof(HttpUri)/sizeof(HTTP_URI))

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
unsigned int uri_hash_cal_value(char *name)
{
	    unsigned int value = 0;

	    while (*name)
	    	value = value * 37 + (unsigned int)(*name++);
	    return value;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void uri_hash_insert_entry(URI_HASH_TABLE *table, HTTP_URI *arg)
{
	  if (table->entry) {
	  	arg->next = table->entry;
	  }
	  table->entry = arg;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int uri_hash_table_init(void)
{
	  int i;

	if ( (uri_hash = (URI_HASH_TABLE *)calloc(sizeof(URI_HASH_TABLE), MAX_URI_HASH_SIZE)) == NULL) {
		dbg("uri_hash_table_init: Allocate memory fail!!!\n");
		return -1;
	}
	for (i=0; i<URI_HASH_SIZE; i++) {
		uri_hash_insert_entry(uri_hash+(uri_hash_cal_value(HttpUri[i].name)%MAX_URI_HASH_SIZE), HttpUri+i);
	}
	return 0;
}

HTTP_URI *http_uri_search(char *arg)
{
	HTTP_URI *opt;

	opt = uri_hash[uri_hash_cal_value(arg)%MAX_URI_HASH_SIZE].entry;

	while (opt) {
		if ( strcasecmp(opt->name, arg) == 0 )
			return opt;
		opt = opt->next;
	}
	return NULL;
}
#endif

#ifdef PSIA_MEDIA

PSIA_URI PsiaUri [] =
{
    {"/PSIA/index"		               ,process_index	          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/indexr"		               ,process_indexr	          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/description"		       ,process_description	      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System"		               ,process_system	          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/index"		       ,process_system_index	          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/description"		   ,process_system_description	      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/reboot"		       ,system_reboot	          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/updateFirmware"	   ,system_updatefirmware     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/configurationData"  ,system_configurationdata  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/factoryReset"	   ,system_factoryreset       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/deviceInfo"	       ,system_deviceinfo	      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/supportReport"	   ,system_supportreport      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/status"	           ,system_status             ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/time"		       ,system_time	              ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/time/capabilities"  ,system_time_capabilities  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/localTime"	       ,system_time_localtime     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/time/timeZone"	   ,system_time_timezone	  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/time/ntpServers"	   ,system_time_ntpservers    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/time/ntpServers/0" ,system_time_ntpservers_id ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/logging"            ,system_logging            ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/logging/messages"   ,system_logging_messages   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Storage"		                 ,process_system_storage	         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/Storage/volumes"	             ,system_storage_volumes             ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Storage/volumes/0"	         ,system_storage_volumes_id	         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Storage/volumes/0/capabilities"	         ,system_storage_volumes_id_capabilities	         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/Storage/volumes/0/status"	 ,system_storage_volumes_id_status   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Storage/volumes/0/format"    ,system_storage_volumes_id_format   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Storage/volumes/0/files"     ,system_storage_volumes_id_files    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Storage/volumes/0/files/0"  ,system_storage_volumes_id_files_id ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Storage/volumes/0/data"	     ,system_storage_volumes_id_data     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Network"                          ,process_system_network                 ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Network/index"                    ,process_system_network_index           ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Network/description"              ,process_system_network_description       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Network/interfaces"               ,system_network_interfaces              ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Network/interfaces/0"	         ,system_network_interfaces_id	         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/Network/interfaces/0/ipAddress"	 ,system_network_interfaces_id_ipaddress ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Network/interfaces/0/discovery"   ,system_network_interfaces_id_discovery ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video"		                     ,process_system_video	       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/index"		            ,process_system_video_index	       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/description"		    ,process_system_video_description	       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs"	                 ,system_video_inputs          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels"		     ,system_video_channels	       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0"	      ,system_video_channels_id     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/capabilities"	     ,system_video_channels_id_capabilities        ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays"	         ,system_video_channels_id_overlays            ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/privacyMask"	     ,system_video_channels_id_privacymask         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/privacyMask/capabilities"	     ,system_video_channels_id_privacymask_capabilities         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/text"	     ,system_video_channels_id_overlays_text       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/image"	     ,system_video_channels_id_overlays_image      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/text/0"	 ,system_video_channels_id_overlays_text_id    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/text/1"	 ,system_video_channels_id_overlays_text_id    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/text/2"	 ,system_video_channels_id_overlays_text_id    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/text/0/capabilities"	 ,system_video_channels_id_overlays_text_id_capabilites    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/text/1/capabilities"	 ,system_video_channels_id_overlays_text_id_capabilites    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/text/2/capabilities"	 ,system_video_channels_id_overlays_text_id_capabilites    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/image/0"	             ,system_video_channels_id_overlays_image_id   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/image/1"	             ,system_video_channels_id_overlays_image_id   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/image/2"	             ,system_video_channels_id_overlays_image_id   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/image/0/capabilities"	 ,system_video_channels_id_overlays_image_id_capabilites   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/image/1/capabilities"	 ,system_video_channels_id_overlays_image_id_capabilites   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Video/inputs/channels/0/overlays/image/2/capabilities"	 ,system_video_channels_id_overlays_image_id_capabilites   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Audio"               ,process_system_audio	       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Audio/channels"      ,system_audio_channels	       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Audio/channels/0"   ,system_audio_channels_id      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Audio/channels/0/capabilities"   ,system_audio_channels_id_capabilities      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security"	                   ,process_security          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/index"	               ,process_security_index          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/description"	       ,process_security_description    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/index"	           ,process_security_aaa_index          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/description"	   ,process_security_aaa_description    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/users"	           ,security_aaa_users        ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/users/"	       ,security_aaa_users_id	  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/users/0"	       ,security_aaa_users_id	  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/users/1"	       ,security_aaa_users_id	  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/users/2"	       ,security_aaa_users_id	  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/users/3"	       ,security_aaa_users_id	  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/AAA/users/4"	       ,security_aaa_users_id	  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Security/users/0/capabilities"	   ,security_aaa_users_id_capabilities	  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming"	                               ,process_streaming                      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/index"	                       ,process_streaming_index                ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/description"	                   ,process_streaming_description          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/status"	                       ,streaming_status                       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels"	                       ,streaming_channels	                   ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/Streaming/channels/"	                   ,streaming_channels_id                  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/0"	                   ,streaming_channels_id                  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/1"	                   ,streaming_channels_id                  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/2"	                   ,streaming_channels_id                  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/0/capabilities"    ,streaming_channels_id_http_capabilities    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/1/capabilities"    ,streaming_channels_id_http_capabilities    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/2/capabilities"    ,streaming_channels_id_http_capabilities    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/0/status"	           ,streaming_channels_id_status           ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/0/http"	               ,streaming_channels_id_http	           ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/1/http"	               ,streaming_channels_id_http	           ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/2/http"	               ,streaming_channels_id_http	           ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/0/picture"	           ,streaming_channels_id_picture               ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/1/picture"	           ,streaming_channels_id_picture               ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/2/picture"	           ,streaming_channels_id_picture               ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/0/http/capabilities"    ,streaming_channels_id_http_capabilities    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/1/http/capabilities"    ,streaming_channels_id_http_capabilities    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/2/http/capabilities"    ,streaming_channels_id_http_capabilities    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Streaming/channels/0/picture/capabilities" ,streaming_channels_id_picture_capabilities ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/System/IO"                  ,system_io                    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/IO/inputs"           ,system_io_inputs             ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/IO/inputs/0"        ,system_io_inputs_id          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/IO/inputs/0/capabilities"        ,system_io_inputs_id_capabilities          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/IO/outputs"          ,system_io_outputs            ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/IO/outputs/0"       ,system_io_outputs_id         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/IO/outputs/0/capabilities"       ,system_io_outputs_id_capabilities         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Serial/ports"        ,system_serial_ports          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Serial/ports/0"     ,system_serial_ports_id       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/System/Serial/ports/0/capabilities"     ,system_serial_ports_id_capabilities       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/index"	                       ,process_custom_index                ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/description"	                   ,process_custom_description          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event"                              ,custom_event                              ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/triggers"                     ,custom_event_triggers                     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/triggers/0"                  ,custom_event_triggers_id                  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/triggers/0/capabilities"     ,custom_event_triggers_id_capabilities     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/triggers/0/notifications"    ,custom_event_triggers_id_notifications    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/triggers/0/notifications/0" ,custom_event_triggers_id_notifications_id ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/triggers/0/notifications/0/capabilities" ,custom_event_triggers_id_notifications_id_capabilities ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/schedule"                     ,custom_event_schedule                     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/notification"                 ,custom_event_notification                 ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/notification/capabilities"    ,custom_event_notification_capabilities    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/notification/mailing"         ,custom_event_notification_mailing         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/notification/ftp"             ,custom_event_notification_ftp             ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/notification/httpHost"        ,custom_event_notification_httpHost        ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/notification/mailing/0"      ,custom_event_notification_mailing_id      ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/notification/ftp/0"          ,custom_event_notification_ftp_id          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Event/notification/httpHost/0"     ,custom_event_notification_httpHost_id     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Analytics"                           ,custom_analytics                                ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/MotionDetection"                     ,custom_analytics_motiondetection                ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/MotionDetection/index"               ,custom_analytics_motiondetection_index          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/MotionDetection/description"         ,custom_analytics_motiondetection_description    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/MotionDetection/0"                   ,custom_analytics_motiondetection_id             ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/MotionDetection/0/capabilities"     ,custom_analytics_motiondetection_id_capabilities             ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Analytics/faceDetection"            ,custom_analytics_facedetection             ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Analytics/faceDetection/0"         ,custom_analytics_facedetection_id          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Analytics/faceDetection/0/capabilities"         ,custom_analytics_facedetection_id_capabilities          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Analytics/faceRecognition"          ,custom_analytics_facerecognition           ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Analytics/faceRecognition/0"       ,custom_analytics_facerecognition_id        ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Analytics/faceRecognition/0/capabilities"       ,custom_analytics_facerecognition_id_capabilities        ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/MotionDetection/0/regions"                ,custom_analytics_motiondetection_id_regions     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/MotionDetection/0/regions/0"              ,custom_analytics_motiondetection_id_regions_id  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/MotionDetection/0/regions/0/capabilities" ,custom_analytics_motiondetection_id_regions_id_capabilities  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Live/channels"         ,custom_live_channels             ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Live/channels/0"       ,custom_live_channels_id          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
    {"/PSIA/Custom/Live/channels/0/capabilities"       ,custom_live_channels_id_capabilities          ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/User"                  ,custom_users                     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/User/0"                ,custom_users_id                  ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/channels"        ,custom_video_channels            ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/channels/0"      ,custom_video_channels_id         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/channels/1"      ,custom_video_channels_id         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/channels/2"      ,custom_video_channels_id         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/channels/0/capabilities"      ,custom_video_channels_id_capabilities         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/channels/1/capabilities"      ,custom_video_channels_id_capabilities         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/channels/2/capabilities"      ,custom_video_channels_id_capabilities         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/advanced/channels"	     ,custom_video_advanced_channels         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/advanced/channels/0"	 ,custom_video_advanced_channels_id       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/advanced/channels/1"	 ,custom_video_advanced_channels_id       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/advanced/channels/2"	 ,custom_video_advanced_channels_id       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/advanced/channels/0/capabilities"	 ,custom_video_advanced_channels_id_capabilities       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/advanced/channels/1/capabilities"	 ,custom_video_advanced_channels_id_capabilities       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Video/advanced/channels/2/capabilities"	 ,custom_video_advanced_channels_id_capabilities       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Camera/channels"	           ,custom_camera_channels         ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Camera/channels/0"	       ,custom_camera_channels_id       ,AUTHORITY_VIEWER  ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Camera/channels/0/capabilities"	   ,custom_camera_channels_id_capabilities       ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Audio/channels"	           ,custom_audio_channels          ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Audio/channels/0"	       ,custom_audio_channels_id       ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/DateTime/channels"	       ,custom_datetime_channels       ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/DateTime/channels/0"	       ,custom_datetime_channels_id    ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/DateTime/channels/0/capabilities"	       ,custom_datetime_channels_id_capabilities   ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Network"	           ,custom_network_channels       ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Network/0"	       ,custom_network_channels_id    ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Network/0/capabilities"	       ,custom_network_channels_id_capabilities    ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Alarm/triggers"	   ,custom_alarm_triggers       ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Alarm/triggers/0"   ,custom_alarm_triggers_id    ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Alarm/triggers/0/capabilities"   ,custom_alarm_triggers_id_capabilities    ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Storage"	           ,custom_storage           ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Storage/0"	       ,custom_storage_id        ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Storage/0/capabilities"	       ,custom_storage_id_capabilities        ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Reload"	           ,custom_reload        ,AUTHORITY_VIEWER    ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Maintenance/systemLog"         ,custom_system_log            ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Maintenance/accessLog"         ,custom_access_log            ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Maintenance/activeList"        ,custom_active_list           ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Maintenance/curParamList"      ,custom_param_list            ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Maintenance/updateFilesys"	   ,custom_maint_update_filesys     ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
	{"/PSIA/Custom/Maintenance/restoreFirmware"	   ,custom_maint_restore_firmware    ,AUTHORITY_VIEWER   ,PSIA_URI_FLAG_VIRTUAL_PAGE   ,NULL },
};

#define PSIA_URI_HASH_SIZE	(sizeof(PsiaUri)/sizeof(PSIA_URI))

unsigned int psia_uri_hash_cal_value(char *name)
{
	    unsigned int value = 0;

	    while (*name)
	    	value = value * 37 + (unsigned int)(*name++);
	    return value;
}

void psia_uri_hash_insert_entry(PSIA_URI_HASH_TABLE *table, PSIA_URI *arg)
{
	  if (table->entry) {
	  	arg->next = table->entry;
	  }
	  table->entry = arg;
}

int psia_uri_hash_table_init(void)
{
	int i;

	if ( (psia_uri_hash = (PSIA_URI_HASH_TABLE *)calloc(sizeof(PSIA_URI_HASH_TABLE), MAX_PSIA_URI_HASH_SIZE)) == NULL) {
		dbg("psia_uri_hash_table_init: Allocate memory fail!!!\n");
		return -1;
	}
	for (i=0; i<PSIA_URI_HASH_SIZE; i++) {
		psia_uri_hash_insert_entry(psia_uri_hash+(psia_uri_hash_cal_value(PsiaUri[i].name)%MAX_PSIA_URI_HASH_SIZE), PsiaUri+i);
	}
	return 0;
}

PSIA_URI *psia_uri_search(char *arg)
{
	PSIA_URI *opt;

	opt = psia_uri_hash[psia_uri_hash_cal_value(arg)%MAX_PSIA_URI_HASH_SIZE].entry;

	while (opt) {
		if ( strcasecmp(opt->name, arg) == 0 )
		{
			return opt;
        }
		opt = opt->next;
	}
	return NULL;
}
#endif

/*
 * Name: new_request
 * Description: Obtains a request struct off the free list, or if the
 * free list is empty, allocates memory
 *
 * Return value: pointer to initialized request
 */

request *new_request(void)
{
    request *req;

    if (request_free) {
        req = request_free;     /* first on free list */
        dequeue(&request_free, request_free); /* dequeue the head */
    } else {
        req = (request *) malloc(sizeof (request));
        if (!req) {
            log_error_time();
            perror("malloc for new request");
            return NULL;
        }
    }

#ifdef SERVER_SSL
	req->ssl = NULL;
#endif /*SERVER_SSL*/

    memset(req, 0, offsetof(request, buffer) + 1);
#ifdef DAVINCI_IPCAM
    req->busy_flag = BUSY_FLAG_AUDIO|BUSY_FLAG_VIDEO;
#endif

    return req;
}

request *get_sock_request(int server_s)
{
    int fd;                     /* socket */
    struct SOCKADDR remote_addr; /* address */
    struct SOCKADDR salocal;
    socklen_t remote_addrlen = sizeof (struct SOCKADDR);
    request *conn;              /* connection */
    size_t len;
    static int system_bufsize = 0; /* Default size of SNDBUF given by system */

    remote_addr.S_FAMILY = 0xdead;
    fd = accept(server_s, (struct sockaddr *) &remote_addr,
                &remote_addrlen);

    if (fd == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            /* abnormal error */
            WARN("accept");
        else
            /* no requests */
            pending_requests = 0;
        return NULL;
    }
    if (fd >= FD_SETSIZE) {
        WARN("Got fd >= FD_SETSIZE.");
        close(fd);
	return NULL;
    }
#ifdef DEBUGNONINET
    /* This shows up due to race conditions in some Linux kernels
       when the client closes the socket sometime between
       the select() and accept() syscalls.
       Code and description by Larry Doolittle <ldoolitt@boa.org>
     */
#define HEX(x) (((x)>9)?(('a'-10)+(x)):('0'+(x)))
    if (remote_addr.sin_family != AF_INET) {
        struct sockaddr *bogus = (struct sockaddr *) &remote_addr;
        char *ap, ablock[44];
        int i;
        close(fd);
        log_error_time();
        for (ap = ablock, i = 0; i < remote_addrlen && i < 14; i++) {
            *ap++ = ' ';
            *ap++ = HEX((bogus->sa_data[i] >> 4) & 0x0f);
            *ap++ = HEX(bogus->sa_data[i] & 0x0f);
        }
        *ap = '\0';
        fprintf(stderr, "non-INET connection attempt: socket %d, "
                "sa_family = %hu, sa_data[%d] = %s\n",
                fd, bogus->sa_family, remote_addrlen, ablock);
        return NULL;
    }
#endif

/* XXX Either delete this, or document why it's needed */
/* Pointed out 3-Oct-1999 by Paul Saab <paul@mu.org> */
#ifdef REUSE_EACH_CLIENT_CONNECTION_SOCKET
    if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,
                    sizeof (sock_opt))) == -1) {
        DIE("setsockopt: unable to set SO_REUSEADDR");
    }
#endif

    len = sizeof(salocal);

    if (getsockname(fd, (struct sockaddr *) &salocal, &len) != 0) {
        WARN("getsockname");
        close(fd);
        return NULL;
    }

    conn = new_request();
    if (!conn) {
        close(fd);
        return NULL;
    }
    conn->fd = fd;
    conn->status = READ_HEADER;
    conn->header_line = conn->client_stream;
    conn->time_last = current_time;
    conn->kacount = ka_max;

    ascii_sockaddr(&salocal, conn->local_ip_addr, NI_MAXHOST);

    /* nonblocking socket */
    if (set_nonblock_fd(conn->fd) == -1)
        WARN("fcntl: unable to set new socket to non-block");

    /* set close on exec to true */
    if (fcntl(conn->fd, F_SETFD, 1) == -1)
        WARN("fctnl: unable to set close-on-exec for new socket");

    /* Increase buffer size if we have to.
     * Only ask the system the buffer size on the first request,
     * and assume all subsequent sockets have the same size.
     */
    if (system_bufsize == 0) {
        len = sizeof (system_bufsize);
        if (getsockopt
            (conn->fd, SOL_SOCKET, SO_SNDBUF, &system_bufsize, &len) == 0
            && len == sizeof (system_bufsize)) {
            /*
               fprintf(stderr, "%sgetsockopt reports SNDBUF %d\n",
               get_commonlog_time(), system_bufsize);
             */
            ;
        } else {
            WARN("getsockopt(SNDBUF)");
            system_bufsize = 1;
        }
    }
    if (system_bufsize < sockbufsize) {
        if (setsockopt
            (conn->fd, SOL_SOCKET, SO_SNDBUF, (void *) &sockbufsize,
             sizeof (sockbufsize)) == -1) {
            WARN("setsockopt: unable to set socket buffer size");
#ifdef DIE_ON_ERROR_TUNING_SNDBUF
            exit(errno);
#endif
        }
    }

    /* for log file and possible use by CGI programs */
    ascii_sockaddr(&remote_addr, conn->remote_ip_addr, NI_MAXHOST);

    /* for possible use by CGI programs */
    conn->remote_port = net_port(&remote_addr);

    status.requests++;

#ifdef USE_TCPNODELAY
    /* Thanks to Jef Poskanzer <jef@acme.com> for this tweak */
    {
        int one = 1;
        if (setsockopt(conn->fd, IPPROTO_TCP, TCP_NODELAY,
                       (void *) &one, sizeof (one)) == -1) {
            DIE("setsockopt: unable to set TCP_NODELAY");
        }

    }
#endif

#ifndef NO_RATE_LIMIT
    if (conn->fd > max_connections) {
        send_r_service_unavailable(conn);
        conn->status = DONE;
        pending_requests = 0;
    }
#endif                          /* NO_RATE_LIMIT */

    total_connections++;
    enqueue(&request_ready, conn);
    return conn;
}

/*
 * Name: get_request
 *
 * Description: Polls the server socket for a request.  If one exists,
 * does some basic initialization and adds it to the ready queue;.
 */
void get_request(int server_s)
{
	get_sock_request(server_s);
}

#ifdef SERVER_SSL
void get_ssl_request(void)
{
	request *conn;

	conn = get_sock_request(server_ssl);
	if (!conn)
		return;
	conn->ssl = SSL_new (ctx);
	if(conn->ssl == NULL){
		printf("Couldn't create ssl connection stuff\n");
		return;
	}
	SSL_set_fd (conn->ssl, conn->fd);
	if(SSL_accept(conn->ssl) <= 0){
		ERR_print_errors_fp(stderr);
		return;
	}
	else{/*printf("SSL_accepted\n");*/}
}
#endif /*SERVER_SSL*/

/*
 * Name: free_request
 *
 * Description: Deallocates memory for a finished request and closes
 * down socket.
 */

static void free_request(request ** list_head_addr, request * req)
{
    int i;
    /* free_request should *never* get called by anything but
       process_requests */

    if (req->buffer_end && req->status != DEAD) {
        req->status = DONE;
        return;
    }
    /* put request on the free list */
    dequeue(list_head_addr, req); /* dequeue from ready or block list */

    if (req->logline)           /* access log */
        log_access(req);

#ifdef DAVINCI_IPCAM
    if (req->http_stream == URI_STREAM_JPEG) {
        GetAVData(AV_OP_UNLOCK_MJPEG, req->av_data.serial, NULL);
    }
    else if (req->http_stream == URI_STREAM_MJPEG) {
        if (req->serial_lock)
            GetAVData(AV_OP_UNLOCK_MJPEG, req->av_data.serial, NULL);
        if (req->audio_lock > 0)
            GetAVData(AV_OP_UNLOCK_ULAW, req->audio_lock, NULL);
    }
    else if (req->http_stream == URI_STREAM_MPEG4 || req->http_stream == URI_STREAM_AVC) {
        if (req->serial_lock > 0)
            GetAVData(AV_OP_UNLOCK_MP4, req->serial_lock, NULL);
        if (req->audio_lock > 0)
            GetAVData(AV_OP_UNLOCK_ULAW, req->audio_lock, NULL);
    }
    else if (req->http_stream == URI_STREAM_MPEG4CIF || req->http_stream == URI_STREAM_AVCCIF) {
        if (req->serial_lock > 0)
            GetAVData(AV_OP_UNLOCK_MP4_CIF, req->serial_lock, NULL);
        if (req->audio_lock > 0)
            GetAVData(AV_OP_UNLOCK_ULAW, req->audio_lock, NULL);
    }
    else {
#endif
        if (req->mmap_entry_var)
            release_mmap(req->mmap_entry_var);
        else if (req->data_mem) {
#ifdef DAVINCI_IPCAM
            if (req->mmap_ptr)
                munmap(req->mmap_ptr, req->mmap_size);
            else
#endif
                munmap(req->data_mem, req->filesize);
        }
#ifdef DAVINCI_IPCAM
    }
#endif

    if (req->data_fd)
	{
		extern  int posix_fadvise(int fd, off_t  offset, off_t len, int advice);
    	fdatasync(req->data_fd);
    	posix_fadvise(req->data_fd, 0,0, POSIX_FADV_DONTNEED );
        close(req->data_fd);
	}

    if (req->post_data_fd)
        close(req->post_data_fd);

    if (req->response_status >= 400)
        status.errors++;

    for (i = COMMON_CGI_COUNT; i < req->cgi_env_index; ++i) {
        if (req->cgi_env[i]) {
            free(req->cgi_env[i]);
        } else {
            log_error_time();
            fprintf(stderr, "Warning: CGI Environment contains NULL value" \
                    "(index %d of %d).\n", i, req->cgi_env_index);
        }
    }

#ifdef DAVINCI_IPCAM
    if (req->cmd_string)
        free(req->cmd_string);
    if (req->mem_flag & MFLAG_IS_MEMORY) {
        if (req->data_mem)
            free(req->data_mem);
    }
#endif
    if (req->pathname)
        free(req->pathname);
    if (req->path_info)
        free(req->path_info);
    if (req->path_translated)
        free(req->path_translated);
    if (req->script_name)
        free(req->script_name);

    if ((req->keepalive == KA_ACTIVE) &&
        (req->response_status < 500) && req->kacount > 0) {
        int bytes_to_move;

        request *conn = new_request();
        if (!conn) {
            /* errors already reported */
            enqueue(&request_free, req);
            close(req->fd);
            total_connections--;
#ifdef SERVER_SSL
		SSL_free(req->ssl);
#endif /*SERVER_SSL*/
            return;
        }
        conn->fd = req->fd;
        conn->status = READ_HEADER;
        conn->header_line = conn->client_stream;
        conn->kacount = req->kacount - 1;
#ifdef SERVER_SSL
        conn->ssl = req->ssl; /*MN*/
#endif /*SERVER_SSL*/

        /* close enough and we avoid a call to time(NULL) */
        conn->time_last = req->time_last;

        /* for log file and possible use by CGI programs */
        memcpy(conn->remote_ip_addr, req->remote_ip_addr, NI_MAXHOST);
        memcpy(conn->local_ip_addr, req->local_ip_addr, NI_MAXHOST);

        /* for possible use by CGI programs */
        conn->remote_port = req->remote_port;

        status.requests++;

        /* we haven't parsed beyond req->parse_pos, so... */
        bytes_to_move = req->client_stream_pos - req->parse_pos;

        if (bytes_to_move) {
            memcpy(conn->client_stream,
                   req->client_stream + req->parse_pos, bytes_to_move);
            conn->client_stream_pos = bytes_to_move;
        }
        enqueue(&request_block, conn);
        BOA_FD_SET(conn->fd, &block_read_fdset);

        enqueue(&request_free, req);
        return;
    }

    /*
     While debugging some weird errors, Jon Nelson learned that
     some versions of Netscape Navigator break the
     HTTP specification.

     Some research on the issue brought up:

     http://www.apache.org/docs/misc/known_client_problems.html

     As quoted here:

     "
     Trailing CRLF on POSTs

     This is a legacy issue. The CERN webserver required POST
     data to have an extra CRLF following it. Thus many
     clients send an extra CRLF that is not included in the
     Content-Length of the request. Apache works around this
     problem by eating any empty lines which appear before a
     request.
     "

     Boa will (for now) hack around this stupid bug in Netscape
     (and Internet Exploder)
     by reading up to 32k after the connection is all but closed.
     This should eliminate any remaining spurious crlf sent
     by the client.

     Building bugs *into* software to be compatable is
     just plain wrong
     */

    if (req->method == M_POST) {
        char buf[32768];
        read(req->fd, buf, 32768);
    }
    close(req->fd);
    total_connections--;
#ifdef SERVER_SSL
    SSL_free(req->ssl);
#endif /*SERVER_SSL*/

    enqueue(&request_free, req);

    return;
}

/*
 * Name: process_requests
 *
 * Description: Iterates through the ready queue, passing each request
 * to the appropriate handler for processing.  It monitors the
 * return value from handler functions, all of which return -1
 * to indicate a block, 0 on completion and 1 to remain on the
 * ready list for more procesing.
 */

void process_requests(int server_s)
{
    int retval = 0;
    request *current, *trailer;
	node_t *node = NULL;
#ifdef DAVINCI_IPCAM
    int busy;
#endif

#ifdef SERVER_SSL
    if(do_sock){
#endif /*SERVER_SSL*/
        if (pending_requests) {
            get_request(server_s);
#ifdef ORIGINAL_BEHAVIOR
            pending_requests = 0;
#endif
        }
#ifdef SERVER_SSL
    }
#endif /*SERVER_SSL*/

#ifdef SERVER_SSL
    if (do_sock < 2) {
        if(FD_ISSET(server_ssl, &block_read_fdset)){ /*If we have the main SSL server socket*/
/*         printf("SSL request received!!\n");*/
            get_ssl_request();
        }
    }
#endif /*SERVER_SSL*/

    current = request_ready;
#ifdef DAVINCI_IPCAM
    busy = 0;
#endif
    while (current) {
        time(&current_time);
        if (current->buffer_end && /* there is data in the buffer */
            current->status != DEAD && current->status != DONE) {
            retval = req_flush(current);
            /*
             * retval can be -2=error, -1=blocked, or bytes left
             */
            if (retval == -2) { /* error */
                current->status = DEAD;
                retval = 0;
            } else if (retval >= 0) {
                /* notice the >= which is different from below?
                   Here, we may just be flushing headers.
                   We don't want to return 0 because we are not DONE
                   or DEAD */

                retval = 1;
            }
        } else {
            switch (current->status) {
            case READ_HEADER:
            case ONE_CR:
            case ONE_LF:
            case TWO_CR:
                retval = read_header(current,node);
                break;
            case BODY_READ:
                retval = read_body(current);
                break;
            case BODY_WRITE:
                retval = write_body(current);
                break;
            case WRITE:
                retval = process_get(current);
                break;
            case PIPE_READ:
                retval = read_from_pipe(current);
                break;
            case PIPE_WRITE:
                retval = write_from_pipe(current);
                break;
            case DONE:
                /* a non-status that will terminate the request */
                retval = req_flush(current);
                /*
                 * retval can be -2=error, -1=blocked, or bytes left
                 */
                if (retval == -2) { /* error */
                    current->status = DEAD;
                    retval = 0;
                } else if (retval > 0) {
                    retval = 1;
                }
                break;
            case DEAD:
                retval = 0;
                current->buffer_end = 0;
                SQUASH_KA(current);
                break;
            default:
                retval = 0;
                fprintf(stderr, "Unknown status (%d), "
                        "closing!\n", current->status);
                current->status = DEAD;
                break;
            }

        }

        if (sigterm_flag)
            SQUASH_KA(current);

        /* we put this here instead of after the switch so that
         * if we are on the last request, and get_request is successful,
         * current->next is valid!
         */
        if (pending_requests)
            get_request(server_s);

#ifdef DAVINCI_IPCAM
        busy |= current->busy_flag;
        //current->busy_flag = BUSY_FLAG_AUDIO|BUSY_FLAG_VIDEO;
#endif

        switch (retval) {
        case -1:               /* request blocked */
            trailer = current;
            current = current->next;
            block_request(trailer);
            break;
        case 0:                /* request complete */
            current->time_last = current_time;
            trailer = current;
            current = current->next;
            free_request(&request_ready, trailer);
            break;
        case 1:                /* more to do */
            current->time_last = current_time;
            current = current->next;
            break;
        default:
            log_error_time();
            fprintf(stderr, "Unknown retval in process requests - "
                    "Status: %d, retval: %d\n", current->status, retval);
            current = current->next;
            break;
        }
    }
#ifdef DAVINCI_IPCAM
    if (!busy)
		usleep(10000);
#endif
}

/*
 * Name: process_logline
 *
 * Description: This is called with the first req->header_line received
 * by a request, called "logline" because it is logged to a file.
 * It is parsed to determine request type and method, then passed to
 * translate_uri for further parsing.  Also sets up CGI environment if
 * needed.
 */

int process_logline(request * req)
{
    char *stop, *stop2;
    static char *SIMPLE_HTTP_VERSION = "HTTP/0.9";

    req->logline = req->client_stream;
    if (!memcmp(req->logline, "GET ", 4))
        req->method = M_GET;
    else if (!memcmp(req->logline, "HEAD ", 5))
        /* head is just get w/no body */
        req->method = M_HEAD;
    else if (!memcmp(req->logline, "PUT ", 4))
        req->method = M_PUT;
	else if (!memcmp(req->logline, "POST ", 5))
        req->method = M_POST;
	else if (!memcmp(req->logline, "DELETE ", 7))
        req->method = M_DELETE;
    else {
        log_error_time();
        fprintf(stderr, "malformed request: \"%s\"\n", req->logline);
        send_r_not_implemented(req);
        return 0;
    }

    req->http_version = SIMPLE_HTTP_VERSION;
    req->simple = 1;

    /* Guaranteed to find ' ' since we matched a method above */
    if(req->method != M_DELETE)
	   stop = req->logline + 3;
	else
       stop = req->logline + 5;

    if (*stop != ' ')
        ++stop;

    /* scan to start of non-whitespace */
    while (*(++stop) == ' ');

    stop2 = stop;

    /* scan to end of non-whitespace */
    while (*stop2 != '\0' && *stop2 != ' ')
        ++stop2;

    if (stop2 - stop > MAX_HEADER_LENGTH) {
        log_error_time();
        fprintf(stderr, "URI too long %d: \"%s\"\n", MAX_HEADER_LENGTH,
                req->logline);
        send_r_bad_request(req);
        return 0;
    }
    memcpy(req->request_uri, stop, stop2 - stop);
    req->request_uri[stop2 - stop] = '\0';

    if (*stop2 == ' ') {
        /* if found, we should get an HTTP/x.x */
        unsigned int p1, p2;

        /* scan to end of whitespace */
        ++stop2;
        while (*stop2 == ' ' && *stop2 != '\0')
            ++stop2;

		/* scan in HTTP/major.minor */
        if (sscanf(stop2, "HTTP/%u.%u", &p1, &p2) == 2 ) {
            /* HTTP/{0.9,1.0,1.1} */
            if (p1 == 1 && (p2 == 0 || p2 == 1)) {
                req->http_version = stop2;
                req->simple = 0;
            } else if (p1 > 1 || (p1 != 0 && p2 > 1)) {
                goto BAD_VERSION;
            }
        } else {
            goto BAD_VERSION;
        }
    }

    if (req->method == M_HEAD && req->simple) {
        send_r_bad_request(req);
        return 0;
    }
    req->cgi_env_index = COMMON_CGI_COUNT;

    return 1;

BAD_VERSION:
    log_error_time();
    fprintf(stderr, "bogus HTTP version: \"%s\"\n", stop2);
    send_r_bad_request(req);
    return 0;
}

/*
 * Name: process_header_end
 *
 * Description: takes a request and performs some final checking before
 * init_cgi or init_get
 * Returns 0 for error or NPH, or 1 for success
 */

int process_header_end(request * req,node_t * node)
{
    if (!req->logline) {
        send_r_error(req);
        return 0;
    }
    /* Percent-decode request */
    if (unescape_uri(req->request_uri, &(req->query_string)) == 0) {
        log_error_doc(req);
        fputs("Problem unescaping uri\n", stderr);
        send_r_bad_request(req);
        return 0;
    }

#ifdef PRINT_REQ
	fprintf(stderr,"Request URI: %s, Query String: %s\n", req->request_uri, req->query_string);
#endif

    /* clean pathname */
    clean_pathname(req->request_uri);

    if (req->request_uri[0] != '/') {
        send_r_bad_request(req);
        return 0;
    }

    if (translate_uri(req) == 0) { /* unescape, parse uri */
        SQUASH_KA(req);
        return 0;               /* failure, close down */
    }

#ifdef DAVINCI_IPCAM
    req->authority = AUTHORITY_NONE;
    if (req->http_uri) {
		if((strcmp(req->request_uri,"/")!= 0) && (strcmp(req->request_uri,"/gIPNC-ro.gz")!= 0) && (strcmp(req->request_uri,"/IPNCWebDlls.CAB")!= 0) &&
			(strcmp(req->request_uri,"/sddel.htm")!= 0) && (strcmp(req->request_uri,"/sdget.htm")!= 0) && (strcmp(req->request_uri,"/sysget.htm")!= 0)
			&& (strcmp(req->request_uri,"/accessget.htm")!= 0) && (strcmp(req->request_uri,"/actvlistget.htm")!= 0) && (strcmp(req->request_uri,"/paraget.htm")!= 0)
			&& (strcmp(req->request_uri,"/paradefault.htm")!= 0)&& (strcmp(req->request_uri,"/actvlistclear.htm")!= 0) && (strcmp(req->request_uri,"/update.cgi")!= 0)){
			if (req->http_uri->authority < req->authority) {
				if (!auth_authorize(req)) {
					dbg("auth_authorize request: %s\n", req->request_uri);
					return 0;
				}
				if (req->http_uri->authority < req->authority) {
					send_r_forbidden(req);
					return 0;
				}
			}
		}
        if (req->http_uri->handler) {
            int ret = (*req->http_uri->handler)(req);
            if (ret >= 0)
               return ret;
        }
    }
#endif

#ifdef PSIA_MEDIA
     if (req->psia_uri) {                      /*PSIA Handler Initialization*/
	    int ret,content_length,bytesRead,bytes2Read;
		int bytes_to_write;
		//char inbuf[RDBFLEN];
		char *buffer;

        if(req->method == M_PUT || req->method == M_POST)
	    {
          if (req->content_length) {
            content_length = boa_atoi(req->content_length);
		  }

          buffer = &req->client_stream[req->parse_pos];

          // check if complete request is read if not read the
          // complete request
          while((req->client_stream_pos - req->parse_pos) < content_length)
          {
              usleep(1000);
              bytes2Read = content_length - (req->client_stream_pos - req->parse_pos);
              bytesRead  = read(req->fd,
                                buffer + (req->client_stream_pos - req->parse_pos),
                                bytes2Read);
              if(bytesRead > 0)
                  req->client_stream_pos += bytesRead;
		  }

	      req->header_line = &req->client_stream[req->parse_pos];
          req->header_end  = &req->client_stream[req->client_stream_pos];

		  req->filesize = content_length;
          req->filepos = 0;

		  if (req->header_end - req->header_line > req->filesize) {
             req->header_end = req->header_line + req->filesize;
          }

		  bytes_to_write = req->header_end - req->header_line;

		  if (req->filepos + bytes_to_write > req->filesize)
             bytes_to_write = req->filesize - req->filepos;

		  memcpy(&req->inbuf,&req->header_line,bytes_to_write);

          req->inlen   = (int)req->content_length;
		  req->command = req->script_name;
		}

        ret = process_psia_request(req);
#if 0
        if (req->psia_uri->authority < req->authority) {
            if (!auth_authorize(req)) {
                dbg("auth_authorize request: %s\n", req->request_uri);
                return 0;
            }
            if (req->psia_uri->authority < req->authority) {
                send_r_forbidden(req);
                return 0;
            }
        }
#endif
        if (req->psia_uri->handler) {
            ret = (*req->psia_uri->handler)(req,node);
            if (ret >= 0)
               return ret;
        }

		//if(inbuf)
		//  free(inbuf);
     }

#endif

    if (req->method == M_POST) {
        req->post_data_fd = create_temporary_file(1, NULL, 0);
        if (req->post_data_fd == 0)
            return(0);
        return(1); /* success */
    }

    if (req->is_cgi) {
        return init_cgi(req);
    }

    req->status = WRITE;
    return init_get(req);       /* get and head */
}

/*
 * Name: process_option_line
 *
 * Description: Parses the contents of req->header_line and takes
 * appropriate action.
 */

int process_option_line(request * req)
{
    char c, *value, *line = req->header_line;

    /* Start by aggressively hacking the in-place copy of the header line */

#ifdef FASCIST_LOGGING
    log_error_time();
    fprintf(stderr, "%s:%d - Parsing \"%s\"\n", __FILE__, __LINE__, line);
#endif

    value = strchr(line, ':');
    if (value == NULL)
        return 0;
    *value++ = '\0';            /* overwrite the : */
    to_upper(line);             /* header types are case-insensitive */
    while ((c = *value) && (c == ' ' || c == '\t'))
        value++;

    if (!memcmp(line, "IF_MODIFIED_SINCE", 18) && !req->if_modified_since)
        req->if_modified_since = value;

    else if (!memcmp(line, "CONTENT_TYPE", 13) && !req->content_type){
        req->content_type = value;
	    //fprintf(stderr,"\r\n Content Type: %s\n",req->content_type);
	}

    else if (!memcmp(line, "CONTENT_LENGTH", 15) && !req->content_length){
        req->content_length = value;
	    //fprintf(stderr,"\r\n Content Length: %s\n",req->content_length);
    }
#ifdef DAVINCI_IPCAM
    else if (!memcmp(line,"AUTHORIZATION",14) && !req->authorization)
        req->authorization = value;
#endif

    else if (!memcmp(line, "CONNECTION", 11) &&
             ka_max && req->keepalive != KA_STOPPED) {
        req->keepalive = (!strncasecmp(value, "Keep-Alive", 10) ?
                          KA_ACTIVE : KA_STOPPED);
    }
    /* #ifdef ACCEPT_ON */
    else if (!memcmp(line, "ACCEPT", 7))
        add_accept_header(req, value);
    /* #endif */

    /* Need agent and referer for logs */
    else if (!memcmp(line, "REFERER", 8)) {
        req->header_referer = value;
        if (!add_cgi_env(req, "REFERER", value, 1))
            return 0;
    } else if (!memcmp(line, "USER_AGENT", 11)) {
        req->header_user_agent = value;
        if (!add_cgi_env(req, "USER_AGENT", value, 1))
            return 0;
    } else {
        if (!add_cgi_env(req, line, value, 1))
            return 0;
    }
    return 1;
}

/*
 * Name: add_accept_header
 * Description: Adds a mime_type to a requests accept char buffer
 *   silently ignore any that don't fit -
 *   shouldn't happen because of relative buffer sizes
 */

void add_accept_header(request * req, char *mime_type)
{
#ifdef ACCEPT_ON
    int l = strlen(req->accept);
    int l2 = strlen(mime_type);

    if ((l + l2 + 2) >= MAX_HEADER_LENGTH)
        return;

    if (req->accept[0] == '\0')
        strcpy(req->accept, mime_type);
    else {
        req->accept[l] = ',';
        req->accept[l + 1] = ' ';
        memcpy(req->accept + l + 2, mime_type, l2 + 1);
        /* the +1 is for the '\0' */
        /*
           sprintf(req->accept + l, ", %s", mime_type);
         */
    }
#endif
}

void free_requests(void)
{
    request *ptr, *next;

    ptr = request_free;
    while (ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
    request_free = NULL;
}
