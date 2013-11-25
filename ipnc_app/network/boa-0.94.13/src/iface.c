/*
 * Device interface to retreive or update settings
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sysctrl.h>
#include <sys_env_type.h>
#include <net_config.h>
#include <web_translate.h>
#include <system_default.h>
#include <file_list.h>

#define INITIALIZE_TAGS

#include <psia.h>
#include <tags.h>
#include <iface.h>

#define ETH_NAME	"eth0"
#define MMC_PATH "/mnt/mmc/ipnc/"
#define MAX_LOG_PAGE_NUM	20
#define NUM_LOG_PER_PAGE	20

int channel_id = 0;
int textoverlay_id = 0;
int imageoverlay_id = 0;
int idflag = 0;
int userIndex = 0;
int logIndex = 0;
int xBlock = 0;
int yBlock = 0;
int zBlock = 0;
int scheduleFlag = 0;
__u8 scheduleEnable[7];

extern char *TZname[];

//extern unsigned char charToint(unsigned char *pChar);

/*******************************************************************/
/*                        Standard Response                        */
/*******************************************************************/
void add_response_status(request *req, node_t *node)
{
	char scstr[32];

	sprintf(scstr, "%d", req->status_code);
	xadd_stag_attr(req, TAG_ResponseStatus, RootAttrStr);
	xadd_elem(req, TAG_requestURL, req->request_uri);
	xadd_elem(req, TAG_statusCode, scstr);
	xadd_elem(req, TAG_statusString, StatusString[req->status_code]);
	xadd_etag(req, TAG_ResponseStatus);
}

/* return the corresponsing HTTP status code */
int get_http_status(request *req)
{
	int http_status;

	http_status = STATUS_INVALID_OPERATION;
	switch(req->status_code) {
		case STATUS_OK:
			http_status = R_REQUEST_OK;
			break;
		case STATUS_DEVICE_BUSY:
			http_status = R_SERVICE_UNAV;
			break;
		case STATUS_DEVICE_ERROR:
			http_status = R_ERROR;
			break;
		case STATUS_INVALID_OPERATION:
			http_status = R_FORBIDDEN;
			break;
		case STATUS_INVALID_XML_FORMAT:
			http_status = R_BAD_REQUEST;
			break;
		case STATUS_INVALID_XML_CONTENT:
			http_status = R_BAD_REQUEST;
			break;
		case STATUS_REBOOT_REQUIRED:
			http_status = R_REQUEST_OK;
			break;
		case STATUS_NOT_IMPLEMENTED:
			http_status = R_SERVICE_UNAV;
			break;
	}
	req->response_complete = 1;
	return http_status;
}

void retrieve_standard_resource(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	//return STATUS_OK;
}

/*******************************************************************/
/*                          Root                                   */
/*******************************************************************/
int retrieve_process_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"indexr");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"System");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Security");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Streaming");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Custom");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_indexr(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"indexr");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

   /* System */

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"System");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"reboot");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"updateFirmware");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"configurationData");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"factoryReset");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"deviceInfo");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"supportReport");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"status");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"time");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"logging");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Storage");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Network");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Video");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Audio");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);
	xadd_etag(req, TAG_Resource);

	/* Security */

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Security");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"AAA");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);
	xadd_etag(req, TAG_Resource);

	/* Streaming */

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Streaming");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
    xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"status");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"channels");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);
	xadd_etag(req, TAG_Resource);


	/* Custom */

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Custom");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Event");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"triggers");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"schedule");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"notification");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"MotionDetection");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Analytics");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"faceDetection");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"faceRecognition");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Live");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	//retrieve_standard_resource(req, node);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"channels");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"User");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Video");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"channels");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"advanced");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"channels");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);
	xadd_etag(req, TAG_ResourceList);

	xadd_etag(req, TAG_Resource);
	xadd_etag(req, TAG_ResourceList);
	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Camera");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"channels");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);
	xadd_etag(req, TAG_ResourceList);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Audio");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"channels");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);
	xadd_etag(req, TAG_ResourceList);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"DateTime");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"channels");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);
	xadd_etag(req, TAG_ResourceList);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Network");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Alarm");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"triggers");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);
	xadd_etag(req, TAG_ResourceList);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Storage");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Reload");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Maintenance");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	xadd_stag_attr(req, TAG_ResourceList, IndexAttrStr);
	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"systemLog");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"accessLog");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"activeList");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"curParamList");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"updateFilesys");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"restoreFirmware");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	//retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"PSIA");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "Root Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);

	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);

	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

/*******************************************************************/
/*                          System/Storage                         */
/*******************************************************************/
int retrieve_system_storage_volumes(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_StorageVolumeList, RootAttrStr);
	for(n = 0; n < get_storage_count(); n++)
		retrieve_system_storage_volumes_id(req, node);
	xadd_etag(req, TAG_StorageVolumeList);

	return STATUS_OK;
}

int retrieve_system_storage_volumes_id(request *req, node_t *node)
{
	StorageVolume_psia StorageVolume;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(StorageVolume.id,"0");

	if(pSysInfo->lan_config.recordlocal == 0){
	  sprintf(StorageVolume.volumeName,"SD Card");
	  sprintf(StorageVolume.volumePath,SD_PATH);
	  sprintf(StorageVolume.volumeDescription,"Storage Card");
	  sprintf(StorageVolume.volumeType,"NA");
	  sprintf(StorageVolume.storageLocation,"%d",pSysInfo->lan_config.recordlocal);
	  sprintf(StorageVolume.storageType,"External");
	  sprintf(StorageVolume.capacity,"NA");
	}else {
	  sprintf(StorageVolume.volumeName,"NAND");
	  sprintf(StorageVolume.volumePath,"/mnt/nand");
	  sprintf(StorageVolume.volumeDescription,"Internal Storage");
	  sprintf(StorageVolume.volumeType,"NA");
	  sprintf(StorageVolume.storageLocation,"%d",pSysInfo->lan_config.recordlocal);
	  sprintf(StorageVolume.storageType,"Internal");
	  sprintf(StorageVolume.capacity,"NA");
    }

	xadd_stag_attr(req, TAG_StorageVolume, RootAttrStr);
	xadd_elem(req, TAG_id, StorageVolume.id);
	xadd_elem(req, TAG_volumeName, StorageVolume.volumeName);
	xadd_elem(req, TAG_volumePath, StorageVolume.volumePath);
	xadd_elem(req, TAG_volumeDescription, StorageVolume.volumeDescription);
	xadd_elem(req, TAG_volumeType, StorageVolume.volumeType);
    xadd_elem(req, TAG_storageLocation, StorageVolume.storageLocation);
    xadd_elem(req, TAG_storageType, StorageVolume.storageType);
	xadd_elem(req, TAG_capacity, StorageVolume.capacity);
	xadd_etag(req, TAG_StorageVolume);

	return STATUS_OK;
}

int retrieve_system_storage_volumes_id_capabilities(request *req, node_t *node)
{
	const char *storageLocationStr = "opt=\"SD/MMC,USB,NAND\"";

	xadd_stag_attr(req, TAG_StorageVolume, RootAttrStr);
	xadd_stag_attr(req, TAG_storageLocation, storageLocationStr);
	xadd_etag(req, TAG_storageLocation);
	xadd_etag(req, TAG_StorageVolume);

	return STATUS_OK;
}

int retrieve_system_storage_volumes_id_status(request *req, node_t *node)
{
	StorageVolumeStatus_psia StorageVolumeStatus;
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo->sdcard_config.sdinsert == 3)
	   sprintf(StorageVolumeStatus.enabled,"1");
	else
	   sprintf(StorageVolumeStatus.enabled,"0");

	sprintf(StorageVolumeStatus.freeSpace,"NA");

	xadd_stag_attr(req, TAG_StorageVolumeStatus, RootAttrStr);
	xadd_elem(req, TAG_enabled, StorageVolumeStatus.enabled);
	xadd_elem(req, TAG_freeSpace, StorageVolumeStatus.freeSpace);
	xadd_etag(req, TAG_StorageVolumeStatus);

	return STATUS_OK;
}

int update_system_storage_volumes_id_format(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_storage_volumes_id_files(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_StorageFileList, RootAttrStr);
	for(n = 0; n < get_storage_count(); n++)
		retrieve_system_storage_volumes_id_files_id(req, node);
	xadd_etag(req, TAG_StorageFileList);

	return STATUS_OK;
}

int delete_system_storage_volumes_id_files(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_storage_volumes_id_files_id(request *req, node_t *node)
{
	StorageFile_psia StorageFile;
	//SysInfo* pSysInfo = GetSysInfo();
	int count =0;
	char str[40] = {0};
	FILE_INFO *pFilelist = NULL;

	pFilelist = Get_File_List( (char*)MMC_PATH, &count );

	sprintf(StorageFile.id,"0");
	sprintf(StorageFile.fileName,"%s",pFilelist[0].name);

	strcat(str, pFilelist[0].date);
	strcat(str, pFilelist[0].time);
	sprintf(StorageFile.fileTimeStamp,"%s",str);

	sprintf(StorageFile.fileSize,"%s",pFilelist[0].size);


	xadd_stag_attr(req, TAG_StorageFile, RootAttrStr);
	xadd_elem(req, TAG_id, StorageFile.id);
	xadd_elem(req, TAG_fileName, StorageFile.fileName);
	xadd_elem(req, TAG_fileTimeStamp, StorageFile.fileTimeStamp);
	xadd_elem(req, TAG_fileSize, StorageFile.fileSize);
	xadd_etag(req, TAG_StorageFile);

	return STATUS_OK;
}

int delete_system_storage_volumes_id_files_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_storage_volumes_id_data(request *req, node_t *node)
{
	return STATUS_OK;
}

int get_storage_count(void)
{
	return 1;
}

void get_storage_url(int id, char *idstr)
{
	/* default ID 0,1,2,3... */
	sprintf(idstr,"/%d", id);
}

/*******************************************************************/
/*                          System/Network                         */
/*******************************************************************/
int retrieve_process_system_network_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"interfaces");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_system_network_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"Network");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "Network Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);

	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);

	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

int retrieve_system_network_interfaces(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_NetworkInterfaceList, RootAttrStr);
	for(n = 0; n < get_network_count(); n++)
		retrieve_system_network_interfaces_id(req, node);
	xadd_etag(req, TAG_NetworkInterfaceList);
	return STATUS_OK;
}

int retrieve_system_network_interfaces_id(request *req, node_t *node)
{
	NetworkInterface_psia NetworkInterface;
	//SysInfo* pSysInfo = GetSysInfo();

	NET_IPV4 ip;
	ip.int32 = net_get_ifaddr(ETH_NAME);

	sprintf(NetworkInterface.id,"0");
	sprintf(NetworkInterface.ip_address, "%03d.%03d.%03d.%03d", ip.str[0], ip.str[1], ip.str[2], ip.str[3]);

	xadd_stag_attr(req, TAG_NetworkInterface, RootAttrStr);
	xadd_elem(req, TAG_id, NetworkInterface.id);
	retrieve_system_network_interfaces_id_ipaddress(req,node);
	retrieve_system_network_interfaces_id_discovery(req,node);
	//xadd_elem(req, TAG_IPAddress, NetworkInterface.ip_address);
	xadd_etag(req, TAG_NetworkInterface);

	return STATUS_OK;
}

int update_system_network_interfaces_id(request *req, node_t *node)
{
	update_system_network_interfaces_id_ipaddress(req, node);
	update_system_network_interfaces_id_discovery(req, node);

	return STATUS_OK;
}

int retrieve_system_network_interfaces_id_ipaddress(request *req, node_t *node)
{
	IPAddress_psia IPAddress;
	SysInfo* pSysInfo = GetSysInfo();

	NET_IPV4 ip,mask,gateway,dns;

	ip.int32 = net_get_ifaddr(ETH_NAME);
	mask.int32 = net_get_netmask(ETH_NAME);
	gateway.int32 = net_get_gateway();
	dns.int32 = net_get_dns();

	sprintf(IPAddress.ip_version,"v4");

	if(pSysInfo->lan_config.net.dhcp_enable == 0)
	   sprintf(IPAddress.addressing_type,"static");
	else
	   sprintf(IPAddress.addressing_type,"dynamic");

	sprintf(IPAddress.ip_address, "%03d.%03d.%03d.%03d", ip.str[0], ip.str[1], ip.str[2], ip.str[3]);
	sprintf(IPAddress.subnet_mask, "%03d.%03d.%03d.%03d", mask.str[0], mask.str[1], mask.str[2], mask.str[3]);
	sprintf(IPAddress.gateway, "%03d.%03d.%03d.%03d", gateway.str[0], gateway.str[1], gateway.str[2], gateway.str[3]);
	sprintf(IPAddress.primaryDNS, "%03d.%03d.%03d.%03d", dns.str[0], dns.str[1], dns.str[2], dns.str[3]);
    //sprintf(IPAddress.secondaryDNS,"0");

	xadd_stag_attr(req, TAG_IPAddress, RootAttrStr);
	xadd_elem(req, TAG_ipVersion, IPAddress.ip_version);
	xadd_elem(req, TAG_addressingType, IPAddress.addressing_type);
	xadd_elem(req, TAG_ipAddress, IPAddress.ip_address);
	xadd_elem(req, TAG_subnetMask, IPAddress.subnet_mask);
	xadd_stag(req, TAG_DefaultGateway);
	xadd_elem(req, TAG_ipAddress, IPAddress.gateway);
	xadd_etag(req, TAG_DefaultGateway);
	xadd_stag(req, TAG_PrimaryDNS);
	xadd_elem(req, TAG_ipAddress, IPAddress.primaryDNS);
	xadd_etag(req, TAG_PrimaryDNS);
	//xadd_stag(req, TAG_SecondaryDNS);
	//xadd_elem(req, TAG_ipAddress, IPAddress.secondaryDNS);
	//xadd_etag(req, TAG_SecondaryDNS);
	xadd_etag(req, TAG_IPAddress);

	return STATUS_OK;
}

int update_system_network_interfaces_id_ipaddress(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	IPAddress_psia IPAddress;
	struct in_addr ipaddr; //sys_ip;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_IPAddress) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_ipVersion) == 0) {
			strcpy(IPAddress.ip_version, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_addressingType) == 0) {
			strcpy(IPAddress.addressing_type, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(IPAddress.ip_address, tag->value);
			required |= xml_required_field(3);
			ipv4_str_to_num(IPAddress.ip_address, &ipaddr);
            ControlSystemData(SFIELD_SET_IP, (void *)&ipaddr.s_addr, sizeof(ipaddr.s_addr));
		}
		else if(strcmp(tag->name, TAG_subnetMask) == 0) {
			strcpy(IPAddress.subnet_mask, tag->value);
			required |= xml_required_field(4);
			ipv4_str_to_num(IPAddress.subnet_mask, &ipaddr);
			ControlSystemData(SFIELD_SET_NETMASK, (void *)&ipaddr.s_addr, sizeof(ipaddr.s_addr));
		}
		else if(strcmp(tag->name, TAG_DefaultGateway) == 0) {
            tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_ipAddress) == 0) {
			    strcpy(IPAddress.gateway, tag->value);
			    required |= xml_required_field(5);
				ipv4_str_to_num(IPAddress.gateway, &ipaddr);
			    ControlSystemData(SFIELD_SET_GATEWAY, (void *)&ipaddr.s_addr, sizeof(ipaddr.s_addr));
			  }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
			tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_PrimaryDNS) == 0) {
            tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_ipAddress) == 0) {
			    strcpy(IPAddress.primaryDNS, tag->value);
			    required |= xml_required_field(6);
				ipv4_str_to_num(IPAddress.primaryDNS, &ipaddr);
			    ControlSystemData(SFIELD_SET_DNS, (void *)&ipaddr.s_addr, sizeof(ipaddr.s_addr));
			  }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
			tag = tag->parent;
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(6))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_network_interfaces_id_discovery(request *req, node_t *node)
{
	Discovery_psia Discovery;
	SysInfo* pSysInfo = GetSysInfo();
	NET_IPV4 ip;

	ip.int32 = net_get_ifaddr(ETH_NAME);

	sprintf(Discovery.upnp_enabled,"1");
	sprintf(Discovery.zconf_enabled,"1");
	sprintf(Discovery.mcast_enabled,"%d",pSysInfo->lan_config.net.multicast_enable);
	sprintf(Discovery.ipAddress, "%03d.%03d.%03d.%03d", ip.str[0], ip.str[1], ip.str[2], ip.str[3]);
	sprintf(Discovery.portNo,"%d",pSysInfo->lan_config.net.http_port);
	sprintf(Discovery.ttl,"0");

	xadd_stag_attr(req, TAG_Discovery, RootAttrStr);
	xadd_stag(req, TAG_UPnP);
	xadd_elem(req, TAG_enabled, Discovery.upnp_enabled);
	xadd_etag(req, TAG_UPnP);
	xadd_stag(req, TAG_Zeroconf);
	xadd_elem(req, TAG_enabled, Discovery.zconf_enabled);
	xadd_etag(req, TAG_Zeroconf);
	xadd_stag(req, TAG_MulticastDiscovery);
	xadd_elem(req, TAG_enabled, Discovery.mcast_enabled);
	xadd_elem(req, TAG_ipAddress, Discovery.ipAddress);
	xadd_elem(req, TAG_portNo, Discovery.portNo);
	xadd_elem(req, TAG_ttl, Discovery.ttl);
	xadd_etag(req, TAG_MulticastDiscovery);
	xadd_etag(req, TAG_Discovery);

	return STATUS_OK;
}

int update_system_network_interfaces_id_discovery(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	Discovery_psia Discovery;
	__u8 value;
	struct in_addr ipaddr; //sys_ip;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_Discovery) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_UPnP) == 0) {
			tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_enabled) == 0) {
			    strcpy(Discovery.upnp_enabled, tag->value);
			    required |= xml_required_field(1);
			  }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
			tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_Zeroconf) == 0) {
			tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_enabled) == 0) {
			    strcpy(Discovery.zconf_enabled, tag->value);
			    required |= xml_required_field(2);
			  }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
			tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_MulticastDiscovery) == 0) {
			tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_enabled) == 0) {
			     strcpy(Discovery.mcast_enabled, tag->value);
			     required |= xml_required_field(3);
				 value = atoi(Discovery.mcast_enabled);
			     ControlSystemData(SFIELD_SET_MULTICAST, (void *)&value, sizeof(value));
			  }
			  else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			     strcpy(Discovery.ipAddress, tag->value);
			     required |= xml_required_field(4);
			     ipv4_str_to_num(Discovery.ipAddress, &ipaddr);
                 ControlSystemData(SFIELD_SET_IP, (void *)&ipaddr.s_addr, sizeof(ipaddr.s_addr));
		      }
			  else if(strcmp(tag->name, TAG_portNo) == 0) {
			    strcpy(Discovery.portNo, tag->value);
			    required |= xml_required_field(5);
				value = atoi(Discovery.portNo);
			    ControlSystemData(SFIELD_SET_HTTPPORT, (void *)&value, sizeof(value));
		      }
		      else if(strcmp(tag->name, TAG_ttl) == 0) {
			    strcpy(Discovery.ttl, tag->value);
			    required |= xml_required_field(6);
		      }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
			tag = tag->parent;
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(6))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int get_network_count(void)
{
	return 1;
}

void get_network_url(int id, char *idstr)
{
	/* default ID 0,1,2,3... */
	sprintf(idstr,"/%d", id);
}

int get_network_index(char *idstr)
{
	idstr++;  // skip the '/'
	return(atoi(idstr));
}

/*******************************************************************/
/*                          System/Video                         */
/*******************************************************************/
int retrieve_process_system_video_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"inputs");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_system_video_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"Video");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "Video Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);

	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);

	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

int retrieve_system_video_inputs(request *req, node_t *node)
{
	xadd_stag_attr(req, TAG_VideoInput, RootAttrStr);
	retrieve_system_video_channels(req, node);
	xadd_etag(req, TAG_VideoInput);

	return STATUS_OK;
}

int retrieve_system_video_channels(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_VideoInputChannelList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
	    retrieve_system_video_channels_id(req, node);
	xadd_etag(req, TAG_VideoInputChannelList);

	return STATUS_OK;
}

int retrieve_system_video_channels_id(request *req, node_t *node)
{
	VideoInputChannel_psia VideoInputChannel;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(VideoInputChannel.id,"0");
	sprintf(VideoInputChannel.inputPort,"80");

	if(pSysInfo->lan_config.net.imagesource == 0)
	   sprintf(VideoInputChannel.powerLineFrequencyMode,"60hz");
	else if(pSysInfo->lan_config.net.imagesource == 1)
	   sprintf(VideoInputChannel.powerLineFrequencyMode,"50hz");
	else if(pSysInfo->lan_config.net.imagesource == 2)
	   sprintf(VideoInputChannel.powerLineFrequencyMode,"None");

	sprintf(VideoInputChannel.whiteBalanceLevel,"0");

	if(pSysInfo->lan_config.nWhiteBalance == 0)
	   sprintf(VideoInputChannel.whiteBalanceMode,"auto");
	else if(pSysInfo->lan_config.nWhiteBalance == 1)
	   sprintf(VideoInputChannel.whiteBalanceMode,"indoor");
	else if(pSysInfo->lan_config.nWhiteBalance == 2)
	   sprintf(VideoInputChannel.whiteBalanceMode,"outdoor");

	sprintf(VideoInputChannel.exposureMode,"auto");
	sprintf(VideoInputChannel.exposureTarget,"0");
	sprintf(VideoInputChannel.exposureAutoMin,"0");

	if(pSysInfo->lan_config.maxexposure == 0)
	   sprintf(VideoInputChannel.exposureAutoMax,"0");
	   //sprintf(VideoInputChannel.exposureAutoMax,"AUTO");

	if(pSysInfo->lan_config.maxgain == 0)
	   sprintf(VideoInputChannel.gainLevel,"0");

	sprintf(VideoInputChannel.brightnessLevel,"%d",((pSysInfo->lan_config.nBrightness*100)/255));
	sprintf(VideoInputChannel.contrastLevel,"%d",((pSysInfo->lan_config.nContrast*100)/255));
	sprintf(VideoInputChannel.sharpnessLevel,"%d",((pSysInfo->lan_config.nSharpness*100)/255));
	sprintf(VideoInputChannel.saturationLevel,"%d",((pSysInfo->lan_config.nSaturation*100)/255));

	if(pSysInfo->lan_config.nDayNight == 0)
	   sprintf(VideoInputChannel.dayNightFilterType,"night");
	else if(pSysInfo->lan_config.nDayNight == 1)
	   sprintf(VideoInputChannel.dayNightFilterType,"day");

	xadd_stag_attr(req, TAG_VideoInputChannel, RootAttrStr);
	xadd_elem(req, TAG_id, VideoInputChannel.id);
	xadd_elem(req, TAG_inputPort, VideoInputChannel.inputPort);
	xadd_elem(req, TAG_powerLineFrequencyMode, VideoInputChannel.powerLineFrequencyMode);
	xadd_elem(req, TAG_whiteBalanceMode, VideoInputChannel.whiteBalanceMode);
	xadd_elem(req, TAG_whiteBalanceLevel, VideoInputChannel.whiteBalanceLevel);
	xadd_elem(req, TAG_exposureMode, VideoInputChannel.exposureMode);
	xadd_stag(req, TAG_Exposure);
	xadd_elem(req, TAG_exposureTarget, VideoInputChannel.exposureTarget);
    xadd_elem(req, TAG_exposureAutoMin, VideoInputChannel.exposureAutoMin);
    xadd_elem(req, TAG_exposureAutoMax, VideoInputChannel.exposureAutoMax);
	xadd_etag(req, TAG_Exposure);
	xadd_elem(req, TAG_gainLevel, VideoInputChannel.gainLevel);
	xadd_elem(req, TAG_brightnessLevel, VideoInputChannel.brightnessLevel);
	xadd_elem(req, TAG_contrastLevel, VideoInputChannel.contrastLevel);
	xadd_elem(req, TAG_sharpnessLevel, VideoInputChannel.sharpnessLevel);
	xadd_elem(req, TAG_saturationLevel, VideoInputChannel.saturationLevel);
	xadd_stag(req, TAG_DayNightFilter);
	xadd_elem(req, TAG_dayNightFilterType, VideoInputChannel.dayNightFilterType);
	xadd_etag(req, TAG_DayNightFilter);
	xadd_etag(req, TAG_VideoInputChannel);

	return STATUS_OK;
}

int update_system_video_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	VideoInputChannel_psia VideoInputChannel;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_VideoInputChannel) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(VideoInputChannel.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_inputPort) == 0) {
			strcpy(VideoInputChannel.inputPort, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_powerLineFrequencyMode) == 0) {
			strcpy(VideoInputChannel.powerLineFrequencyMode, tag->value);

			if(strncmp(VideoInputChannel.powerLineFrequencyMode,"60hz",3) == 0)
               strcpy(VideoInputChannel.powerLineFrequencyMode,"0");
			else if(strncmp(VideoInputChannel.powerLineFrequencyMode,"50hz",3) == 0)
               strcpy(VideoInputChannel.powerLineFrequencyMode,"1");

			required |= xml_required_field(3);
			value = atoi(VideoInputChannel.powerLineFrequencyMode);
			ControlSystemData(SFIELD_SET_IMAGESOURCE, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_whiteBalanceMode) == 0) {
			strcpy(VideoInputChannel.whiteBalanceMode, tag->value);

			if(strncmp(VideoInputChannel.whiteBalanceMode,"auto",3) == 0)
               strcpy(VideoInputChannel.whiteBalanceMode,"0");
			else if(strncmp(VideoInputChannel.whiteBalanceMode,"indoor",3) == 0)
               strcpy(VideoInputChannel.whiteBalanceMode,"1");
			else if(strncmp(VideoInputChannel.whiteBalanceMode,"outdoor",3) == 0)
               strcpy(VideoInputChannel.whiteBalanceMode,"2");

			required |= xml_required_field(4);
			value = atoi(VideoInputChannel.whiteBalanceMode);
			ControlSystemData(SFIELD_SET_WHITE_BALANCE, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_whiteBalanceLevel) == 0) {
			strcpy(VideoInputChannel.whiteBalanceLevel, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_exposureMode) == 0) {
			strcpy(VideoInputChannel.exposureMode, tag->value);
			required |= xml_required_field(6);
		}
		else if(strcmp(tag->name, TAG_Exposure) == 0) {
			tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_exposureTarget) == 0) {
			     strcpy(VideoInputChannel.exposureTarget, tag->value);
			     required |= xml_required_field(7);
			  }
			  else if(strcmp(tag->name, TAG_exposureAutoMin) == 0) {
			     strcpy(VideoInputChannel.exposureAutoMin, tag->value);
			     required |= xml_required_field(8);
		      }
			  else if(strcmp(tag->name, TAG_exposureAutoMax) == 0) {
			     strcpy(VideoInputChannel.exposureAutoMax, tag->value);
			     required |= xml_required_field(9);
				 value = atoi(VideoInputChannel.exposureAutoMax);
				 ControlSystemData(SFIELD_SET_MAXEXPOSURE, (void *)&value, sizeof(value));
		      }
			 if(!tag->next)
				  break;
			 tag = tag->next;
			}
		   tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_gainLevel) == 0) {
			strcpy(VideoInputChannel.gainLevel, tag->value);
			required |= xml_required_field(10);
			value = atoi(VideoInputChannel.gainLevel);
			ControlSystemData(SFIELD_SET_MAXGAIN, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_brightnessLevel) == 0) {
			strcpy(VideoInputChannel.brightnessLevel, tag->value);
			required |= xml_required_field(11);
			value = atoi(VideoInputChannel.brightnessLevel);
			value = ((value * 255) / 100) + 1;
			ControlSystemData(SFIELD_SET_BRIGHTNESS, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_contrastLevel) == 0) {
			strcpy(VideoInputChannel.contrastLevel, tag->value);
			required |= xml_required_field(12);
			value = atoi(VideoInputChannel.contrastLevel);
			value = ((value * 255) / 100) + 1;
			ControlSystemData(SFIELD_SET_CONTRAST, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_sharpnessLevel) == 0) {
			strcpy(VideoInputChannel.sharpnessLevel, tag->value);
			required |= xml_required_field(13);
			value = atoi(VideoInputChannel.sharpnessLevel);
			value = ((value * 255) / 100) + 1;
			ControlSystemData(SFIELD_SET_SHARPNESS, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_saturationLevel) == 0) {
			strcpy(VideoInputChannel.saturationLevel, tag->value);
			required |= xml_required_field(14);
			value = atoi(VideoInputChannel.saturationLevel);
			value = ((value * 255) / 100) + 1;
			ControlSystemData(SFIELD_SET_SATURATION, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_DayNightFilter) == 0) {
			tag = tag->first_child;
			while(tag) {
			   if(strcmp(tag->name, TAG_dayNightFilterType) == 0) {
			     strcpy(VideoInputChannel.dayNightFilterType, tag->value);

			   if(strncmp(VideoInputChannel.dayNightFilterType,"night",3) == 0)
                  strcpy(VideoInputChannel.dayNightFilterType,"0");
			   else if(strncmp(VideoInputChannel.dayNightFilterType,"day",3) == 0)
                  strcpy(VideoInputChannel.dayNightFilterType,"1");

			   required |= xml_required_field(15);
			   value = atoi(VideoInputChannel.dayNightFilterType);
			   ControlSystemData(SFIELD_SET_DAY_NIGHT, (void *)&value, sizeof(value));
			  }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
		   tag = tag->parent;
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(15))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_video_channels_id_capabilities(request *req, node_t *node)
{
	const char *whiteBalanceModeStr = "opt=\"auto,indoor,outdoor\"";
	const char *powerLineFrequencyModeStr = "opt=\"60hz,50hz\"";
	const char *exposureAutoMaxStr = "opt=\"AUTO\"";
	const char *gainLevelStr = "opt=\"AUTO\"";

	xadd_stag_attr(req, TAG_VideoInputChannel, RootAttrStr);
	xadd_stag_attr(req, TAG_powerLineFrequencyMode, powerLineFrequencyModeStr);
	xadd_etag(req, TAG_powerLineFrequencyMode);
	xadd_stag_attr(req, TAG_whiteBalanceMode, whiteBalanceModeStr);
	xadd_etag(req, TAG_whiteBalanceMode);

	xadd_stag(req, TAG_Exposure);
	xadd_stag_attr(req, TAG_exposureAutoMax, exposureAutoMaxStr);
	xadd_etag(req, TAG_exposureAutoMax);
	xadd_etag(req, TAG_Exposure);

	xadd_stag_attr(req, TAG_gainLevel, gainLevelStr);
	xadd_etag(req, TAG_gainLevel);
	xadd_etag(req, TAG_VideoInputChannel);

	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_VideoOverlay, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++){
		retrieve_system_video_channels_id_overlays_text(req, node);
	    retrieve_system_video_channels_id_overlays_image(req, node);
	}
	xadd_etag(req, TAG_VideoOverlay);

	return STATUS_OK;
}

int update_system_video_channels_id_overlays(request *req, node_t *node)
{
	int n;

	for(n = 0; n < get_channel_count(); n++){
		update_system_video_channels_id_overlays_text(req, node);
	    update_system_video_channels_id_overlays_image(req, node);
	}
	return STATUS_OK;
}

int delete_system_video_channels_id_overlays(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_video_channels_id_privacymask(request *req, node_t *node)
{
	PrivacyMask_psia PrivacyMask;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(PrivacyMask.enabled,"%d",pSysInfo->face_config.pmask);

	xadd_stag_attr(req, TAG_PrivacyMask, RootAttrStr);
	xadd_elem(req, TAG_enabled, PrivacyMask.enabled);
	xadd_etag(req, TAG_PrivacyMask);

	return STATUS_OK;
}

int update_system_video_channels_id_privacymask(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	PrivacyMask_psia PrivacyMask;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_PrivacyMask) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(PrivacyMask.enabled, tag->value);
			required |= xml_required_field(1);
			value = atoi(PrivacyMask.enabled);
			ControlSystemData(SFIELD_SET_PRIVACY_MASK, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(1))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays_text(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	int c,channel_count = 0;
	//char idstr[64];

	xadd_stag_attr(req, TAG_TextOverlayList, RootAttrStr);

	if(pSysInfo->lan_config.nStreamType == 0)
		channel_count = 1;
	else if (pSysInfo->lan_config.nStreamType == 1)
	    channel_count = 2;
	else if (pSysInfo->lan_config.nStreamType == 2)
	    channel_count = 3;

	for(c = 0; c < channel_count; c++) {
		retrieve_system_video_channels_id_overlays_text_id(req, node);
		textoverlay_id ++;
	}
	xadd_etag(req, TAG_TextOverlayList);
	textoverlay_id = 0;

	return STATUS_OK;
}

int update_system_video_channels_id_overlays_text(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_TextOverlayList) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_TextOverlay) == 0) {
			idflag = 1;
			update_system_video_channels_id_overlays_text_id(req, node, tag);
		    required |= xml_required_field(1);
			channel_id++;
		}
	 /* Protocol */
		tag = tag->next;
	}
	channel_id = 0;
	idflag = 0;
	/* check all the required fileds were present */
	//if(required != xml_required_mask(1))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int create_system_video_channels_id_overlays_text(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int delete_system_video_channels_id_overlays_text(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_video_channels_id_overlays_image(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	int c,channel_count = 0;
	//char idstr[64];

	xadd_stag_attr(req, TAG_ImageOverlayList, RootAttrStr);

	if(pSysInfo->lan_config.nStreamType == 0)
		channel_count = 1;
	else if (pSysInfo->lan_config.nStreamType == 1)
	    channel_count = 2;
	else if (pSysInfo->lan_config.nStreamType == 2)
	    channel_count = 3;

	for(c = 0; c < channel_count; c++) {
		retrieve_system_video_channels_id_overlays_image_id(req, node);
		imageoverlay_id ++;
	}
	xadd_etag(req, TAG_ImageOverlayList);
	imageoverlay_id = 0;

	return STATUS_OK;
}

int update_system_video_channels_id_overlays_image(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_ImageOverlayList) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_ImageOverlay) == 0) {
			idflag = 1;
			update_system_video_channels_id_overlays_image_id(req, node, tag);
		    required |= xml_required_field(1);
			channel_id++;
		}
	 /* Protocol */
		tag = tag->next;
	}
	channel_id = 0;
	idflag = 0;
	/* check all the required fileds were present */
	//if(required != xml_required_mask(1))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_system_video_channels_id_overlays_image(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_video_channels_id_overlays_text_id(request *req, node_t *node)
{
	TextOverlay_psia TextOverlay;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(TextOverlay.videoInputID,"NA");
	sprintf(TextOverlay.backgroundColor,"NA");
	sprintf(TextOverlay.fontColor,"NA");
	sprintf(TextOverlay.fontSize,"NA");

	if(textoverlay_id == 0){
	  if(pSysInfo->lan_config.nStreamType == 0){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(TextOverlay.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(TextOverlay.id,"1");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(TextOverlay.id,"0");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 1){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(TextOverlay.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(TextOverlay.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(TextOverlay.id,"2");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 3){
	       sprintf(TextOverlay.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 4){
	       sprintf(TextOverlay.id,"2");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 2){
	      if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(TextOverlay.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(TextOverlay.id,"1");
	     }
	  }

	  sprintf(TextOverlay.enabled,"%d",pSysInfo->osd_config[0].nTextEnable);
	  sprintf(TextOverlay.displayText,"%s",pSysInfo->osd_config[0].overlaytext);
	  sprintf(TextOverlay.timeStampEnabled,"%d",pSysInfo->osd_config[0].tstampenable);

	  if(pSysInfo->osd_config[0].nTextPosition == 0){
	    sprintf(TextOverlay.horizontalAlignType,"left");
	    sprintf(TextOverlay.verticalAlignType,"top");
	  }
	  else if(pSysInfo->osd_config[0].nTextPosition == 1){
        sprintf(TextOverlay.horizontalAlignType,"right");
	    sprintf(TextOverlay.verticalAlignType,"top");
	  }
	}
	else if(textoverlay_id == 1){
	  if(pSysInfo->lan_config.nStreamType == 1){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(TextOverlay.id,"0");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(TextOverlay.id,"0");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(TextOverlay.id,"2");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 3){
	       sprintf(TextOverlay.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 4){
	       sprintf(TextOverlay.id,"1");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 2){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(TextOverlay.id,"0");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(TextOverlay.id,"0");
	     }
	  }

	  sprintf(TextOverlay.enabled,"%d",pSysInfo->osd_config[1].nTextEnable);
	  sprintf(TextOverlay.displayText,"%s",pSysInfo->osd_config[1].overlaytext);
	  sprintf(TextOverlay.timeStampEnabled,"%d",pSysInfo->osd_config[1].tstampenable);

	  if(pSysInfo->osd_config[1].nTextPosition == 0){
	    sprintf(TextOverlay.horizontalAlignType,"left");
	    sprintf(TextOverlay.verticalAlignType,"top");
	  }
	  else if(pSysInfo->osd_config[1].nTextPosition == 1){
        sprintf(TextOverlay.horizontalAlignType,"right");
	    sprintf(TextOverlay.verticalAlignType,"top");
	  }
	}
	else if(textoverlay_id == 2){
	  if(pSysInfo->lan_config.nVideocombo == 0){
	     sprintf(TextOverlay.id,"2");
	  }
	  else if(pSysInfo->lan_config.nVideocombo == 1){
	     sprintf(TextOverlay.id,"1");
	  }

	  sprintf(TextOverlay.enabled,"%d",pSysInfo->osd_config[2].nTextEnable);
	  sprintf(TextOverlay.displayText,"%s",pSysInfo->osd_config[2].overlaytext);
	  sprintf(TextOverlay.timeStampEnabled,"%d",pSysInfo->osd_config[2].tstampenable);

	  if(pSysInfo->osd_config[2].nTextPosition == 0){
	    sprintf(TextOverlay.horizontalAlignType,"left");
	    sprintf(TextOverlay.verticalAlignType,"top");
	  }
	  else if(pSysInfo->osd_config[2].nTextPosition == 1){
        sprintf(TextOverlay.horizontalAlignType,"right");
	    sprintf(TextOverlay.verticalAlignType,"top");
	  }
	}

	xadd_stag_attr(req, TAG_TextOverlay, RootAttrStr);
	xadd_elem(req, TAG_id, TextOverlay.id);
	xadd_elem(req, TAG_enabled, TextOverlay.enabled);
	xadd_elem(req, TAG_videoInputID, TextOverlay.videoInputID);
	xadd_elem(req, TAG_timeStampEnabled, TextOverlay.timeStampEnabled);
    xadd_elem(req, TAG_backgroundColor, TextOverlay.backgroundColor);
    xadd_elem(req, TAG_fontColor, TextOverlay.fontColor);
	xadd_elem(req, TAG_fontSize, TextOverlay.fontSize);
	xadd_elem(req, TAG_displayText, TextOverlay.displayText);
	xadd_elem(req, TAG_horizontalAlignType, TextOverlay.horizontalAlignType);
	xadd_elem(req, TAG_verticalAlignType, TextOverlay.verticalAlignType);
	xadd_etag(req, TAG_TextOverlay);

	return STATUS_OK;
}

int update_system_video_channels_id_overlays_text_id(request *req, node_t *node, tag_t *parentTag)
{
	tag_t	*tag = parentTag;
	unsigned long required;
	TextOverlay_psia TextOverlay;
	__u8 value;

	fprintf(stderr,"\r\nUpdating Streaming Channels...\n");

	if(idflag == 0){
	  // returns bytes processed or -1
	  if(xml_validate(req, req->inbuf, req->filesize) < 0)
		  return STATUS_INVALID_XML_FORMAT;

	  tag = req->root_tag;
	  if(!((tag) && (strcmp(tag->name, TAG_TextOverlay) == 0)))
		  return STATUS_INVALID_XML_CONTENT;
	}

	fprintf(stderr,"\r\nInside update_system_video_channels_id_overlays_text_id with tag:%s\n",tag->name);
	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(TextOverlay.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(TextOverlay.enabled, tag->value);
			required |= xml_required_field(2);
			value = atoi(TextOverlay.enabled);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_TEXTENABLE1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_TEXTENABLE2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_TEXTENABLE3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_videoInputID) == 0) {
			strcpy(TextOverlay.videoInputID, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_timeStampEnabled) == 0) {
			strcpy(TextOverlay.timeStampEnabled, tag->value);
			required |= xml_required_field(4);
			value = atoi(TextOverlay.timeStampEnabled);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_TSTAMPENABLE1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_TSTAMPENABLE2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_TSTAMPENABLE3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_backgroundColor) == 0) {
			strcpy(TextOverlay.backgroundColor, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_fontColor) == 0) {
			strcpy(TextOverlay.fontColor, tag->value);
			required |= xml_required_field(6);
		}
		else if(strcmp(tag->name, TAG_fontSize) == 0) {
			strcpy(TextOverlay.fontSize, tag->value);
			required |= xml_required_field(7);
		}
		else if(strcmp(tag->name, TAG_displayText) == 0) {
			strcpy(TextOverlay.displayText, tag->value);
			required |= xml_required_field(8);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_OVERLAY_TEXT1, (void *)TextOverlay.displayText, strlen(TextOverlay.displayText));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_OVERLAY_TEXT2, (void *)TextOverlay.displayText, strlen(TextOverlay.displayText));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_OVERLAY_TEXT3, (void *)TextOverlay.displayText, strlen(TextOverlay.displayText));
		}
		else if(strcmp(tag->name, TAG_horizontalAlignType) == 0) {
			strcpy(TextOverlay.horizontalAlignType, tag->value);

			if(strncmp(TextOverlay.horizontalAlignType,"left",4) == 0)
                strcpy(TextOverlay.horizontalAlignType,"0");
			else if(strncmp(TextOverlay.horizontalAlignType,"right",4) == 0)
                strcpy(TextOverlay.horizontalAlignType,"1");

			required |= xml_required_field(9);
			value = atoi(TextOverlay.horizontalAlignType);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_TEXT_POSITION1, (void *)&value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_TEXT_POSITION2, (void *)&value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_TEXT_POSITION3, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_verticalAlignType) == 0) {
			strcpy(TextOverlay.verticalAlignType, tag->value);
			required |= xml_required_field(10);
		/*
			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_TEXT_POSITION1, &TextOverlay.verticalAlignType, sizeof(TextOverlay.verticalAlignType));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_TEXT_POSITION2, &TextOverlay.verticalAlignType, sizeof(TextOverlay.verticalAlignType));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_TEXT_POSITION3, &TextOverlay.verticalAlignType, sizeof(TextOverlay.verticalAlignType));
	    */
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(10))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_system_video_channels_id_overlays_text_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_video_channels_id_overlays_image_id(request *req, node_t *node)
{
	ImageOverlay_psia ImageOverlay;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(ImageOverlay.videoInputID,"NA");
	sprintf(ImageOverlay.imageName,"Logo");
	sprintf(ImageOverlay.transparentColorEnabled,"NA");
	sprintf(ImageOverlay.transparentColor,"NA");

	if(imageoverlay_id == 0){
	  if(pSysInfo->lan_config.nStreamType == 0){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(ImageOverlay.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(ImageOverlay.id,"1");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(ImageOverlay.id,"0");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 1){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(ImageOverlay.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(ImageOverlay.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(ImageOverlay.id,"2");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 3){
	       sprintf(ImageOverlay.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 4){
	       sprintf(ImageOverlay.id,"2");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 2){
	      if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(ImageOverlay.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(ImageOverlay.id,"1");
	     }
	  }

      sprintf(ImageOverlay.enabled,"%d",pSysInfo->osd_config[0].nLogoEnable);

	  if(pSysInfo->osd_config[0].nLogoPosition == 0){
	    sprintf(ImageOverlay.positionX,"left");
	    sprintf(ImageOverlay.positionY,"top");
	  }
	  else if(pSysInfo->osd_config[0].nLogoPosition == 1){
        sprintf(ImageOverlay.positionX,"right");
	    sprintf(ImageOverlay.positionY,"top");
	  }
	}
	else if(imageoverlay_id == 1){
	  if(pSysInfo->lan_config.nStreamType == 1){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(ImageOverlay.id,"0");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(ImageOverlay.id,"0");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(ImageOverlay.id,"2");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 3){
	       sprintf(ImageOverlay.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 4){
	       sprintf(ImageOverlay.id,"1");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 2){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(ImageOverlay.id,"0");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(ImageOverlay.id,"0");
	     }
	  }

	  sprintf(ImageOverlay.enabled,"%d",pSysInfo->osd_config[1].nLogoEnable);

	  if(pSysInfo->osd_config[1].nLogoPosition == 0){
	    sprintf(ImageOverlay.positionX,"left");
	    sprintf(ImageOverlay.positionY,"top");
	  }
	  else if(pSysInfo->osd_config[1].nLogoPosition == 1){
        sprintf(ImageOverlay.positionX,"right");
	    sprintf(ImageOverlay.positionY,"top");
	  }
	}
	else if(imageoverlay_id == 2){
	  if(pSysInfo->lan_config.nVideocombo == 0){
	     sprintf(ImageOverlay.id,"2");
	  }
	  else if(pSysInfo->lan_config.nVideocombo == 1){
	     sprintf(ImageOverlay.id,"1");
	  }

	  sprintf(ImageOverlay.enabled,"%d",pSysInfo->osd_config[2].nLogoEnable);

	  if(pSysInfo->osd_config[2].nLogoPosition == 0){
	    sprintf(ImageOverlay.positionX,"left");
	    sprintf(ImageOverlay.positionY,"top");
	  }
	  else if(pSysInfo->osd_config[2].nLogoPosition == 1){
        sprintf(ImageOverlay.positionX,"right");
	    sprintf(ImageOverlay.positionY,"top");
	  }
	}

	xadd_stag_attr(req, TAG_ImageOverlay, RootAttrStr);
	xadd_elem(req, TAG_id, ImageOverlay.id);
	xadd_elem(req, TAG_enabled, ImageOverlay.enabled);
	xadd_elem(req, TAG_videoInputID, ImageOverlay.videoInputID);
	xadd_elem(req, TAG_imageName, ImageOverlay.imageName);
	xadd_elem(req, TAG_positionX, ImageOverlay.positionX);
    xadd_elem(req, TAG_positionY, ImageOverlay.positionY);
    xadd_elem(req, TAG_transparentColorEnabled, ImageOverlay.transparentColorEnabled);
	xadd_elem(req, TAG_transparentColor, ImageOverlay.transparentColor);
	xadd_etag(req, TAG_ImageOverlay);

	return STATUS_OK;
}

int update_system_video_channels_id_overlays_image_id(request *req, node_t *node, tag_t *parentTag)
{
	tag_t	*tag = parentTag;
	unsigned long required;
	ImageOverlay_psia ImageOverlay;
	__u8 value;

	fprintf(stderr,"\r\nUpdating Streaming Channels...\n");

	if(idflag == 0){
	  // returns bytes processed or -1
	  if(xml_validate(req, req->inbuf, req->filesize) < 0)
		  return STATUS_INVALID_XML_FORMAT;

	  tag = req->root_tag;
	  if(!((tag) && (strcmp(tag->name, TAG_ImageOverlay) == 0)))
		  return STATUS_INVALID_XML_CONTENT;
	}

	fprintf(stderr,"\r\nInside update_system_video_channels_id_overlays_image_id with tag:%s\n",tag->name);
	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(ImageOverlay.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(ImageOverlay.enabled, tag->value);
			required |= xml_required_field(2);
			value = atoi(ImageOverlay.enabled);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_LOGOENABLE1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_LOGOENABLE2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_LOGOENABLE3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_videoInputID) == 0) {
			strcpy(ImageOverlay.videoInputID, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_imageName) == 0) {
			strcpy(ImageOverlay.imageName, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_positionX) == 0) {
			strcpy(ImageOverlay.positionX, tag->value);

			if(strncmp(ImageOverlay.positionX,"left",4) == 0)
                strcpy(ImageOverlay.positionX,"0");
			else if(strncmp(ImageOverlay.positionX,"right",4) == 0)
                strcpy(ImageOverlay.positionX,"1");

			required |= xml_required_field(5);
			value = atoi(ImageOverlay.positionX);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_LOGO_POSITION1, (void *)&value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_LOGO_POSITION2, (void *)&value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_LOGO_POSITION3, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_positionY) == 0) {
			strcpy(ImageOverlay.positionY, tag->value);
			required |= xml_required_field(6);
		/*
			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_LOGO_POSITION1, &ImageOverlay.positionY, sizeof(ImageOverlay.positionY));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_LOGO_POSITION2, &ImageOverlay.positionY, sizeof(ImageOverlay.positionY));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_LOGO_POSITION3, &ImageOverlay.positionY, sizeof(ImageOverlay.positionY));
         */
		}
		else if(strcmp(tag->name, TAG_transparentColorEnabled) == 0) {
			strcpy(ImageOverlay.transparentColorEnabled, tag->value);
			required |= xml_required_field(7);
		}
		else if(strcmp(tag->name, TAG_transparentColor) == 0) {
			strcpy(ImageOverlay.transparentColor, tag->value);
			required |= xml_required_field(8);
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(8))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_system_video_channels_id_overlays_image_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_video_channels_id_privacymask_capabilities(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays_text_id_capabilities(request *req, node_t *node)
{
	const char *horizontalAlignTypeStr = "opt=\"top-left,top-right\"";

	xadd_stag_attr(req, TAG_TextOverlay, RootAttrStr);
   	xadd_stag_attr(req, TAG_horizontalAlignType, horizontalAlignTypeStr);
	xadd_etag(req, TAG_horizontalAlignType);
	xadd_etag(req, TAG_TextOverlay);

	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays_image_id_capabilities(request *req, node_t *node)
{
	const char *positionXStr = "opt=\"top-left,top-right\"";

	xadd_stag_attr(req, TAG_ImageOverlay, RootAttrStr);
	xadd_stag_attr(req, TAG_positionX, positionXStr);
	xadd_etag(req, TAG_positionX);
    xadd_etag(req, TAG_ImageOverlay);

	return STATUS_OK;
}


/*******************************************************************/
/*                          System/Audio                         */
/*******************************************************************/

int retrieve_system_audio_channels(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_AudioChannelList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
		retrieve_system_audio_channels_id(req, node);
	xadd_etag(req, TAG_AudioChannelList);

	return STATUS_OK;
}

int retrieve_system_audio_channels_id(request *req, node_t *node)
{
	AudioChannel_psia AudioChannel;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(AudioChannel.id,"0");

	if(pSysInfo->audio_config.audiomode == 0){
	   sprintf(AudioChannel.audioMode,"talkonly");
	   sprintf(AudioChannel.microphoneEnabled,"1");
	   sprintf(AudioChannel.speakerEnabled,"0");
	}
	else if(pSysInfo->audio_config.audiomode == 1){
	   sprintf(AudioChannel.audioMode,"listenonly");
	   sprintf(AudioChannel.microphoneEnabled,"0");
	   sprintf(AudioChannel.speakerEnabled,"1");
	}
	else if(pSysInfo->audio_config.audiomode == 2){
	   sprintf(AudioChannel.audioMode,"talkandlisten");
	   sprintf(AudioChannel.microphoneEnabled,"1");
	   sprintf(AudioChannel.speakerEnabled,"1");
    }
	sprintf(AudioChannel.microphoneSource,"internal");
	sprintf(AudioChannel.microphoneVolume,"%d",pSysInfo->audio_config.audiooutvolume);
	sprintf(AudioChannel.speakerVolume,"%d",pSysInfo->audio_config.audiooutvolume);
	sprintf(AudioChannel.enabled,"%d",pSysInfo->lan_config.nAlarmAudioPlay);

	xadd_stag_attr(req, TAG_AudioChannel, RootAttrStr);
	xadd_elem(req, TAG_id, AudioChannel.id);
	xadd_elem(req, TAG_enabled, AudioChannel.enabled);
	xadd_elem(req, TAG_audioMode, AudioChannel.audioMode);
	xadd_elem(req, TAG_microphoneEnabled, AudioChannel.microphoneEnabled);
	xadd_elem(req, TAG_microphoneSource, AudioChannel.microphoneSource);
	xadd_elem(req, TAG_microphoneVolume, AudioChannel.microphoneVolume);
	xadd_elem(req, TAG_speakerEnabled, AudioChannel.speakerEnabled);
	xadd_elem(req, TAG_speakerVolume, AudioChannel.speakerVolume);
	xadd_etag(req, TAG_AudioChannel);

	return STATUS_OK;
}

int update_system_audio_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	AudioChannel_psia AudioChannel;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_AudioChannel) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(AudioChannel.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(AudioChannel.enabled, tag->value);
			required |= xml_required_field(2);
			value = atoi(AudioChannel.enabled);
			ControlSystemData(SFIELD_SET_ALARM_AUDIOPLAY, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_audioMode) == 0) {
			strcpy(AudioChannel.audioMode, tag->value);

			if(strncmp(AudioChannel.audioMode,"Only Mic",8) == 0)
                strcpy(AudioChannel.audioMode,"0");
			else if(strncmp(AudioChannel.audioMode,"Only Speaker",8) == 0)
                strcpy(AudioChannel.audioMode,"1");
			else if(strncmp(AudioChannel.audioMode,"Both",4) == 0)
                strcpy(AudioChannel.audioMode,"2");

			required |= xml_required_field(3);
			value = atoi(AudioChannel.audioMode);
			ControlSystemData(SFIELD_SET_AUDIOMODE, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_microphoneEnabled) == 0) {
			strcpy(AudioChannel.microphoneEnabled, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_microphoneSource) == 0) {
			strcpy(AudioChannel.microphoneSource, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_microphoneVolume) == 0) {
			strcpy(AudioChannel.microphoneVolume, tag->value);
			required |= xml_required_field(6);
			value = atoi(AudioChannel.microphoneVolume);
			ControlSystemData(SFIELD_SET_AUDIOOUTVOLUME, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_speakerEnabled) == 0) {
			strcpy(AudioChannel.speakerEnabled, tag->value);
			required |= xml_required_field(7);
		}
		else if(strcmp(tag->name, TAG_speakerVolume) == 0) {
			strcpy(AudioChannel.speakerVolume, tag->value);
			required |= xml_required_field(8);
			value = atoi(AudioChannel.speakerVolume);
			ControlSystemData(SFIELD_SET_AUDIOOUTVOLUME, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fields were present */
	//if(required != xml_required_mask(8))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_audio_channels_id_capabilities(request *req, node_t *node)
{
	const char *audioModeStr = "opt=\"Only Mic,Only Speaker,Both\"";

	xadd_stag_attr(req, TAG_AudioChannel, RootAttrStr);
	xadd_stag_attr(req, TAG_audioMode, audioModeStr);
	xadd_etag(req, TAG_audioMode);
	xadd_etag(req, TAG_AudioChannel);

	return STATUS_OK;
}

/*******************************************************************/
/*                              System                             */
/*******************************************************************/
int retrieve_process_system_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"service");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"reboot");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"updateFirmware");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"configurationData");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"factoryReset");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"deviceInfo");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"supportReport");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"status");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"time");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"logging");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_system_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"System");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "System Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);

	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);

	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

int do_system_reboot(request *req, node_t *node)
{
	return STATUS_REBOOT_REQUIRED;
}

void reboot_ipcam(void)
{
	char cmd[256];

	//int dev_fp;
	//void *pMem_map;
	//unsigned long phyAddr= 0x01C41000;
	//unsigned long length = 0xAE0;
	//unsigned int *pPSCCtrl = NULL;
	//unsigned int *pPTCMD = NULL;

    // AYK - 11032010
    // System hangs during register settings
    // Commenting it out

#if 0
	if ((dev_fp=open("/dev/mem",O_RDWR|O_SYNC))==-1)
	{
		printf("dev_fp Fail!! \n");
		return ;
	}

	pMem_map=mmap(	(void	*)phyAddr,length/*0x10000000*/,
					PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,
					dev_fp,phyAddr);

	if (pMem_map!=(void *)phyAddr)
	{
		printf("pMem_map Fail!! \n");
		return ;
	}


	pPTCMD 		= (unsigned int *)( (char *)pMem_map+0x120);
	pPSCCtrl 	= (unsigned int *)( (char *)pMem_map+0xA00);

	/*EDMA */
	*(pPSCCtrl+0) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+1) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+2) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+3) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+4) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*VPSS */
	*(pPSCCtrl+47) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*MJCP */
	*(pPSCCtrl+50) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*HDVICP */
	*(pPSCCtrl+51) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);

	if( pMem_map )
		munmap(pMem_map, length);

	if( dev_fp >= 0)
		close(dev_fp);
#endif

	sprintf(cmd, "shutdown -r now\n");
	system(cmd);

	return;
}

int do_system_updatefirmware(request *req, node_t *node)
{
	/* update firmware of inlen bytes starting at inbuf */
	return STATUS_REBOOT_REQUIRED;
	//return STATUS_OK;
}

int do_custom_maint_update_filesys(request *req, node_t *node)
{
	return STATUS_REBOOT_REQUIRED;
}

int do_custom_maint_restore_firmware(request *req, node_t *node)
{
	return STATUS_REBOOT_REQUIRED;
}

int retrieve_system_configurationdata(request *req, node_t *node)
{
	return STATUS_OK;
}

int update_system_configurationdata(request *req, node_t *node)
{
	return STATUS_OK;
}

int do_system_factory_reset(request *req, node_t *node)
{
	__u8 value;
	char defaultFlag[10];

    sprintf(defaultFlag,"1");
	value = atoi(defaultFlag);

	ControlSystemData(SFIELD_SET_DEFAULT, &value, sizeof(value));

	return STATUS_OK;
}

int retrieve_system_deviceinfo(request *req, node_t *node)
{
	DeviceInfo_psia DeviceInfo;
	SysInfo* pSysInfo = GetSysInfo();
	unsigned char mac[6];

	sprintf(DeviceInfo.device_name,"%s",pSysInfo->lan_config.title);
	sprintf(DeviceInfo.model,"%s",pSysInfo->lan_config.modelname);
	if(pSysInfo->lan_config.chipConfig)
		sprintf(DeviceInfo.firmware_version,"%s", DM368_APP_VERSION);
	else
		sprintf(DeviceInfo.firmware_version,"%s", DM365_APP_VERSION);

	net_get_hwaddr(ETH_NAME, mac);
	sprintf(DeviceInfo.serialNumber,"%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	sprintf(DeviceInfo.mac_address,"%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	sprintf(DeviceInfo.device_id,"1419d68a-1dd2-11b2-a105-%02X%02X%02X%02X%02X%02X",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	xadd_stag_attr(req, TAG_DeviceInfo, RootAttrStr);
	xadd_elem(req, TAG_deviceName, DeviceInfo.device_name);
	xadd_elem(req, TAG_deviceID, DeviceInfo.device_id);
	xadd_elem(req, TAG_model, DeviceInfo.model);
	xadd_elem(req, TAG_serialNumber, DeviceInfo.serialNumber);
	xadd_elem(req, TAG_macAddress, DeviceInfo.mac_address);
	xadd_elem(req, TAG_firmwareVersion, DeviceInfo.firmware_version);
	xadd_etag(req, TAG_DeviceInfo);

	return STATUS_OK;
}

int update_system_deviceinfo(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	DeviceInfo_psia DeviceInfo;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_DeviceInfo) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_deviceName) == 0) {
			strcpy(DeviceInfo.device_name, tag->value);
			required |= xml_required_field(1);
			ControlSystemData(SFIELD_SET_TITLE, (void *)DeviceInfo.device_name, strlen(DeviceInfo.device_name));
		}
		else if(strcmp(tag->name, TAG_deviceID) == 0) {
			strcpy(DeviceInfo.device_id, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_model) == 0) {
			strcpy(DeviceInfo.model, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_serialNumber) == 0) {
			strcpy(DeviceInfo.serialNumber, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_macAddress) == 0) {
			strcpy(DeviceInfo.mac_address, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_firmwareVersion) == 0) {
			strcpy(DeviceInfo.firmware_version, tag->value);
			required |= xml_required_field(6);
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(6))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_supportreport(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_status(request *req, node_t *node)
{
	DeviceStatus_psia DeviceStatus;
	//SysInfo* pSysInfo = GetSysInfo();

	time_t tnow;
	struct tm *tmnow;

	tzset();
	time(&tnow);
	tmnow = localtime(&tnow);

	sprintf(DeviceStatus.current_device_time, "%04d-%02d-%02dT%02d:%02d:%02dZ", tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday, tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec);

	sprintf(DeviceStatus.device_up_time,"200");
	sprintf(DeviceStatus.tempSensorDescription,"IPNC");
	sprintf(DeviceStatus.temperature,"80");
	sprintf(DeviceStatus.cpu_description,"ARM9");
	sprintf(DeviceStatus.cpu_utilization,"80");
	sprintf(DeviceStatus.memory_description,"DDR2");
	sprintf(DeviceStatus.memory_usage,"60");
	sprintf(DeviceStatus.memory_available,"24");

	xadd_stag_attr(req, TAG_DeviceStatus, RootAttrStr);
	xadd_elem(req, TAG_currentDeviceTime, DeviceStatus.current_device_time);
	xadd_elem(req, TAG_deviceUpTime, DeviceStatus.device_up_time);

	xadd_stag(req, TAG_TemperatureList);
	xadd_stag(req, TAG_Temperature);
	xadd_elem(req, TAG_tempSensorDescription, DeviceStatus.tempSensorDescription);
	xadd_elem(req, TAG_temperature, DeviceStatus.temperature);
	xadd_etag(req, TAG_Temperature);
	xadd_etag(req, TAG_TemperatureList);

	xadd_stag(req, TAG_CPUList);
	xadd_stag(req, TAG_CPU);
	xadd_elem(req, TAG_cpuDescription, DeviceStatus.cpu_description);
	xadd_elem(req, TAG_cpuUtilization, DeviceStatus.cpu_utilization);
	xadd_etag(req, TAG_CPU);
	xadd_etag(req, TAG_CPUList);

	xadd_stag(req, TAG_MemoryList);
	xadd_stag(req, TAG_Memory);
	xadd_elem(req, TAG_memoryDescription, DeviceStatus.memory_description);
	xadd_elem(req, TAG_memoryUsage, DeviceStatus.memory_usage);
	xadd_elem(req, TAG_memoryAvailable, DeviceStatus.memory_available);
	xadd_etag(req, TAG_Memory);
	xadd_etag(req, TAG_MemoryList);

	xadd_etag(req, TAG_DeviceStatus);
	return STATUS_OK;
}

int retrieve_system_time(request *req, node_t *node)
{
	Time_psia Time;
	SysInfo* pSysInfo = GetSysInfo();
	time_t tnow;
	struct tm *tmnow;

	tzset();
	time(&tnow);
	tmnow = localtime(&tnow);

	sprintf(Time.timemode,"manual");
	sprintf(Time.localtime, "%04d-%02d-%02dT%02d:%02d:%02dZ", tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday, tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec);

	if (pSysInfo->lan_config.net.ntp_timezone <= 24)
		 sprintf(Time.timezone, "%s", TZname[pSysInfo->lan_config.net.ntp_timezone]);

	xadd_stag_attr(req, TAG_Time, RootAttrStr);
	xadd_elem(req, TAG_timeMode, Time.timemode);
	xadd_elem(req, TAG_localTime, Time.localtime);
	xadd_elem(req, TAG_timeZone, Time.timezone);
	xadd_etag(req, TAG_Time);

	return STATUS_OK;
}

int update_system_time(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	Time_psia Time;
	__u8 value;
	char localTimeStr[20];
	char tempStr[5];
	char *ch1,*ch2;
	int hour,minute,second,year,month,day;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_Time) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_timeMode) == 0) {
			strcpy(Time.timemode, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_localTime) == 0) {
			strcpy(Time.localtime, tag->value);
			required |= xml_required_field(2);

            ch1 = Time.localtime;
			ch2 = localTimeStr;

			while(*ch1 != '\0')
			{
			 if(*ch1 != '-' && *ch1 != 'T' && *ch1 != ':' && *ch1 != 'Z')
			 {
				*ch2 = *ch1;
				 ch2++;
			 }
			  ch1++;
			}
			*ch2 = '\0';

			//fprintf(stderr,"\r\nInside update_system_time with localTimeStr:%s\n",localTimeStr);

			strncpy(tempStr, localTimeStr, 4);
			tempStr[4] = '\0';
			year = atoi(tempStr);

			strncpy(tempStr, localTimeStr + 4, 2);
			tempStr[2] = '\0';
			month = atoi(tempStr);

			strncpy(tempStr, localTimeStr + 6, 2);
			tempStr[2] = '\0';
			day = atoi(tempStr);

			strncpy(tempStr, localTimeStr + 8, 2);
			tempStr[2] = '\0';
			hour = atoi(tempStr);

			strncpy(tempStr, localTimeStr + 10, 2);
			tempStr[2] = '\0';
			minute = atoi(tempStr);

			strncpy(tempStr, localTimeStr + 12, 2);
			tempStr[2] = '\0';
			second = atoi(tempStr);

			//fprintf(stderr,"\r\nyear:%d month:%d day:%d hour:%d minute:%d second: %d\n",year,month,day,hour,minute,second);

            sys_set_time(hour, minute, second);
	        sys_set_date(year, month, day);
		}
		else if(strcmp(tag->name, TAG_timeZone) == 0) {
			strcpy(Time.timezone, tag->value);

			if(strncmp(Time.timezone,"GMT-12",6) == 0)
               strcpy(Time.timezone,"0");
			else if(strncmp(Time.timezone,"GMT-11",6) == 0)
               strcpy(Time.timezone,"1");
			else if(strncmp(Time.timezone,"GMT-10",6) == 0)
               strcpy(Time.timezone,"2");
			else if(strncmp(Time.timezone,"GMT-09",6) == 0)
               strcpy(Time.timezone,"3");
			else if(strncmp(Time.timezone,"GMT-08",6) == 0)
               strcpy(Time.timezone,"4");
			else if(strncmp(Time.timezone,"GMT-07",6) == 0)
               strcpy(Time.timezone,"5");
			else if(strncmp(Time.timezone,"GMT-06",6) == 0)
               strcpy(Time.timezone,"6");
			else if(strncmp(Time.timezone,"GMT-05",6) == 0)
               strcpy(Time.timezone,"7");
			else if(strncmp(Time.timezone,"GMT-04",6) == 0)
               strcpy(Time.timezone,"8");
			else if(strncmp(Time.timezone,"GMT-03",6) == 0)
               strcpy(Time.timezone,"9");
			else if(strncmp(Time.timezone,"GMT-02",6) == 0)
               strcpy(Time.timezone,"10");
			else if(strncmp(Time.timezone,"GMT-01",6) == 0)
               strcpy(Time.timezone,"11");
			else if(strncmp(Time.timezone,"GMT+00",6) == 0)
               strcpy(Time.timezone,"12");
			else if(strncmp(Time.timezone,"GMT+01",6) == 0)
               strcpy(Time.timezone,"13");
			else if(strncmp(Time.timezone,"GMT+02",6) == 0)
               strcpy(Time.timezone,"14");
			else if(strncmp(Time.timezone,"GMT+03",6) == 0)
               strcpy(Time.timezone,"15");
			else if(strncmp(Time.timezone,"GMT+04",6) == 0)
               strcpy(Time.timezone,"16");
			else if(strncmp(Time.timezone,"GMT+05",6) == 0)
               strcpy(Time.timezone,"17");
			else if(strncmp(Time.timezone,"GMT+06",6) == 0)
               strcpy(Time.timezone,"18");
			else if(strncmp(Time.timezone,"GMT+07",6) == 0)
               strcpy(Time.timezone,"19");
			else if(strncmp(Time.timezone,"GMT+08",6) == 0)
               strcpy(Time.timezone,"20");
			else if(strncmp(Time.timezone,"GMT+09",6) == 0)
               strcpy(Time.timezone,"21");
			else if(strncmp(Time.timezone,"GMT+10",6) == 0)
               strcpy(Time.timezone,"22");
			else if(strncmp(Time.timezone,"GMT+11",6) == 0)
               strcpy(Time.timezone,"23");
			else if(strncmp(Time.timezone,"GMT+12",6) == 0)
               strcpy(Time.timezone,"24");

			required |= xml_required_field(3);
			value = atoi(Time.timezone);

			ControlSystemData(SFIELD_SET_TIMEZONE, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(4))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_time_capabilities(request *req, node_t *node)
{
	const char *timezoneStr = "opt=\"GMT-12 Eniwetok Kwajalein,GMT-11 Midway Island Samoa,GMT-10 Hawaii,GMT-09 Alaska,GMT-08 Pacific Time (US and Canada) Tijuana,GMT-07 Mountain Time (US and Canada) Arizona,GMT-06 Central Time (US and Canada) Mexico City Tegucigalpa Saskatchewan,GMT-05 Eastern Time (US and Canada) Indiana(East) Bogota Lima,GMT-04 Atlantic Time (Canada) Caracas La Paz,GMT-03 Brasilia Buenos Aires Georgetown,GMT-02 Mid-Atlantic,GMT-01 Azores Cape Verdes Is.,GMT+00 GMT Dublin Edinburgh London Lisbon Monrovia Casablanca,GMT+01 Berlin Stockholm Rome Bern Brussels Vienna Paris Madrid Amsterdam Prague Warsaw Budapest,GMT+02 Athens Helsinki Istanbul Cairo Eastern Europe Harare Pretoria Israel,GMT+03 Baghdad Kuwait Nairobi Riyadh Moscow St. Petersburg Kazan Volgograd,GMT+04 Abu Dhabi Muscat Tbilisi,GMT+05 Islamabad Karachi Ekaterinburg Tashkent,GMT+06 Alma Ata Dhaka,GMT+07 Bangkok Jakarta Hanoi,GMT+08 Taipei Beijing Chongqing Urumqi Hong Kong Perth Singapore,GMT+09 Tokyo Osaka Sapporo Seoul Yakutsk,GMT+10 Brisbane Melbourne Sydney Guam Port Moresby Vladivostok Hobart,GMT+11 Magadan Solomon Is. New Caledonia,GMT+12 Fiji Kamchatka Marshall Is. Wellington Auckland\"";

	xadd_stag_attr(req, TAG_Time, RootAttrStr);
	xadd_stag_attr(req, TAG_timeZone, timezoneStr);
	xadd_etag(req, TAG_timeZone);
	xadd_etag(req, TAG_Time);

	return STATUS_OK;
}

int retrieve_system_time_localtime(request *req, node_t *node)
{
	//const char *Local_Time;
	char local_time[30];
	time_t tnow;
	struct tm *tmnow;

	tzset();
	time(&tnow);
	tmnow = localtime(&tnow);

	sprintf(local_time, "%04d-%02d-%02dT%02d:%02d:%02dZ", tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday, tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec);

	send_r_psia_datetime(req,local_time);

	return STATUS_OK;
}

int update_system_time_localtime(request *req, node_t *node)
{
	Time_psia Time;
	char localTimeStr[20];
	char tempStr[5];
	char *ch1,*ch2;
	int hour,minute,second,year,month,day;

	strcpy(Time.localtime,req->inbuf);

	ch1 = Time.localtime;
	ch2 = localTimeStr;

	while(*ch1 != '\0')
	{
	 if(*ch1 != '-' && *ch1 != 'T' && *ch1 != ':' && *ch1 != 'Z')
	 {
		*ch2 = *ch1;
		 ch2++;
	 }
	  ch1++;
	}
	*ch2 = '\0';

	strncpy(tempStr, localTimeStr, 4);
	tempStr[4] = '\0';
	year = atoi(tempStr);

	strncpy(tempStr, localTimeStr + 4, 2);
	tempStr[2] = '\0';
	month = atoi(tempStr);

    strncpy(tempStr, localTimeStr + 6, 2);
	tempStr[2] = '\0';
	day = atoi(tempStr);

	strncpy(tempStr, localTimeStr + 8, 2);
	tempStr[2] = '\0';
	hour = atoi(tempStr);

	strncpy(tempStr, localTimeStr + 10, 2);
	tempStr[2] = '\0';
	minute = atoi(tempStr);

	strncpy(tempStr, localTimeStr + 12, 2);
	tempStr[2] = '\0';
	second = atoi(tempStr);

    sys_set_time(hour, minute, second);
	sys_set_date(year, month, day);

	return STATUS_OK;
}

int retrieve_system_time_timezone(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	char timezone[100];

	if (pSysInfo->lan_config.net.ntp_timezone <= 24)
		 sprintf(timezone, "%s", TZname[pSysInfo->lan_config.net.ntp_timezone]);

	send_r_psia_timezone(req,timezone);

	return STATUS_OK;
}

int update_system_time_timezone(request *req, node_t *node)
{
	Time_psia Time;
	__u8 value;

	strcpy(Time.timezone,req->inbuf);

	if(strncmp(Time.timezone,"GMT-12",6) == 0)
       strcpy(Time.timezone,"0");
	else if(strncmp(Time.timezone,"GMT-11",6) == 0)
       strcpy(Time.timezone,"1");
	else if(strncmp(Time.timezone,"GMT-10",6) == 0)
       strcpy(Time.timezone,"2");
	else if(strncmp(Time.timezone,"GMT-09",6) == 0)
       strcpy(Time.timezone,"3");
	else if(strncmp(Time.timezone,"GMT-08",6) == 0)
       strcpy(Time.timezone,"4");
	else if(strncmp(Time.timezone,"GMT-07",6) == 0)
       strcpy(Time.timezone,"5");
	else if(strncmp(Time.timezone,"GMT-06",6) == 0)
       strcpy(Time.timezone,"6");
	else if(strncmp(Time.timezone,"GMT-05",6) == 0)
       strcpy(Time.timezone,"7");
	else if(strncmp(Time.timezone,"GMT-04",6) == 0)
       strcpy(Time.timezone,"8");
	else if(strncmp(Time.timezone,"GMT-03",6) == 0)
       strcpy(Time.timezone,"9");
	else if(strncmp(Time.timezone,"GMT-02",6) == 0)
       strcpy(Time.timezone,"10");
	else if(strncmp(Time.timezone,"GMT-01",6) == 0)
       strcpy(Time.timezone,"11");
	else if(strncmp(Time.timezone,"GMT+00",6) == 0)
       strcpy(Time.timezone,"12");
	else if(strncmp(Time.timezone,"GMT+01",6) == 0)
       strcpy(Time.timezone,"13");
	else if(strncmp(Time.timezone,"GMT+02",6) == 0)
       strcpy(Time.timezone,"14");
	else if(strncmp(Time.timezone,"GMT+03",6) == 0)
       strcpy(Time.timezone,"15");
	else if(strncmp(Time.timezone,"GMT+04",6) == 0)
       strcpy(Time.timezone,"16");
	else if(strncmp(Time.timezone,"GMT+05",6) == 0)
       strcpy(Time.timezone,"17");
	else if(strncmp(Time.timezone,"GMT+06",6) == 0)
       strcpy(Time.timezone,"18");
	else if(strncmp(Time.timezone,"GMT+07",6) == 0)
       strcpy(Time.timezone,"19");
	else if(strncmp(Time.timezone,"GMT+08",6) == 0)
       strcpy(Time.timezone,"20");
	else if(strncmp(Time.timezone,"GMT+09",6) == 0)
       strcpy(Time.timezone,"21");
	else if(strncmp(Time.timezone,"GMT+10",6) == 0)
       strcpy(Time.timezone,"22");
	else if(strncmp(Time.timezone,"GMT+11",6) == 0)
       strcpy(Time.timezone,"23");
	else if(strncmp(Time.timezone,"GMT+12",6) == 0)
       strcpy(Time.timezone,"24");

	value = atoi(Time.timezone);

	ControlSystemData(SFIELD_SET_TIMEZONE, (void *)&value, sizeof(value));

	return STATUS_OK;
}

int retrieve_system_time_ntpservers(request *req, node_t *node)
{
    int c;
	char idstr[64];

	xadd_stag_attr(req, TAG_NtpServerList, RootAttrStr);
	for(c = 0; c < get_ntpserver_count(); c++) {
		get_ntpserver_url(c, idstr);
		retrieve_system_time_ntpservers_id(req, node);
	}
	xadd_etag(req, TAG_NtpServerList);

	return STATUS_OK;
}

int update_system_time_ntpservers(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_NtpServerList) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_NtpServer) == 0) {
			idflag = 1;
			update_system_time_ntpservers_id(req, node, tag);
		    required |= xml_required_field(1);
		}
	 /* Protocol */
		tag = tag->next;
	}
	idflag = 0;

	return STATUS_OK;
}

int create_system_time_ntpservers(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int delete_system_time_ntpservers(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_time_ntpservers_id(request *req, node_t *node)
{
	NtpServer_psia NtpServer;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(NtpServer.id,"0");
	sprintf(NtpServer.addressingFormatType,"hostname");
	sprintf(NtpServer.hostname,"%s",pSysInfo->lan_config.net.ntp_server);
	sprintf(NtpServer.ip_address,"NA");
	sprintf(NtpServer.portNo,"NA");

	xadd_stag_attr(req, TAG_NtpServer, RootAttrStr);
	xadd_elem(req, TAG_id, NtpServer.id);
	xadd_elem(req, TAG_addressingFormatType,NtpServer.addressingFormatType);
	xadd_elem(req, TAG_hostName,NtpServer.hostname);
	//xadd_elem(req, TAG_ipAddress, NtpServer.ip_address);
	//xadd_elem(req, TAG_portNo, NtpServer.portNo);
	xadd_etag(req, TAG_NtpServer);

	return STATUS_OK;
}

int update_system_time_ntpservers_id(request *req, node_t *node, tag_t *parentTag)
{
	tag_t	*tag = parentTag;
	unsigned long required;
	NtpServer_psia NtpServer;

	fprintf(stderr,"\r\nUpdating NTPServers List...\n");

	if(idflag == 0){
	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_NtpServer) == 0)))
		return STATUS_INVALID_XML_CONTENT;
	}

	fprintf(stderr,"\r\nInside update_system_time_ntpservers_id with tag:%s\n",tag->name);

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(NtpServer.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_addressingFormatType) == 0) {
			strcpy(NtpServer.addressingFormatType, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_hostName) == 0) {
			strcpy(NtpServer.hostname, tag->value);
			required |= xml_required_field(3);
			ControlSystemData(SFIELD_SET_SNTP_SERVER, (void *)NtpServer.hostname, strlen(NtpServer.hostname));
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(NtpServer.ip_address, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_portNo) == 0) {
			strcpy(NtpServer.portNo, tag->value);
			required |= xml_required_field(5);
		}
		/* Protocol */

		tag = tag->next;
	}

	/* check all the required fields were present */
	//if(required != xml_required_mask(5))
		//return STATUS_INVALID_XML_CONTENT;
	return STATUS_OK;
}

int delete_system_time_ntpservers_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_system_logging(request *req, node_t *node)
{
	char maxEntries[20];

	sprintf(maxEntries,"%d",MAX_LOG_PAGE_NUM * NUM_LOG_PER_PAGE);

	xadd_stag_attr(req, TAG_Logging, RootAttrStr);
	xadd_stag(req, TAG_LocalLog);
	xadd_elem(req, TAG_maxEntries,maxEntries);
	xadd_etag(req, TAG_LocalLog);
	xadd_etag(req, TAG_Logging);

	return STATUS_OK;
}

int update_system_logging(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_logging_messages(request *req, node_t *node)
{
	int i; //j;

	xadd_stag_attr(req, TAG_LogMessageList, RootAttrStr);
	for(i = 0; i < NUM_LOG_PER_PAGE; i++) {
		retrieve_system_logging_messages_id(req, node);
		logIndex++;
	}
	logIndex = 0;
	xadd_etag(req, TAG_LogMessageList);

	return STATUS_OK;
}

int retrieve_system_logging_messages_id(request *req, node_t *node)
{
	LogMessage_psia LogMessage;
	//SysInfo* pSysInfo = GetSysInfo();

	LogEntry_t tLog;
	struct tm* ptm;
	GetSysLog(0, logIndex, &tLog);

	ptm = &tLog.time;

	sprintf(LogMessage.logNo,"%d",logIndex);
	sprintf(LogMessage.dateTime,"%d-%02d-%02d %02d:%02d:%02d",ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	sprintf(LogMessage.severity,"NA");
	sprintf(LogMessage.message,"%s",tLog.event);

	xadd_stag(req, TAG_LogMessage);
	xadd_elem(req, TAG_logNo,LogMessage.logNo);
	xadd_elem(req, TAG_dateTime,LogMessage.dateTime);
	xadd_elem(req, TAG_severity, LogMessage.severity);
	xadd_elem(req, TAG_message, LogMessage.message);
	xadd_etag(req, TAG_LogMessage);

	return STATUS_OK;
}

int retrieve_custom_system_log(request *req, node_t *node)
{
	LogMessage_psia LogMessage;
	//SysInfo* pSysInfo = GetSysInfo();

	LogEntry_t tLog;
	struct tm* ptm;
	int i,logNum=0;

	xadd_stag_attr(req, TAG_LogMessageList, RootAttrStr);

	for(i = 0; i < NUM_LOG_PER_PAGE; i++) {
		GetSysLog(0, logIndex, &tLog);
		ptm = &tLog.time;

		if(strstr(tLog.event, "login") == NULL){

		  logNum++;
		  sprintf(LogMessage.logNo,"%d",logNum);
	      sprintf(LogMessage.dateTime,"%d-%02d-%02d %02d:%02d:%02d",ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	      sprintf(LogMessage.severity,"NA");
	      sprintf(LogMessage.message,"%s",tLog.event);

	      xadd_stag(req, TAG_LogMessage);
	      xadd_elem(req, TAG_logNo,LogMessage.logNo);
	      xadd_elem(req, TAG_dateTime,LogMessage.dateTime);
	      xadd_elem(req, TAG_severity, LogMessage.severity);
	      xadd_elem(req, TAG_message, LogMessage.message);
	      xadd_etag(req, TAG_LogMessage);
	    }
	    logIndex++;
	}
	logIndex = 0;
	logNum = 0;

	xadd_etag(req, TAG_LogMessageList);

	return STATUS_OK;
}

int update_custom_system_log(request *req, node_t *node)
{
	cleanSystemLog();

	return STATUS_OK;
}


int retrieve_custom_access_log(request *req, node_t *node)
{
	LogMessage_psia LogMessage;
	//SysInfo* pSysInfo = GetSysInfo();

	LogEntry_t tLog;
	struct tm* ptm;
	int i,logNum=0;

	xadd_stag_attr(req, TAG_LogMessageList, RootAttrStr);

	for(i = 0; i < NUM_LOG_PER_PAGE; i++) {
		GetSysLog(0, logIndex, &tLog);
		ptm = &tLog.time;

		if(strstr(tLog.event, "login") != NULL){

		  logNum++;
		  sprintf(LogMessage.logNo,"%d",logNum);
	      sprintf(LogMessage.dateTime,"%d-%02d-%02d %02d:%02d:%02d",ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	      sprintf(LogMessage.severity,"NA");
	      sprintf(LogMessage.message,"%s",tLog.event);

	      xadd_stag(req, TAG_LogMessage);
	      xadd_elem(req, TAG_logNo,LogMessage.logNo);
	      xadd_elem(req, TAG_dateTime,LogMessage.dateTime);
	      xadd_elem(req, TAG_severity, LogMessage.severity);
	      xadd_elem(req, TAG_message, LogMessage.message);
	      xadd_etag(req, TAG_LogMessage);
	    }
	    logIndex++;
	}
	logIndex = 0;
	logNum = 0;

	xadd_etag(req, TAG_LogMessageList);

	return STATUS_OK;
}


int update_custom_access_log(request *req, node_t *node)
{
	cleanAccessLog();

	return STATUS_OK;
}


int retrieve_custom_active_list(request *req, node_t *node)
{
	LogMessage_psia LogMessage;
	//SysInfo* pSysInfo = GetSysInfo();

	LogEntry_t tLog;
	struct tm* ptm;
	int logNum=0;

	xadd_stag_attr(req, TAG_LogMessageList, RootAttrStr);

	GetSysLog(0, logIndex, &tLog);
	ptm = &tLog.time;

	logNum++;
	sprintf(LogMessage.logNo,"%d",logNum);
	sprintf(LogMessage.dateTime,"%d-%02d-%02d %02d:%02d:%02d",ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	sprintf(LogMessage.severity,"NA");
	sprintf(LogMessage.message,"%s",tLog.event);

	xadd_stag(req, TAG_LogMessage);
	xadd_elem(req, TAG_logNo,LogMessage.logNo);
	xadd_elem(req, TAG_dateTime,LogMessage.dateTime);
	xadd_elem(req, TAG_severity, LogMessage.severity);
	xadd_elem(req, TAG_message, LogMessage.message);
	xadd_etag(req, TAG_LogMessage);

	xadd_etag(req, TAG_LogMessageList);

	return STATUS_OK;
}


int update_custom_active_list(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_param_list(request *req, node_t *node)
{
	Curparam_psia Curparam;
	char String[40];

	xadd_stag_attr(req, TAG_CurParamList, RootAttrStr);

	GetStreamType(String);
	sprintf(Curparam.paramName,"Stream Type");
	sprintf(Curparam.curVal,"%s",String);
	sprintf(Curparam.defaultVal,"Single");

	xadd_stag(req, TAG_CurParam);
	xadd_elem(req, TAG_paramName,Curparam.paramName);
	xadd_elem(req, TAG_curVal,Curparam.curVal);
	xadd_elem(req, TAG_defaultVal, Curparam.defaultVal);
	xadd_etag(req, TAG_CurParam);

	GetCodecCombo(String);
	sprintf(Curparam.paramName,"Codec Combo");
	sprintf(Curparam.curVal,"%s",String);
	sprintf(Curparam.defaultVal,"H.264");

	xadd_stag(req, TAG_CurParam);
	xadd_elem(req, TAG_paramName,Curparam.paramName);
	xadd_elem(req, TAG_curVal,Curparam.curVal);
	xadd_elem(req, TAG_defaultVal, Curparam.defaultVal);
	xadd_etag(req, TAG_CurParam);

	GetResolution(String);
	sprintf(Curparam.paramName,"Resolution");
	sprintf(Curparam.curVal,"%s",String);
	sprintf(Curparam.defaultVal,"H264:720");

	xadd_stag(req, TAG_CurParam);
	xadd_elem(req, TAG_paramName,Curparam.paramName);
	xadd_elem(req, TAG_curVal,Curparam.curVal);
	xadd_elem(req, TAG_defaultVal, Curparam.defaultVal);
	xadd_etag(req, TAG_CurParam);

	xadd_etag(req, TAG_CurParamList);

	return STATUS_OK;
}

int get_ntpserver_count(void)
{
	return 1;
}

void get_ntpserver_url(int id, char *idstr)
{
	/* default ID 0,1,2,3... */
	sprintf(idstr,"/%d", id);
}

int get_ntpserver_index(char *idstr)
{
	idstr++;  // skip the '/'
	return(atoi(idstr));
}

/*******************************************************************/
/*                             Security                            */
/*******************************************************************/

static int DemoUserCount = 4;

int get_user_count(void)
{
	return DemoUserCount;
}

int retrieve_process_security_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"AAA");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_security_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"Security");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "Security Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);

	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);

	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

int retrieve_process_security_aaa_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"users");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_security_aaa_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"AAA");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "Authentication and authorization Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);

	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);

	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

int retieve_security_aaa_users(request *req, node_t *node)
{
	int i;
	//char idstr[64];
	//User_psia User;

	xadd_stag_attr(req, TAG_UserList, RootAttrStr);
	for(i = 0; i < ACOUNT_NUM; i++) {
		retieve_security_aaa_users_id(req, node);
		userIndex++;
	}
	userIndex = 0;
	xadd_etag(req, TAG_UserList);
	return STATUS_OK;
}

int update_security_aaa_users(request *req, node_t *node)
{
	/* update a user list */
	tag_t	*tag;
	unsigned long required;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_UserList) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_User) == 0) {
			idflag = 1;
			update_security_aaa_users_id(req, node,tag);
		    required |= xml_required_field(1);
		}
	 /* Protocol */
		tag = tag->next;
	}
	idflag = 0;
	/* check all the required fileds were present */
	//if(required != xml_required_mask(1))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int create_security_aaa_users(request *req, node_t *node)
{
	int ret;
	tag_t	*tag;
	unsigned long required;
	User_psia User;
	add_user_t account;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	// AYK - 11032010
	// PSIA conformance test utility v1.0.0.12 doesn't send the TAG_UserList tag
	// while adding user instead it sends one TAG_User tag
	// so commenting out the following check

#if 0
	if(!((tag) && (strcmp(tag->name, TAG_UserList) == 0)))
		return STATUS_INVALID_XML_CONTENT;
#endif

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_User) == 0){
		  tag = tag->first_child;
		  while(tag) {
		    if(strcmp(tag->name, TAG_id) == 0) {
			 strcpy(User.id, tag->value);
			 required |= xml_required_field(1);
		    }
		    else if(strcmp(tag->name, TAG_userName) == 0) {
			 strcpy(User.userName, tag->value);
		 	 required |= xml_required_field(2);
		    }
		    else if(strcmp(tag->name, TAG_password) == 0) {
			 strcpy(User.password, tag->value);
			 required |= xml_required_field(3);
		    }
			if(!tag->next)
			break;
		    tag = tag->next;
		  }
		  tag = tag->parent;
		}
		strcpy(account.user_id,User.userName);
	    strcpy(account.password,User.password);

        ret = ControlSystemData(SFIELD_ADD_USER, (void *)&account, sizeof(account));

		if(!tag->next)
		  break;
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(3))
		//return STATUS_INVALID_XML_CONTENT;

	/* <!--- add new user to list ---> */
	return STATUS_OK;
}

int delete_security_aaa_users(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	//add_user_t account;
	User_psia User;
	int i; //ret;

	//fprintf(stderr,"\r\nInside delete_security_aaa_users\n");

	for(i = 0; i < ACOUNT_NUM; i++) {
		sprintf(User.userName,"%s",pSysInfo->acounts[userIndex].user);

		if(strlen(User.userName))
		   ControlSystemData(SFIELD_DEL_USER, (void *)User.userName, strlen(User.userName));

		userIndex++;
	}
	userIndex = 0;

	return STATUS_OK;
}

int retieve_security_aaa_users_id(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	User_psia User;

	sprintf(User.id,"%d",userIndex);
	sprintf(User.userName,"%s",pSysInfo->acounts[userIndex].user);
	sprintf(User.password,"%s",pSysInfo->acounts[userIndex].password);

	xadd_stag_attr(req, TAG_User, RootAttrStr);
	xadd_elem(req, TAG_id, User.id);
	xadd_elem(req, TAG_userName, User.userName);
	//xadd_elem(req, TAG_password, User.password);
	xadd_etag(req, TAG_User);

	return STATUS_OK;
}

int update_security_aaa_users_id(request *req, node_t *node, tag_t *parentTag)
{
	int ret;
	tag_t	*tag = parentTag;
	unsigned long required;
	User_psia User;
	add_user_t account;

	fprintf(stderr,"\r\nUpdating User List...\n");

	if(idflag == 0){
	// returns bytes processed or -1
	  if(xml_validate(req, req->inbuf, req->filesize) < 0)
		 return STATUS_INVALID_XML_FORMAT;

	  tag = req->root_tag;
	  if(!((tag) && (strcmp(tag->name, TAG_User) == 0)))
		  return STATUS_INVALID_XML_CONTENT;
	}
	fprintf(stderr,"\r\nInside update_security_aaa_users_id with tag:%s\n",tag->name);
	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(User.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_userName) == 0) {
			strcpy(User.userName, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_password) == 0) {
			strcpy(User.password, tag->value);
			required |= xml_required_field(3);
		}
		/* Protocol */
		tag = tag->next;
	}

	strcpy(account.user_id,User.userName);
	strcpy(account.password,User.password);

    ret = ControlSystemData(SFIELD_ADD_USER, (void *)&account, sizeof(account));
	/* check all the required fileds were present */
	//if(required != xml_required_mask(4))
		//return STATUS_INVALID_XML_CONTENT;

	/* <!--- add new user to list ---> */
	return STATUS_OK;
}

int delete_security_aaa_users_id(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	//add_user_t account;
	User_psia User;

	if(strstr(req->request_uri, "0"))
	   sprintf(User.userName,"%s",pSysInfo->acounts[0].user);
	else if(strstr(req->request_uri, "1"))
	   sprintf(User.userName,"%s",pSysInfo->acounts[1].user);
	else if(strstr(req->request_uri, "2"))
	   sprintf(User.userName,"%s",pSysInfo->acounts[2].user);
	else if(strstr(req->request_uri, "3"))
	   sprintf(User.userName,"%s",pSysInfo->acounts[3].user);
	else if(strstr(req->request_uri, "4"))
	   sprintf(User.userName,"%s",pSysInfo->acounts[4].user);

	ControlSystemData(SFIELD_DEL_USER, (void *)User.userName, strlen(User.userName));

	return STATUS_OK;
}

int retrieve_security_aaa_users_id_capabilities(request *req, node_t *node)
{
	return STATUS_OK;
}

/*******************************************************************/
/*                            Streaming                            */
/*******************************************************************/
int retrieve_process_streaming_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
    xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"status");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"channels");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_streaming_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"Streaming");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "Streaming Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);

	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);

	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

int get_channel_count(void)
{
	return 1;
}

void get_channel_url(int id, char *idstr)
{
	/* default ID 0,1,2,3... */
	sprintf(idstr,"/%d", id);
}

int get_channel_index(char *idstr)
{
	idstr++;  // skip the '/'
	return(atoi(idstr));
}

int retrieve_streaming_status(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	StreamingStatus_psia StreamingStatus;

	if(pSysInfo->lan_config.nStreamType == 0)
		sprintf(StreamingStatus.totalStreamingSessions,"1");
	else if (pSysInfo->lan_config.nStreamType == 1)
	    sprintf(StreamingStatus.totalStreamingSessions,"2");
	else if (pSysInfo->lan_config.nStreamType == 2)
	    sprintf(StreamingStatus.totalStreamingSessions,"3");

	xadd_stag_attr(req, TAG_StreamingStatus, RootAttrStr);
	xadd_elem(req, TAG_totalStreamingSessions, StreamingStatus.totalStreamingSessions);
	retrieve_streaming_channels_id_status(req,node);
	xadd_etag(req, TAG_StreamingStatus);

	return STATUS_OK;
}

int retrieve_streaming_channels(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	int c,channel_count = 0;
	char idstr[64];

	xadd_stag_attr(req, TAG_StreamingChannelList, RootAttrStr);
#if 1
	if(pSysInfo->lan_config.nStreamType == 0)
		channel_count = 1;
	else if (pSysInfo->lan_config.nStreamType == 1)
	    channel_count = 2;
	else if (pSysInfo->lan_config.nStreamType == 2)
	    channel_count = 3;
#endif

	for(c = 0; c < channel_count; c++) {
		get_channel_url(c, idstr);
		retrieve_streaming_channels_id(req, node);
		channel_id ++;
	}
	xadd_etag(req, TAG_StreamingChannelList);
	channel_id = 0;

	return STATUS_OK;
}

int retrieve_streaming_channels_id(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	StreamingChannel_psia StreamingChannel;

    sprintf(StreamingChannel.rtpTransportType,"RTP/TCP");
	sprintf(StreamingChannel.enabled,"1");
	sprintf(StreamingChannel.disabled,"0");
	sprintf(StreamingChannel.videoScanType,"interlaced");
	sprintf(StreamingChannel.audioInputChannelID,"NA");

	if(channel_id == 0){
	 if(pSysInfo->lan_config.nStreamType == 0){
		sprintf(StreamingChannel.videoInputChannelID,"Single");

		if(pSysInfo->lan_config.nVideocombo == 0){
		  sprintf(StreamingChannel.id,"2");
		  sprintf(StreamingChannel.channelName,"H264 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8557");
		  sprintf(StreamingChannel.videoCodecType,"H.264");
		  sprintf(StreamingChannel.fixedQuality,"0");
		  sprintf(StreamingChannel.constantBitRate,"%d",pSysInfo->lan_config.nMpeg41bitrate/1000);
		  sprintf(StreamingChannel.keyFrameInterval,"%d",pSysInfo->codec_advconfig[0].ipRatio);

		  if(pSysInfo->lan_config.nRateControl1 == 1)
			sprintf(StreamingChannel.videoQualityControlType,"VBR");
		  else if(pSysInfo->lan_config.nRateControl1 == 2)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");
		  else if(pSysInfo->lan_config.nRateControl1 == 0)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"1280");
	        sprintf(StreamingChannel.videoResolutionHeight,"720");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			   sprintf(StreamingChannel.maxFrameRate,"30");
			else if(pSysInfo->lan_config.nFrameRate1 == 1)
			   sprintf(StreamingChannel.maxFrameRate,"24");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			   sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 3)
			   sprintf(StreamingChannel.maxFrameRate,"8");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			  sprintf(StreamingChannel.maxFrameRate,"30");
			else if(pSysInfo->lan_config.nFrameRate1 == 1)
			  sprintf(StreamingChannel.maxFrameRate,"24");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			  sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 3)
			  sprintf(StreamingChannel.maxFrameRate,"8");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 2){
			sprintf(StreamingChannel.videoResolutionWidth,"1280");
	        sprintf(StreamingChannel.videoResolutionHeight,"960");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			  sprintf(StreamingChannel.maxFrameRate,"24");
		    else if(pSysInfo->lan_config.nFrameRate1 == 1)
			  sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			  sprintf(StreamingChannel.maxFrameRate,"8");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 3){
			sprintf(StreamingChannel.videoResolutionWidth,"1920");
	        sprintf(StreamingChannel.videoResolutionHeight,"1072");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			  sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 1)
			  sprintf(StreamingChannel.maxFrameRate,"12");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			  sprintf(StreamingChannel.maxFrameRate,"8");
		  }
	    }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
		  sprintf(StreamingChannel.id,"1");
		  sprintf(StreamingChannel.channelName,"MPEG4 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8553");
		  sprintf(StreamingChannel.videoCodecType,"MPEG4");
		  sprintf(StreamingChannel.fixedQuality,"0");
		  sprintf(StreamingChannel.constantBitRate,"%d",pSysInfo->lan_config.nMpeg41bitrate/1000);
		  sprintf(StreamingChannel.keyFrameInterval,"%d",pSysInfo->codec_advconfig[0].ipRatio);

		  if(pSysInfo->lan_config.nRateControl1 == 1)
			sprintf(StreamingChannel.videoQualityControlType,"VBR");
		  else if(pSysInfo->lan_config.nRateControl1 == 2)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");
		  else if(pSysInfo->lan_config.nRateControl1 == 0)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"1280");
	        sprintf(StreamingChannel.videoResolutionHeight,"720");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			   sprintf(StreamingChannel.maxFrameRate,"30");
			else if(pSysInfo->lan_config.nFrameRate1 == 1)
			   sprintf(StreamingChannel.maxFrameRate,"24");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			   sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 3)
			   sprintf(StreamingChannel.maxFrameRate,"8");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			   sprintf(StreamingChannel.maxFrameRate,"30");
		    else if(pSysInfo->lan_config.nFrameRate1 == 1)
			   sprintf(StreamingChannel.maxFrameRate,"24");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			   sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 3)
			   sprintf(StreamingChannel.maxFrameRate,"8");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 2){
			sprintf(StreamingChannel.videoResolutionWidth,"1280");
	        sprintf(StreamingChannel.videoResolutionHeight,"960");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			   sprintf(StreamingChannel.maxFrameRate,"30");
		    else if(pSysInfo->lan_config.nFrameRate1 == 1)
			   sprintf(StreamingChannel.maxFrameRate,"24");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			   sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 3)
			   sprintf(StreamingChannel.maxFrameRate,"8");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 3){
			sprintf(StreamingChannel.videoResolutionWidth,"1920");
	        sprintf(StreamingChannel.videoResolutionHeight,"1072");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			   sprintf(StreamingChannel.maxFrameRate,"21");
		    else if(pSysInfo->lan_config.nFrameRate1 == 1)
			   sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			   sprintf(StreamingChannel.maxFrameRate,"8");
		  }
	    }
	    else if (pSysInfo->lan_config.nVideocombo == 2){
		  sprintf(StreamingChannel.id,"0");
		  sprintf(StreamingChannel.channelName,"MJPEG Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8555");
		  sprintf(StreamingChannel.videoCodecType,"MJPEG");
		  sprintf(StreamingChannel.fixedQuality,"%d",pSysInfo->lan_config.njpegquality);
		  sprintf(StreamingChannel.constantBitRate,"0");
		  sprintf(StreamingChannel.videoQualityControlType,"CBR");
		  sprintf(StreamingChannel.keyFrameInterval,"0");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"1600");
	        sprintf(StreamingChannel.videoResolutionHeight,"1200");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			   sprintf(StreamingChannel.maxFrameRate,"24");
		    else if(pSysInfo->lan_config.nFrameRate1 == 1)
			   sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			   sprintf(StreamingChannel.maxFrameRate,"8");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"2048");
	        sprintf(StreamingChannel.videoResolutionHeight,"1536");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			   sprintf(StreamingChannel.maxFrameRate,"15");
		    else if(pSysInfo->lan_config.nFrameRate1 == 1)
			   sprintf(StreamingChannel.maxFrameRate,"12");
		    else if(pSysInfo->lan_config.nFrameRate1 == 2)
			   sprintf(StreamingChannel.maxFrameRate,"8");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 2){
			sprintf(StreamingChannel.videoResolutionWidth,"2592");
	        sprintf(StreamingChannel.videoResolutionHeight,"1920");

			if(pSysInfo->lan_config.nFrameRate1 == 0)
			   sprintf(StreamingChannel.maxFrameRate,"Auto");
		  }
        }
	 }
	 else if (pSysInfo->lan_config.nStreamType == 1){
	    sprintf(StreamingChannel.videoInputChannelID,"Dual");
		sprintf(StreamingChannel.keyFrameInterval,"%d",pSysInfo->codec_advconfig[0].ipRatio);

		if(pSysInfo->lan_config.nRateControl1 == 1)
		   sprintf(StreamingChannel.videoQualityControlType,"VBR");
		else if(pSysInfo->lan_config.nRateControl1 == 2)
		   sprintf(StreamingChannel.videoQualityControlType,"CBR");
		else if(pSysInfo->lan_config.nRateControl1 == 0)
		   sprintf(StreamingChannel.videoQualityControlType,"CBR");

		if(pSysInfo->lan_config.nFrameRate1 == 0)
		   sprintf(StreamingChannel.maxFrameRate,"30");
		else if(pSysInfo->lan_config.nFrameRate1 == 1)
		   sprintf(StreamingChannel.maxFrameRate,"15");
		else if(pSysInfo->lan_config.nFrameRate1 == 2)
		   sprintf(StreamingChannel.maxFrameRate,"8");

		sprintf(StreamingChannel.fixedQuality,"0");
		sprintf(StreamingChannel.constantBitRate,"%d",pSysInfo->lan_config.nMpeg41bitrate/1000);

		if (pSysInfo->lan_config.nVideocombo == 0){
		  sprintf(StreamingChannel.id,"2");
		  sprintf(StreamingChannel.channelName,"H264 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8557");
		  sprintf(StreamingChannel.videoCodecType,"H.264");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"1280");
	        sprintf(StreamingChannel.videoResolutionHeight,"720");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
		  sprintf(StreamingChannel.id,"1");
		  sprintf(StreamingChannel.channelName,"MPEG4 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8553");
		  sprintf(StreamingChannel.videoCodecType,"MPEG4");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"1280");
	        sprintf(StreamingChannel.videoResolutionHeight,"720");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 2){
		  sprintf(StreamingChannel.id,"2");
		  sprintf(StreamingChannel.channelName,"H264 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8557");
		  sprintf(StreamingChannel.videoCodecType,"H.264");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"1280");
	        sprintf(StreamingChannel.videoResolutionHeight,"720");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 3){
		  sprintf(StreamingChannel.id,"1");
		  sprintf(StreamingChannel.channelName,"MPEG4 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8553");
		  sprintf(StreamingChannel.videoCodecType,"MPEG4");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"1280");
	        sprintf(StreamingChannel.videoResolutionHeight,"720");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 4){
		  sprintf(StreamingChannel.id,"2");
		  sprintf(StreamingChannel.channelName,"H264 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8557");
		  sprintf(StreamingChannel.videoCodecType,"H.264");

		  sprintf(StreamingChannel.videoResolutionWidth,"720");
	      sprintf(StreamingChannel.videoResolutionHeight,"480");
        }
	 }
	 else if (pSysInfo->lan_config.nStreamType == 2){
	    sprintf(StreamingChannel.videoInputChannelID,"Tri-Stream");
		sprintf(StreamingChannel.keyFrameInterval,"%d",pSysInfo->codec_advconfig[0].ipRatio);

		if(pSysInfo->lan_config.nRateControl1 == 1)
		   sprintf(StreamingChannel.videoQualityControlType,"VBR");
		else if(pSysInfo->lan_config.nRateControl1 == 2)
		   sprintf(StreamingChannel.videoQualityControlType,"CBR");
		else if(pSysInfo->lan_config.nRateControl1 == 0)
		   sprintf(StreamingChannel.videoQualityControlType,"CBR");

		if(pSysInfo->lan_config.nFrameRate1 == 0)
		   sprintf(StreamingChannel.maxFrameRate,"30");
		else if(pSysInfo->lan_config.nFrameRate1 == 1)
		   sprintf(StreamingChannel.maxFrameRate,"24");
		else if(pSysInfo->lan_config.nFrameRate1 == 2)
		   sprintf(StreamingChannel.maxFrameRate,"15");
		else if(pSysInfo->lan_config.nFrameRate1 == 3)
		   sprintf(StreamingChannel.maxFrameRate,"8");

		sprintf(StreamingChannel.fixedQuality,"0");
		sprintf(StreamingChannel.constantBitRate,"%d",pSysInfo->lan_config.nMpeg41bitrate/1000);

		if (pSysInfo->lan_config.nVideocombo == 0){
		  sprintf(StreamingChannel.id,"2");
		  sprintf(StreamingChannel.channelName,"H264 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8557");
		  sprintf(StreamingChannel.videoCodecType,"H.264");

		  sprintf(StreamingChannel.videoResolutionWidth,"1280");
	      sprintf(StreamingChannel.videoResolutionHeight,"720");
        }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
		  sprintf(StreamingChannel.id,"1");
		  sprintf(StreamingChannel.channelName,"MPEG4 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8553");
		  sprintf(StreamingChannel.videoCodecType,"MPEG4");

		  sprintf(StreamingChannel.videoResolutionWidth,"1280");
	      sprintf(StreamingChannel.videoResolutionHeight,"720");
        }
	 }
    }
	else if(channel_id == 1){
	 if (pSysInfo->lan_config.nStreamType == 1){
	    sprintf(StreamingChannel.videoInputChannelID,"Dual");

		if(pSysInfo->lan_config.nFrameRate2 == 0)
		  sprintf(StreamingChannel.maxFrameRate,"30");
		else if(pSysInfo->lan_config.nFrameRate2 == 1)
		  sprintf(StreamingChannel.maxFrameRate,"15");
		else if(pSysInfo->lan_config.nFrameRate2 == 2)
		  sprintf(StreamingChannel.maxFrameRate,"8");

		if (pSysInfo->lan_config.nVideocombo == 0){
		  sprintf(StreamingChannel.id,"0");
		  sprintf(StreamingChannel.channelName,"MJPEG Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8555");
		  sprintf(StreamingChannel.videoCodecType,"MJPEG");
		  sprintf(StreamingChannel.fixedQuality,"%d",pSysInfo->lan_config.njpegquality);
		  sprintf(StreamingChannel.constantBitRate,"0");
		  sprintf(StreamingChannel.videoQualityControlType,"CBR");
		  sprintf(StreamingChannel.keyFrameInterval,"0");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"640");
	        sprintf(StreamingChannel.videoResolutionHeight,"352");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
		  sprintf(StreamingChannel.id,"0");
		  sprintf(StreamingChannel.channelName,"MJPEG Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8555");
		  sprintf(StreamingChannel.videoCodecType,"MJPEG");
		  sprintf(StreamingChannel.fixedQuality,"%d",pSysInfo->lan_config.njpegquality);
		  sprintf(StreamingChannel.constantBitRate,"0");
		  sprintf(StreamingChannel.videoQualityControlType,"CBR");
		  sprintf(StreamingChannel.keyFrameInterval,"0");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"640");
	        sprintf(StreamingChannel.videoResolutionHeight,"352");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 2){
		  sprintf(StreamingChannel.id,"2");
		  sprintf(StreamingChannel.channelName,"H264 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8556");
		  sprintf(StreamingChannel.videoCodecType,"H.264");
		  sprintf(StreamingChannel.fixedQuality,"0");
		  sprintf(StreamingChannel.constantBitRate,"%d",pSysInfo->lan_config.nMpeg42bitrate/1000);
		  sprintf(StreamingChannel.keyFrameInterval,"%d",pSysInfo->codec_advconfig[1].ipRatio);

		  if(pSysInfo->lan_config.nRateControl2 == 1)
			sprintf(StreamingChannel.videoQualityControlType,"VBR");
		  else if(pSysInfo->lan_config.nRateControl2 == 2)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");
		  else if(pSysInfo->lan_config.nRateControl2 == 0)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"320");
	        sprintf(StreamingChannel.videoResolutionHeight,"192");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 3){
		  sprintf(StreamingChannel.id,"1");
		  sprintf(StreamingChannel.channelName,"MPEG4 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8554");
		  sprintf(StreamingChannel.videoCodecType,"MPEG4");
		  sprintf(StreamingChannel.fixedQuality,"0");
		  sprintf(StreamingChannel.constantBitRate,"%d",pSysInfo->lan_config.nMpeg42bitrate/1000);
		  sprintf(StreamingChannel.keyFrameInterval,"%d",pSysInfo->codec_advconfig[1].ipRatio);

		  if(pSysInfo->lan_config.nRateControl2 == 1)
			sprintf(StreamingChannel.videoQualityControlType,"VBR");
		  else if(pSysInfo->lan_config.nRateControl2 == 2)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");
		  else if(pSysInfo->lan_config.nRateControl2 == 0)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
			sprintf(StreamingChannel.videoResolutionWidth,"320");
	        sprintf(StreamingChannel.videoResolutionHeight,"192");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
			sprintf(StreamingChannel.videoResolutionWidth,"720");
	        sprintf(StreamingChannel.videoResolutionHeight,"480");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 4){
		  sprintf(StreamingChannel.id,"1");
		  sprintf(StreamingChannel.channelName,"MPEG4 Stream");
		  sprintf(StreamingChannel.rtspPortNo,"8554");
		  sprintf(StreamingChannel.videoCodecType,"MPEG4");
		  sprintf(StreamingChannel.fixedQuality,"0");
		  sprintf(StreamingChannel.constantBitRate,"%d",pSysInfo->lan_config.nMpeg42bitrate/1000);
		  sprintf(StreamingChannel.keyFrameInterval,"%d",pSysInfo->codec_advconfig[1].ipRatio);

		  if(pSysInfo->lan_config.nRateControl2 == 1)
			sprintf(StreamingChannel.videoQualityControlType,"VBR");
		  else if(pSysInfo->lan_config.nRateControl2 == 2)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");
		  else if(pSysInfo->lan_config.nRateControl2 == 0)
			sprintf(StreamingChannel.videoQualityControlType,"CBR");

		  sprintf(StreamingChannel.videoResolutionWidth,"720");
	      sprintf(StreamingChannel.videoResolutionHeight,"480");
        }
	 }
	 else if (pSysInfo->lan_config.nStreamType == 2){
	    sprintf(StreamingChannel.videoInputChannelID,"Tri-Stream");
		sprintf(StreamingChannel.id,"0");
		sprintf(StreamingChannel.channelName,"MJPEG Stream");
		sprintf(StreamingChannel.rtspPortNo,"8555");
		sprintf(StreamingChannel.videoCodecType,"MJPEG");
	    sprintf(StreamingChannel.videoResolutionWidth,"640");
	    sprintf(StreamingChannel.videoResolutionHeight,"352");
		sprintf(StreamingChannel.fixedQuality,"%d",pSysInfo->lan_config.njpegquality);
		sprintf(StreamingChannel.constantBitRate,"0");
		sprintf(StreamingChannel.videoQualityControlType,"CBR");
		sprintf(StreamingChannel.keyFrameInterval,"0");

        if (pSysInfo->lan_config.nVideocombo == 0){

		  if(pSysInfo->lan_config.nFrameRate2 == 0)
		    sprintf(StreamingChannel.maxFrameRate,"30");
		  else if(pSysInfo->lan_config.nFrameRate2 == 1)
		    sprintf(StreamingChannel.maxFrameRate,"24");
		  else if(pSysInfo->lan_config.nFrameRate2 == 2)
		    sprintf(StreamingChannel.maxFrameRate,"15");
		  else if(pSysInfo->lan_config.nFrameRate2 == 3)
		    sprintf(StreamingChannel.maxFrameRate,"8");
        }
	    else if (pSysInfo->lan_config.nVideocombo == 1){

		  if(pSysInfo->lan_config.nFrameRate2 == 0)
		    sprintf(StreamingChannel.maxFrameRate,"30");
		  else if(pSysInfo->lan_config.nFrameRate2 == 1)
		    sprintf(StreamingChannel.maxFrameRate,"24");
		  else if(pSysInfo->lan_config.nFrameRate2 == 2)
		    sprintf(StreamingChannel.maxFrameRate,"15");
		  else if(pSysInfo->lan_config.nFrameRate2 == 3)
		    sprintf(StreamingChannel.maxFrameRate,"8");
        }
	 }
    }
	else if(channel_id == 2){
	  sprintf(StreamingChannel.videoInputChannelID,"Tri-Stream");
	  sprintf(StreamingChannel.keyFrameInterval,"%d",pSysInfo->codec_advconfig[2].ipRatio);
	  sprintf(StreamingChannel.fixedQuality,"0");
	  sprintf(StreamingChannel.constantBitRate,"%d",pSysInfo->lan_config.nMpeg42bitrate/1000);
	  sprintf(StreamingChannel.videoResolutionWidth,"320");
	  sprintf(StreamingChannel.videoResolutionHeight,"192");

	  if(pSysInfo->lan_config.nRateControl2 == 1)
		 sprintf(StreamingChannel.videoQualityControlType,"VBR");
	  else if(pSysInfo->lan_config.nRateControl2 == 2)
		 sprintf(StreamingChannel.videoQualityControlType,"CBR");
	  else if(pSysInfo->lan_config.nRateControl2 == 0)
		 sprintf(StreamingChannel.videoQualityControlType,"CBR");

	  if (pSysInfo->lan_config.nVideocombo == 0){
		 sprintf(StreamingChannel.id,"2");
		 sprintf(StreamingChannel.channelName,"H264 Stream");
		 sprintf(StreamingChannel.rtspPortNo,"8556");
		 sprintf(StreamingChannel.videoCodecType,"H.264");

		 if(pSysInfo->lan_config.nFrameRate3 == 0)
			sprintf(StreamingChannel.maxFrameRate,"15");
		 else if(pSysInfo->lan_config.nFrameRate3 == 1)
			sprintf(StreamingChannel.maxFrameRate,"12");
		 else if(pSysInfo->lan_config.nFrameRate3 == 2)
			sprintf(StreamingChannel.maxFrameRate,"8");
      }
	  else if (pSysInfo->lan_config.nVideocombo == 1){
		 sprintf(StreamingChannel.id,"1");
		 sprintf(StreamingChannel.channelName,"MPEG4 Stream");
		 sprintf(StreamingChannel.rtspPortNo,"8554");
		 sprintf(StreamingChannel.videoCodecType,"MPEG4");

		 if(pSysInfo->lan_config.nFrameRate3 == 0)
			sprintf(StreamingChannel.maxFrameRate,"30");
		 else if(pSysInfo->lan_config.nFrameRate3 == 1)
			sprintf(StreamingChannel.maxFrameRate,"15");
		 else if(pSysInfo->lan_config.nFrameRate3 == 2)
			sprintf(StreamingChannel.maxFrameRate,"8");
      }
    }

	if(pSysInfo->audio_config.codectype == 0 || pSysInfo->audio_config.codectype == 1)
	   sprintf(StreamingChannel.audioCompressionType,"G.711ulaw");

	if(pSysInfo->audio_config.bitrate == 0)
	   sprintf(StreamingChannel.audioBitRate,"64");
	else if(pSysInfo->audio_config.bitrate == 1)
	   sprintf(StreamingChannel.audioBitRate,"128");

	if(pSysInfo->audio_config.samplerate == 0)
	  sprintf(StreamingChannel.audioSamplingRate,"8");
	else if(pSysInfo->audio_config.samplerate == 1)
	  sprintf(StreamingChannel.audioSamplingRate,"16");

    if(pSysInfo->lan_config.mirror == 0)
	   sprintf(StreamingChannel.mirrorEnabled,"0");
	else
	   sprintf(StreamingChannel.mirrorEnabled,"1");

	xadd_stag_attr(req, TAG_StreamingChannel, RootAttrStr);
	xadd_elem(req, TAG_id, StreamingChannel.id);
	xadd_elem(req, TAG_channelName, StreamingChannel.channelName);
	xadd_elem(req, TAG_enabled, StreamingChannel.enabled);

	xadd_stag(req, TAG_Transport);
	xadd_elem(req, TAG_rtspPortNo,StreamingChannel.rtspPortNo);

	xadd_stag(req, TAG_ControlProtocolList);
	xadd_stag(req, TAG_ControlProtocol);
	sprintf(StreamingChannel.streamingTransport,"RTSP");
	xadd_elem(req, TAG_streamingTransport,StreamingChannel.streamingTransport);
	//sprintf(StreamingChannel.streamingTransport,"HTTP");
	//xadd_elem(req, TAG_streamingTransport,StreamingChannel.streamingTransport);
	xadd_etag(req, TAG_ControlProtocol);
	xadd_etag(req, TAG_ControlProtocolList);

	xadd_stag(req, TAG_Unicast);
	if(pSysInfo->lan_config.net.multicast_enable == 0)
	    xadd_elem(req, TAG_enabled, StreamingChannel.enabled);
	else if(pSysInfo->lan_config.net.multicast_enable == 1)
	    xadd_elem(req, TAG_enabled, StreamingChannel.disabled);
	xadd_elem(req, TAG_rtpTransportType,StreamingChannel.rtpTransportType);
	xadd_etag(req, TAG_Unicast);

	xadd_stag(req, TAG_Multicast);
	if(pSysInfo->lan_config.net.multicast_enable == 0)
	    xadd_elem(req, TAG_enabled, StreamingChannel.disabled);
	else if(pSysInfo->lan_config.net.multicast_enable == 1)
	    xadd_elem(req, TAG_enabled, StreamingChannel.enabled);
	xadd_etag(req, TAG_Multicast);

	xadd_stag(req, TAG_Security);
	xadd_elem(req, TAG_enabled, StreamingChannel.disabled);
	xadd_etag(req, TAG_Security);
	xadd_etag(req, TAG_Transport);

	xadd_stag(req, TAG_Video);
	xadd_elem(req, TAG_enabled, StreamingChannel.enabled);
	xadd_elem(req, TAG_videoInputChannelID, StreamingChannel.videoInputChannelID);
	xadd_elem(req, TAG_videoCodecType, StreamingChannel.videoCodecType);
	xadd_elem(req, TAG_videoScanType, StreamingChannel.videoScanType);
	xadd_elem(req, TAG_videoResolutionWidth, StreamingChannel.videoResolutionWidth);
	xadd_elem(req, TAG_videoResolutionHeight, StreamingChannel.videoResolutionHeight);
	xadd_elem(req, TAG_videoQualityControlType, StreamingChannel.videoQualityControlType);
	xadd_elem(req, TAG_constantBitRate, StreamingChannel.constantBitRate);
	xadd_elem(req, TAG_fixedQuality, StreamingChannel.fixedQuality);
	xadd_elem(req, TAG_maxFrameRate, StreamingChannel.maxFrameRate);
	xadd_elem(req, TAG_keyFrameInterval, StreamingChannel.keyFrameInterval);
	xadd_elem(req, TAG_mirrorEnabled, StreamingChannel.mirrorEnabled);
	xadd_etag(req, TAG_Video);

	xadd_stag(req, TAG_Audio);
	if(pSysInfo->audio_config.audioenable == 0)
	   xadd_elem(req, TAG_enabled, StreamingChannel.disabled);
	else
       xadd_elem(req, TAG_enabled, StreamingChannel.enabled);

	xadd_elem(req, TAG_audioInputChannelID, StreamingChannel.audioInputChannelID);
	xadd_elem(req, TAG_audioCompressionType, StreamingChannel.audioCompressionType);
    xadd_elem(req, TAG_audioBitRate, StreamingChannel.audioBitRate);
	xadd_elem(req, TAG_audioSamplingRate, StreamingChannel.audioSamplingRate);
	xadd_etag(req, TAG_Audio);

	xadd_etag(req, TAG_StreamingChannel);

	return STATUS_OK;
}

int update_streaming_channels(request *req, node_t *node)
{
	//char idstr[64];
	tag_t	*tag;
	unsigned long required;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_StreamingChannelList) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_StreamingChannel) == 0) {
			idflag = 1;
			update_streaming_channels_id(req, node, tag);
		    required |= xml_required_field(1);
			channel_id++;
		}
	 /* Protocol */
		tag = tag->next;
	}
	channel_id = 0;
	idflag = 0;
	/* check all the required fileds were present */
	//if(required != xml_required_mask(1))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int update_streaming_channels_id(request *req, node_t *node, tag_t *parentTag)
{
	SysInfo* pSysInfo = GetSysInfo();
	tag_t	*tag = parentTag;
	unsigned long required;
	StreamingChannel_psia StreamingChannel;
	__u8 value;
	int bitRateValue;
	__u32 ipRatioValue;
	char videoCodecCombo[20];

	fprintf(stderr,"\r\nUpdating Streaming Channels...\n");

	if(idflag == 0){
	// returns bytes processed or -1
	  if(xml_validate(req, req->inbuf, req->filesize) < 0)
		 return STATUS_INVALID_XML_FORMAT;

	  tag = req->root_tag;
	  if(!((tag) && (strcmp(tag->name, TAG_StreamingChannel) == 0)))
		  return STATUS_INVALID_XML_CONTENT;
	}
	fprintf(stderr,"\r\nInside update_streaming_channels_id with tag:%s\n",tag->name);
	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(StreamingChannel.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_channelName) == 0) {
			strcpy(StreamingChannel.channelName, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(StreamingChannel.enabled, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_Transport) == 0) {
			tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_rtspPortNo) == 0) {
			     strcpy(StreamingChannel.rtspPortNo, tag->value);
			     required |= xml_required_field(4);
			  }
			  else if(strcmp(tag->name, TAG_ControlProtocolList) == 0) {
			    tag = tag->first_child;
				if(strcmp(tag->name, TAG_ControlProtocol) == 0) {
			      tag = tag->first_child;
				  if(strcmp(tag->name, TAG_streamingTransport) == 0) {
					 strcpy(StreamingChannel.streamingTransport, tag->value);
			         required |= xml_required_field(5);
				  }
			    }
				tag = tag->parent;
				tag = tag->parent;
		      }
			  else if(strcmp(tag->name, TAG_Unicast) == 0) {
				tag = tag->first_child;
			    while(tag) {
				  if(strcmp(tag->name, TAG_enabled) == 0) {
					 strcpy(StreamingChannel.enabled, tag->value);
			         required |= xml_required_field(6);
				  }
				  else if(strcmp(tag->name, TAG_rtpTransportType) == 0) {
					 strcpy(StreamingChannel.rtpTransportType, tag->value);
			         required |= xml_required_field(7);
				  }
				  if(!tag->next)
				  break;
	              tag = tag->next;
				}
				tag = tag->parent;
			  }
			  else if(strcmp(tag->name, TAG_Multicast) == 0) {
				tag = tag->first_child;
				if(strcmp(tag->name, TAG_enabled) == 0) {
				  strcpy(StreamingChannel.enabled, tag->value);
			      required |= xml_required_field(8);
				  value = atoi(StreamingChannel.enabled);
				  ControlSystemData(SFIELD_SET_MULTICAST, (void *)&value, sizeof(value));
				}
				tag = tag->parent;
			  }
			  else if(strcmp(tag->name, TAG_Security) == 0) {
				tag = tag->first_child;
				if(strcmp(tag->name, TAG_enabled) == 0) {
				  strcpy(StreamingChannel.enabled, tag->value);
			      required |= xml_required_field(9);
				}
				tag = tag->parent;
			  }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
			tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_Video) == 0) {
			//fprintf(stderr,"\r\nInside TAG_Video\n");
		    tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_enabled) == 0) {
				//fprintf(stderr,"\r\nInside TAG_enabled\n");
				strcpy(StreamingChannel.enabled, tag->value);
			    required |= xml_required_field(10);
			  }
			  else if(strcmp(tag->name, TAG_videoInputChannelID) == 0) {
				//fprintf(stderr,"\r\nInside TAG_videoInputChannelID\n");
				strcpy(StreamingChannel.videoInputChannelID, tag->value);

				if(strncmp(StreamingChannel.videoInputChannelID,"Single",4) == 0)
                   strcpy(StreamingChannel.videoInputChannelID,"0");
				else if(strncmp(StreamingChannel.videoInputChannelID,"Dual",4) == 0)
                   strcpy(StreamingChannel.videoInputChannelID,"1");
				else if(strncmp(StreamingChannel.videoInputChannelID,"Tri-Stream",4) == 0)
                   strcpy(StreamingChannel.videoInputChannelID,"2");

			    required |= xml_required_field(11);
				value = atoi(StreamingChannel.videoInputChannelID);
				ControlSystemData(SFIELD_SET_VIDEO_MODE, (void *)&value, sizeof(value));
			  }
			  else if(strcmp(tag->name, TAG_videoCodecType) == 0) {
				//fprintf(stderr,"\r\nInside TAG_videoCodecType\n");
			    strcpy(StreamingChannel.videoCodecType, tag->value);

				if(strncmp(StreamingChannel.videoCodecType,"H.264",3) == 0)
                   strcpy(videoCodecCombo,"0");
				else if(strncmp(StreamingChannel.videoCodecType,"MPEG4",3) == 0)
                   strcpy(videoCodecCombo,"1");
				else if(strncmp(StreamingChannel.videoCodecType,"MegaPixel",3) == 0)
                   strcpy(videoCodecCombo,"2");
				else if(strncmp(StreamingChannel.videoCodecType,"H.264 + JPEG",10) == 0)
                   strcpy(videoCodecCombo,"0");
				else if(strncmp(StreamingChannel.videoCodecType,"MPEG4 + JPEG",10) == 0)
                   strcpy(videoCodecCombo,"1");
				else if(strncmp(StreamingChannel.videoCodecType,"Dual H.264",8) == 0)
                   strcpy(videoCodecCombo,"2");
				else if((strncmp(StreamingChannel.videoCodecType,"Dual MPEG4",8) == 0) && (strncmp(StreamingChannel.videoCodecType,"Dual MPEG4 + JPEG",16) != 0))
                   strcpy(videoCodecCombo,"3");
				else if(strncmp(StreamingChannel.videoCodecType,"H264 + MPEG4",10) == 0)
                   strcpy(videoCodecCombo,"4");
				else if(strncmp(StreamingChannel.videoCodecType,"Dual H264 + JPEG",16) == 0)
                   strcpy(videoCodecCombo,"0");
				else if(strncmp(StreamingChannel.videoCodecType,"Dual MPEG4 + JPEG",16) == 0)
                   strcpy(videoCodecCombo,"1");

			    required |= xml_required_field(12);
				value = atoi(videoCodecCombo);
				ControlSystemData(SFIELD_SET_VIDEOCODECCOMBO, (void *)&value, sizeof(value));
		      }
			  else if(strcmp(tag->name, TAG_videoScanType) == 0) {
			    strcpy(StreamingChannel.videoScanType, tag->value);
			    required |= xml_required_field(13);
		      }
			  else if(strcmp(tag->name, TAG_videoResolutionWidth) == 0) {
				//fprintf(stderr,"\r\nInside TAG_videoResolutionWidth with StreamingChannel.videoCodecType:%s\n",StreamingChannel.videoCodecType);
			    strcpy(StreamingChannel.videoResolutionWidth, tag->value);

				if((strncmp(StreamingChannel.videoCodecType,"H.264 + JPEG",10) == 0) || (strncmp(StreamingChannel.videoCodecType,"MPEG4 + JPEG",10) == 0)){
				  if(strncmp(StreamingChannel.videoResolutionWidth,"1280",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"640",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"720",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"1");
			    }
				else if((strncmp(StreamingChannel.videoCodecType,"H.264",3) == 0) || (strncmp(StreamingChannel.videoCodecType,"MPEG4",3) == 0)){
				  if(strncmp(StreamingChannel.videoResolutionWidth,"1280",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"720",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"1");
				  //else if(strncmp(StreamingChannel.videoResolutionWidth,"1280",3) == 0)
                     //strcpy(StreamingChannel.videoResolutionWidth,"2");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"1920",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"3");

			    }
				else if((strncmp(StreamingChannel.videoCodecType,"MegaPixel",3) == 0)){
				  if(strncmp(StreamingChannel.videoResolutionWidth,"1600",4) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"2048",4) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"1");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"2592",4) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"2");
			    }
				else if((strncmp(StreamingChannel.videoCodecType,"Dual H264 + JPEG",16) == 0) || (strncmp(StreamingChannel.videoCodecType,"Dual MPEG4 + JPEG",16) == 0)){
				  if(strncmp(StreamingChannel.videoResolutionWidth,"1280",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"640",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"320",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
			    }
				else if((strncmp(StreamingChannel.videoCodecType,"Dual H.264",8) == 0) || (strncmp(StreamingChannel.videoCodecType,"Dual MPEG4",8) == 0)){
				  if(strncmp(StreamingChannel.videoResolutionWidth,"1280",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"320",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"720",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"1");
				  else if(strncmp(StreamingChannel.videoResolutionWidth,"1920",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"3");
			    }
				else if((strncmp(StreamingChannel.videoCodecType,"H264 + MPEG4",10) == 0)){
				  if(strncmp(StreamingChannel.videoResolutionWidth,"720",3) == 0)
                     strcpy(StreamingChannel.videoResolutionWidth,"0");
			    }

			    required |= xml_required_field(14);
				value = atoi(StreamingChannel.videoResolutionWidth);
				ControlSystemData(SFIELD_SET_VIDEOCODECRES, (void *)&value, sizeof(value));
		      }
			  else if(strcmp(tag->name, TAG_videoResolutionHeight) == 0) {
				//fprintf(stderr,"\r\nInside TAG_videoResolutionHeight\n");
			    strcpy(StreamingChannel.videoResolutionHeight, tag->value);

				if((strncmp(StreamingChannel.videoCodecType,"H.264 + JPEG",10) == 0) || (strncmp(StreamingChannel.videoCodecType,"MPEG4 + JPEG",10) == 0)){
				  if(strncmp(StreamingChannel.videoResolutionHeight,"720",3) == 0)
                     strcpy(StreamingChannel.videoResolutionHeight,"2");
			    }
				else if((strncmp(StreamingChannel.videoCodecType,"H.264",3) == 0) || (strncmp(StreamingChannel.videoCodecType,"MPEG4",3) == 0)){
				  if(strncmp(StreamingChannel.videoResolutionHeight,"960",3) == 0){
                    strcpy(StreamingChannel.videoResolutionHeight,"2");
				    value = atoi(StreamingChannel.videoResolutionHeight);
				    ControlSystemData(SFIELD_SET_VIDEOCODECRES, (void *)&value, sizeof(value));
			      }
			    }
				required |= xml_required_field(15);
		      }
			  else if(strcmp(tag->name, TAG_videoQualityControlType) == 0) {
				//fprintf(stderr,"\r\nInside TAG_videoQualityControlType\n");
			    strcpy(StreamingChannel.videoQualityControlType, tag->value);

                if(strncmp(StreamingChannel.videoQualityControlType,"OFF",3) == 0)
                   strcpy(StreamingChannel.videoQualityControlType,"0");
				else if(strncmp(StreamingChannel.videoQualityControlType,"VBR",3) == 0)
                   strcpy(StreamingChannel.videoQualityControlType,"1");
				else if(strncmp(StreamingChannel.videoQualityControlType,"CBR",3) == 0)
                   strcpy(StreamingChannel.videoQualityControlType,"2");

			    required |= xml_required_field(16);
				value = atoi(StreamingChannel.videoQualityControlType);

				if(channel_id ==0){
				  if(strcmp(StreamingChannel.id,"0") != 0)
					ControlSystemData(SFIELD_SET_RATE_CONTROL1, (void *)&value, sizeof(value));
				}
			    else if(channel_id == 1){
			      if(strcmp(StreamingChannel.id,"0") != 0)
					 ControlSystemData(SFIELD_SET_RATE_CONTROL2, (void *)&value, sizeof(value));
				}
			    else if(channel_id == 2)
				   ControlSystemData(SFIELD_SET_RATE_CONTROL2, (void *)&value, sizeof(value));
		      }
			  else if(strcmp(tag->name, TAG_constantBitRate) == 0) {
			    strcpy(StreamingChannel.constantBitRate, tag->value);
			    required |= xml_required_field(17);
				bitRateValue = atoi(StreamingChannel.constantBitRate);
				bitRateValue *= 1000;

				if(channel_id == 0){
				  if(strcmp(StreamingChannel.id,"0") != 0){
				     ControlSystemData(SFIELD_SET_MPEG41_BITRATE, (void *)&bitRateValue, sizeof(bitRateValue));
				   }
				}
			    else if(channel_id == 1){
			       if(strcmp(StreamingChannel.id,"0") != 0){
					 ControlSystemData(SFIELD_SET_MPEG42_BITRATE, (void *)&bitRateValue, sizeof(bitRateValue));
				   }
				}
			    else if(channel_id == 2){
					ControlSystemData(SFIELD_SET_MPEG42_BITRATE, (void *)&bitRateValue, sizeof(bitRateValue));
				}
		      }
			  else if(strcmp(tag->name, TAG_fixedQuality) == 0) {
				//fprintf(stderr,"\r\nInside TAG_fixedQuality\n");
			    strcpy(StreamingChannel.fixedQuality, tag->value);
			    required |= xml_required_field(18);
				value = atoi(StreamingChannel.fixedQuality);

				if(StreamingChannel.id == 0)
				    ControlSystemData(SFIELD_SET_JQUALITY, (void *)&value, sizeof(value));
		      }
			  else if(strcmp(tag->name, TAG_maxFrameRate) == 0) {
				//fprintf(stderr,"\r\nInside TAG_maxFrameRate\n");
			    strcpy(StreamingChannel.maxFrameRate, tag->value);

				if(strncmp(StreamingChannel.maxFrameRate,"30",2) == 0)
                   strcpy(StreamingChannel.maxFrameRate,"0");
				else if(strncmp(StreamingChannel.maxFrameRate,"21",2) == 0)
                   strcpy(StreamingChannel.maxFrameRate,"0");
				else if(strncmp(StreamingChannel.maxFrameRate,"24",2) == 0)
                   strcpy(StreamingChannel.maxFrameRate,"1");
				else if(strncmp(StreamingChannel.maxFrameRate,"15",2) == 0)
                   strcpy(StreamingChannel.maxFrameRate,"2");
				else if(strncmp(StreamingChannel.maxFrameRate,"8",2) == 0)
                   strcpy(StreamingChannel.maxFrameRate,"3");

			    required |= xml_required_field(19);
				value = atoi(StreamingChannel.maxFrameRate);

				if(channel_id ==0){
					 ControlSystemData(SFIELD_SET_FRAMERATE1, (void *)&value, sizeof(value));
				}
			    else if(channel_id == 1){
					 ControlSystemData(SFIELD_SET_FRAMERATE2, (void *)&value, sizeof(value));
				}
			    else if(channel_id == 2)
					 ControlSystemData(SFIELD_SET_FRAMERATE3, (void *)&value, sizeof(value));
		      }
			  else if(strcmp(tag->name, TAG_keyFrameInterval) == 0) {
			    strcpy(StreamingChannel.keyFrameInterval, tag->value);
			    required |= xml_required_field(2);
			    ipRatioValue = atoi(StreamingChannel.keyFrameInterval);

			    if(channel_id ==0)
			       ControlSystemData(SFIELD_SET_IPRATIO1, &ipRatioValue, sizeof(ipRatioValue));
			    else if(channel_id == 1)
			       ControlSystemData(SFIELD_SET_IPRATIO2, &ipRatioValue, sizeof(ipRatioValue));
			    else if(channel_id == 2)
			       ControlSystemData(SFIELD_SET_IPRATIO3, &ipRatioValue, sizeof(ipRatioValue));
		      }
		      else if(strcmp(tag->name, TAG_mirrorEnabled) == 0) {
			    strcpy(StreamingChannel.mirrorEnabled, tag->value);

				/*if(strncmp(StreamingChannel.mirrorEnabled,"30",2) == 0)
                   strcpy(StreamingChannel.mirrorEnabled,"0");
				else if(strncmp(StreamingChannel.mirrorEnabled,"21",2) == 0)
                   strcpy(StreamingChannel.mirrorEnabled,"0");
                */
			    required |= xml_required_field(21);
				value = atoi(StreamingChannel.mirrorEnabled);
				ControlSystemData(SFIELD_SET_MIRROR, (void *)&value, sizeof(value));
		      }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
		    tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_Audio) == 0) {
		    tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_enabled) == 0) {
			    strcpy(StreamingChannel.audioEnabled, tag->value);
			    required |= xml_required_field(23);
				value = atoi(StreamingChannel.audioEnabled);
				ControlSystemData(SFIELD_SET_AUDIOENABLE, (void *)&value, sizeof(value));
				ControlSystemData(SFIELD_SET_AUDIOMODE, (void *)&pSysInfo->audio_config.audiomode, sizeof(pSysInfo->audio_config.audiomode));
		      }
		      else if(strcmp(tag->name, TAG_audioInputChannelID) == 0) {
			    strcpy(StreamingChannel.audioInputChannelID, tag->value);
			    required |= xml_required_field(24);
		      }
		      else if(strcmp(tag->name, TAG_audioCompressionType) == 0) {
			    strcpy(StreamingChannel.audioCompressionType, tag->value);

				if(strncmp(StreamingChannel.audioCompressionType,"G.711",4) == 0)
                  strcpy(StreamingChannel.audioCompressionType,"0");

			    required |= xml_required_field(25);
				value = atoi(StreamingChannel.audioCompressionType);
				ControlSystemData(SFIELD_SET_AUDIO_ENCODE, (void *)&value, sizeof(value));
		      }
		      else if(strcmp(tag->name, TAG_audioBitRate) == 0) {
				//fprintf(stderr,"\r\nInside TAG_audioBitRate\n");
			    strcpy(StreamingChannel.audioBitRate, tag->value);

				if(strncmp(StreamingChannel.audioBitRate,"64",2) == 0)
                  strcpy(StreamingChannel.audioBitRate,"0");
				else
                  strcpy(StreamingChannel.audioBitRate,"1");

			    required |= xml_required_field(26);
				value = atoi(StreamingChannel.audioBitRate);
				ControlSystemData(SFIELD_SET_AUDIO_BITRATE, (void *)&value, sizeof(value));
		      }
		      else if(strcmp(tag->name, TAG_audioSamplingRate) == 0) {
				//fprintf(stderr,"\r\nInside TAG_audioSamplingRate\n");
			    strcpy(StreamingChannel.audioSamplingRate, tag->value);

				if(strncmp(StreamingChannel.audioSamplingRate,"8",1) == 0)
                  strcpy(StreamingChannel.audioSamplingRate,"0");
				else
                  strcpy(StreamingChannel.audioSamplingRate,"1");

			    required |= xml_required_field(27);
				value = atoi(StreamingChannel.audioSamplingRate);
				ControlSystemData(SFIELD_SET_AUDIO_SAMPLERATE, (void *)&value, sizeof(value));
		      }
			  if(!tag->next)
				  break;
			  tag = tag->next;
			}
			tag = tag->parent;
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(27))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int create_streaming_channels(request *req, node_t *node)
{
	return STATUS_OK;
	//return STATUS_INVALID_OPERATION;
}

int delete_streaming_channels(request *req, node_t *node)
{
	return STATUS_OK;
	//return STATUS_INVALID_OPERATION;
}

int delete_streaming_channels_id(request *req, node_t *node)
{
	return STATUS_OK;
	//return STATUS_INVALID_OPERATION;
}

int retrieve_custom_video_advanced_channels(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	int c,channel_count = 0;
	char idstr[64];

	xadd_stag_attr(req, TAG_VideoAdvancedSettingList, RootAttrStr);

	if(pSysInfo->lan_config.nStreamType == 0)
		channel_count = 1;
	else if (pSysInfo->lan_config.nStreamType == 1)
	    channel_count = 2;
	else if (pSysInfo->lan_config.nStreamType == 2)
	    channel_count = 3;

	for(c = 0; c < channel_count; c++) {
		get_channel_url(c, idstr);
		retrieve_custom_video_advanced_channels_id(req, node);
		channel_id ++;
	}
	xadd_etag(req, TAG_VideoAdvancedSettingList);
	channel_id = 0;

	return STATUS_OK;
}

int retrieve_custom_video_advanced_channels_id(request *req, node_t *node)
{
    VideoAdvancedSetting_psia VideoAdvancedSetting;
	SysInfo* pSysInfo = GetSysInfo();

	if(channel_id == 0){
	  if(pSysInfo->lan_config.nStreamType == 0){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(VideoAdvancedSetting.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(VideoAdvancedSetting.id,"1");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(VideoAdvancedSetting.id,"0");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 1){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(VideoAdvancedSetting.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(VideoAdvancedSetting.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(VideoAdvancedSetting.id,"2");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 3){
	       sprintf(VideoAdvancedSetting.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 4){
	       sprintf(VideoAdvancedSetting.id,"2");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 2){
	      if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(VideoAdvancedSetting.id,"2");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(VideoAdvancedSetting.id,"1");
	     }
	  }

	  sprintf(VideoAdvancedSetting.forceIFrame,"%d",pSysInfo->codec_advconfig[0].fIframe);
	  sprintf(VideoAdvancedSetting.qpInit,"%d",pSysInfo->codec_advconfig[0].qpInit);
	  sprintf(VideoAdvancedSetting.qpMin,"%d",pSysInfo->codec_advconfig[0].qpMin);
	  sprintf(VideoAdvancedSetting.qpMax,"%d",pSysInfo->codec_advconfig[0].qpMax);

	  if(pSysInfo->codec_advconfig[0].meConfig == 0)
		  sprintf(VideoAdvancedSetting.meConfig,"AUTO");

	  sprintf(VideoAdvancedSetting.packetSize,"%d",pSysInfo->codec_advconfig[0].packetSize);
	  sprintf(VideoAdvancedSetting.ROIEnable,"%d",pSysInfo->codec_roiconfig[0].numROI);

	  sprintf(VideoAdvancedSetting.positionX1,"%d",pSysInfo->codec_roiconfig[0].roi[0].startx);
	  sprintf(VideoAdvancedSetting.positionY1,"%d",pSysInfo->codec_roiconfig[0].roi[0].starty);
	  sprintf(VideoAdvancedSetting.width1,"%d",pSysInfo->codec_roiconfig[0].roi[0].width);
	  sprintf(VideoAdvancedSetting.height1,"%d",pSysInfo->codec_roiconfig[0].roi[0].height);

	  sprintf(VideoAdvancedSetting.positionX2,"%d",pSysInfo->codec_roiconfig[0].roi[1].startx);
	  sprintf(VideoAdvancedSetting.positionY2,"%d",pSysInfo->codec_roiconfig[0].roi[1].starty);
	  sprintf(VideoAdvancedSetting.width2,"%d",pSysInfo->codec_roiconfig[0].roi[1].width);
	  sprintf(VideoAdvancedSetting.height2,"%d",pSysInfo->codec_roiconfig[0].roi[1].height);

	  sprintf(VideoAdvancedSetting.positionX3,"%d",pSysInfo->codec_roiconfig[0].roi[2].startx);
	  sprintf(VideoAdvancedSetting.positionY3,"%d",pSysInfo->codec_roiconfig[0].roi[2].starty);
	  sprintf(VideoAdvancedSetting.width3,"%d",pSysInfo->codec_roiconfig[0].roi[2].width);
	  sprintf(VideoAdvancedSetting.height3,"%d",pSysInfo->codec_roiconfig[0].roi[2].height);
	}
	else if(channel_id == 1){
	  if(pSysInfo->lan_config.nStreamType == 1){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(VideoAdvancedSetting.id,"0");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(VideoAdvancedSetting.id,"0");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 2){
	       sprintf(VideoAdvancedSetting.id,"2");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 3){
	       sprintf(VideoAdvancedSetting.id,"1");
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 4){
	       sprintf(VideoAdvancedSetting.id,"1");
	     }
	  }
	  else if(pSysInfo->lan_config.nStreamType == 2){
	     if(pSysInfo->lan_config.nVideocombo == 0){
	       sprintf(VideoAdvancedSetting.id,"0");
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       sprintf(VideoAdvancedSetting.id,"0");
	     }
	  }

	  sprintf(VideoAdvancedSetting.forceIFrame,"%d",pSysInfo->codec_advconfig[1].fIframe);
	  sprintf(VideoAdvancedSetting.qpInit,"%d",pSysInfo->codec_advconfig[1].qpInit);
	  sprintf(VideoAdvancedSetting.qpMin,"%d",pSysInfo->codec_advconfig[1].qpMin);
	  sprintf(VideoAdvancedSetting.qpMax,"%d",pSysInfo->codec_advconfig[1].qpMax);

	  if(pSysInfo->codec_advconfig[1].meConfig == 0)
		  sprintf(VideoAdvancedSetting.meConfig,"AUTO");

	  sprintf(VideoAdvancedSetting.packetSize,"%d",pSysInfo->codec_advconfig[1].packetSize);
	  sprintf(VideoAdvancedSetting.ROIEnable,"%d",pSysInfo->codec_roiconfig[1].numROI);

	  sprintf(VideoAdvancedSetting.positionX1,"%d",pSysInfo->codec_roiconfig[1].roi[0].startx);
	  sprintf(VideoAdvancedSetting.positionY1,"%d",pSysInfo->codec_roiconfig[1].roi[0].starty);
	  sprintf(VideoAdvancedSetting.width1,"%d",pSysInfo->codec_roiconfig[1].roi[0].width);
	  sprintf(VideoAdvancedSetting.height1,"%d",pSysInfo->codec_roiconfig[1].roi[0].height);

	  sprintf(VideoAdvancedSetting.positionX2,"%d",pSysInfo->codec_roiconfig[1].roi[1].startx);
	  sprintf(VideoAdvancedSetting.positionY2,"%d",pSysInfo->codec_roiconfig[1].roi[1].starty);
	  sprintf(VideoAdvancedSetting.width2,"%d",pSysInfo->codec_roiconfig[1].roi[1].width);
	  sprintf(VideoAdvancedSetting.height2,"%d",pSysInfo->codec_roiconfig[1].roi[1].height);

	  sprintf(VideoAdvancedSetting.positionX3,"%d",pSysInfo->codec_roiconfig[1].roi[2].startx);
	  sprintf(VideoAdvancedSetting.positionY3,"%d",pSysInfo->codec_roiconfig[1].roi[2].starty);
	  sprintf(VideoAdvancedSetting.width3,"%d",pSysInfo->codec_roiconfig[1].roi[2].width);
	  sprintf(VideoAdvancedSetting.height3,"%d",pSysInfo->codec_roiconfig[1].roi[2].height);

	}
	else if(channel_id == 2){
	  if(pSysInfo->lan_config.nVideocombo == 0){
	     sprintf(VideoAdvancedSetting.id,"2");
	  }
	  else if(pSysInfo->lan_config.nVideocombo == 1){
	     sprintf(VideoAdvancedSetting.id,"1");
	  }

	  sprintf(VideoAdvancedSetting.forceIFrame,"%d",pSysInfo->codec_advconfig[2].fIframe);
	  sprintf(VideoAdvancedSetting.qpInit,"%d",pSysInfo->codec_advconfig[2].qpInit);
	  sprintf(VideoAdvancedSetting.qpMin,"%d",pSysInfo->codec_advconfig[2].qpMin);
	  sprintf(VideoAdvancedSetting.qpMax,"%d",pSysInfo->codec_advconfig[2].qpMax);

	  if(pSysInfo->codec_advconfig[2].meConfig == 0)
		  sprintf(VideoAdvancedSetting.meConfig,"AUTO");

	  sprintf(VideoAdvancedSetting.packetSize,"%d",pSysInfo->codec_advconfig[2].packetSize);
	  sprintf(VideoAdvancedSetting.ROIEnable,"%d",pSysInfo->codec_roiconfig[2].numROI);

	  sprintf(VideoAdvancedSetting.positionX1,"%d",pSysInfo->codec_roiconfig[2].roi[0].startx);
	  sprintf(VideoAdvancedSetting.positionY1,"%d",pSysInfo->codec_roiconfig[2].roi[0].starty);
	  sprintf(VideoAdvancedSetting.width1,"%d",pSysInfo->codec_roiconfig[2].roi[0].width);
	  sprintf(VideoAdvancedSetting.height1,"%d",pSysInfo->codec_roiconfig[2].roi[0].height);

	  sprintf(VideoAdvancedSetting.positionX2,"%d",pSysInfo->codec_roiconfig[2].roi[1].startx);
	  sprintf(VideoAdvancedSetting.positionY2,"%d",pSysInfo->codec_roiconfig[2].roi[1].starty);
	  sprintf(VideoAdvancedSetting.width2,"%d",pSysInfo->codec_roiconfig[2].roi[1].width);
	  sprintf(VideoAdvancedSetting.height2,"%d",pSysInfo->codec_roiconfig[2].roi[1].height);

	  sprintf(VideoAdvancedSetting.positionX3,"%d",pSysInfo->codec_roiconfig[2].roi[2].startx);
	  sprintf(VideoAdvancedSetting.positionY3,"%d",pSysInfo->codec_roiconfig[2].roi[2].starty);
	  sprintf(VideoAdvancedSetting.width3,"%d",pSysInfo->codec_roiconfig[2].roi[2].width);
	  sprintf(VideoAdvancedSetting.height3,"%d",pSysInfo->codec_roiconfig[2].roi[2].height);
	}

	xadd_stag_attr(req, TAG_VideoAdvancedSetting, RootAttrStr);
	xadd_elem(req, TAG_id, VideoAdvancedSetting.id);
	xadd_elem(req, TAG_forceIFrame, VideoAdvancedSetting.forceIFrame);
	xadd_elem(req, TAG_qpInit, VideoAdvancedSetting.qpInit);
	xadd_elem(req, TAG_qpMin, VideoAdvancedSetting.qpMin);
	xadd_elem(req, TAG_qpMax, VideoAdvancedSetting.qpMax);
	xadd_elem(req, TAG_meConfig, VideoAdvancedSetting.meConfig);
	xadd_elem(req, TAG_packetSize, VideoAdvancedSetting.packetSize);
	xadd_elem(req, TAG_ROIEnable, VideoAdvancedSetting.ROIEnable);
	xadd_stag(req, TAG_RegionCoordinatesList);

	xadd_stag(req, TAG_RegionCoordinates);
	xadd_elem(req, TAG_positionX, VideoAdvancedSetting.positionX1);
	xadd_elem(req, TAG_positionY, VideoAdvancedSetting.positionY1);
	xadd_elem(req, TAG_width, VideoAdvancedSetting.width1);
	xadd_elem(req, TAG_height, VideoAdvancedSetting.height1);
	xadd_etag(req, TAG_RegionCoordinates);

	xadd_stag(req, TAG_RegionCoordinates);
	xadd_elem(req, TAG_positionX, VideoAdvancedSetting.positionX2);
	xadd_elem(req, TAG_positionY, VideoAdvancedSetting.positionY2);
	xadd_elem(req, TAG_width, VideoAdvancedSetting.width2);
	xadd_elem(req, TAG_height, VideoAdvancedSetting.height2);
	xadd_etag(req, TAG_RegionCoordinates);

	xadd_stag(req, TAG_RegionCoordinates);
	xadd_elem(req, TAG_positionX, VideoAdvancedSetting.positionX3);
	xadd_elem(req, TAG_positionY, VideoAdvancedSetting.positionY3);
	xadd_elem(req, TAG_width, VideoAdvancedSetting.width3);
	xadd_elem(req, TAG_height, VideoAdvancedSetting.height3);
	xadd_etag(req, TAG_RegionCoordinates);


	xadd_etag(req, TAG_RegionCoordinatesList);
	xadd_etag(req, TAG_VideoAdvancedSetting);

	return STATUS_OK;
}

int update_custom_video_advanced_channels(request *req, node_t *node)
{
	//int c;
	//char idstr[64];
	tag_t	*tag;
	unsigned long required;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_VideoAdvancedSettingList) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_VideoAdvancedSetting) == 0) {
			idflag = 1;
			update_custom_video_advanced_channels_id(req, node, tag);
		    required |= xml_required_field(1);
			channel_id++;
		}
	 /* Protocol */
		tag = tag->next;
	}
	channel_id = 0;
	idflag = 0;
	/* check all the required fileds were present */
	//if(required != xml_required_mask(1))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}


int update_custom_video_advanced_channels_id(request *req, node_t *node, tag_t *parentTag)
{
	//int ret;
	tag_t	*tag = parentTag;
	unsigned long required;
	int regionCount = 0;
	VideoAdvancedSetting_psia VideoAdvancedSetting;
	__u8 value;
	unsigned int valueX1,valueX2,valueX3;
	unsigned int valueY1,valueY2,valueY3;
	unsigned int valueW1,valueW2,valueW3;
	unsigned int valueH1,valueH2,valueH3;

	fprintf(stderr,"\r\nUpdating Streaming Channels...\n");

	if(idflag == 0){
	  // returns bytes processed or -1
	  if(xml_validate(req, req->inbuf, req->filesize) < 0)
		  return STATUS_INVALID_XML_FORMAT;

	  tag = req->root_tag;
	  if(!((tag) && (strcmp(tag->name, TAG_VideoAdvancedSetting) == 0)))
		  return STATUS_INVALID_XML_CONTENT;
    }

	fprintf(stderr,"\r\nInside update_custom_video_advanced_channels_id with tag:%s\n",tag->name);
	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(VideoAdvancedSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_forceIFrame) == 0) {
			//fprintf(stderr,"\r\nInside TAG_forceIFrame with channel_id:%d\n",channel_id);
			strcpy(VideoAdvancedSetting.forceIFrame, tag->value);
			required |= xml_required_field(2);
			value = atoi(VideoAdvancedSetting.forceIFrame);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_FORCE_IFRAME1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_FORCE_IFRAME2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_FORCE_IFRAME3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_qpInit) == 0) {
			//fprintf(stderr,"\r\nInside TAG_qpInit with channel_id:%d\n",channel_id);
			strcpy(VideoAdvancedSetting.qpInit, tag->value);
			required |= xml_required_field(3);
			value = atoi(VideoAdvancedSetting.qpInit);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_QPINIT1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_QPINIT2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_QPINIT3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_qpMin) == 0) {
			strcpy(VideoAdvancedSetting.qpMin, tag->value);
			required |= xml_required_field(4);
			value = atoi(VideoAdvancedSetting.qpMin);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_QPMIN1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_QPMIN2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_QPMIN3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_qpMax) == 0) {
			strcpy(VideoAdvancedSetting.qpMax, tag->value);
			required |= xml_required_field(5);
			value = atoi(VideoAdvancedSetting.qpMax);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_QPMAX1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_QPMAX2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_QPMAX3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_meConfig) == 0) {
			//fprintf(stderr,"\r\nInside TAG_meConfig with channel_id:%d\n",channel_id);
			strcpy(VideoAdvancedSetting.meConfig, tag->value);

			if(strncmp(VideoAdvancedSetting.meConfig,"AUTO",4) == 0)
                strcpy(VideoAdvancedSetting.meConfig,"0");

			required |= xml_required_field(6);
			value = atoi(VideoAdvancedSetting.meConfig);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_MECONFIG1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_MECONFIG2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_MECONFIG3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_packetSize) == 0) {
			//fprintf(stderr,"\r\nInside TAG_packetSize with channel_id:%d\n",channel_id);
			strcpy(VideoAdvancedSetting.packetSize, tag->value);
			required |= xml_required_field(7);
			value = atoi(VideoAdvancedSetting.packetSize);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_PACKETSIZE1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_PACKETSIZE2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_PACKETSIZE3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_ROIEnable) == 0) {
			//fprintf(stderr,"\r\nInside TAG_ROIEnable with channel_id:%d\n",channel_id);
			strcpy(VideoAdvancedSetting.ROIEnable, tag->value);
			required |= xml_required_field(8);
			value = atoi(VideoAdvancedSetting.ROIEnable);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_ROI_ENABLE1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_ROI_ENABLE2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_ROI_ENABLE3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_RegionCoordinatesList) == 0) {
            tag = tag->first_child;
			while(tag) {
				if(strcmp(tag->name, TAG_RegionCoordinates) == 0) {
				  regionCount++;
                  tag = tag->first_child;
				  while(tag) {
					  if(strcmp(tag->name, TAG_positionX) == 0) {
						 //fprintf(stderr,"\r\nInside TAG_positionX with regionCount:%d\n",regionCount);
						 if(regionCount == 1)
                           strcpy(VideoAdvancedSetting.positionX1, tag->value);
						 else if(regionCount == 2)
                           strcpy(VideoAdvancedSetting.positionX2, tag->value);
						 else if(regionCount == 3)
                           strcpy(VideoAdvancedSetting.positionX3, tag->value);

			             required |= xml_required_field(9);
						 valueX1 = atoi(VideoAdvancedSetting.positionX1);
						 valueX2 = atoi(VideoAdvancedSetting.positionX2);
						 valueX3 = atoi(VideoAdvancedSetting.positionX3);
					  }
					  else if(strcmp(tag->name, TAG_positionY) == 0) {
						 //fprintf(stderr,"\r\nInside TAG_positionY with regionCount:%d\n",regionCount);
                         if(regionCount == 1)
                           strcpy(VideoAdvancedSetting.positionY1, tag->value);
						 else if(regionCount == 2)
                           strcpy(VideoAdvancedSetting.positionY2, tag->value);
						 else if(regionCount == 3)
                           strcpy(VideoAdvancedSetting.positionY3, tag->value);

			             required |= xml_required_field(10);
						 valueY1 = atoi(VideoAdvancedSetting.positionY1);
						 valueY2 = atoi(VideoAdvancedSetting.positionY2);
						 valueY3 = atoi(VideoAdvancedSetting.positionY3);
					  }
					  else if(strcmp(tag->name, TAG_width) == 0) {
						 //fprintf(stderr,"\r\nInside TAG_width with regionCount:%d\n",regionCount);
                         if(regionCount == 1)
                           strcpy(VideoAdvancedSetting.width1, tag->value);
						 else if(regionCount == 2)
                           strcpy(VideoAdvancedSetting.width2, tag->value);
						 else if(regionCount == 3)
                           strcpy(VideoAdvancedSetting.width3, tag->value);

			             required |= xml_required_field(11);
						 valueW1 = atoi(VideoAdvancedSetting.width1);
						 valueW2 = atoi(VideoAdvancedSetting.width2);
						 valueW3 = atoi(VideoAdvancedSetting.width3);
					  }
					  else if(strcmp(tag->name, TAG_height) == 0) {
						 //fprintf(stderr,"\r\nInside TAG_height with regionCount:%d\n",regionCount);
                         if(regionCount == 1)
                           strcpy(VideoAdvancedSetting.height1, tag->value);
						 else if(regionCount == 2)
                           strcpy(VideoAdvancedSetting.height2, tag->value);
						 else if(regionCount == 3)
                           strcpy(VideoAdvancedSetting.height3, tag->value);

			             required |= xml_required_field(12);
						 valueH1 = atoi(VideoAdvancedSetting.height1);
						 valueH2 = atoi(VideoAdvancedSetting.height2);
						 valueH3 = atoi(VideoAdvancedSetting.height3);
					  }
					  if(!tag->next)
				         break;
				      tag = tag->next;
				  }
				  tag = tag->parent;
                }
				if(!tag->next)
				   break;
                tag = tag->next;
			}
            tag = tag->parent;

			if(channel_id ==0){
			  //fprintf(stderr,"\r\nInside TAG_RegionCoordinates with channel_id:%d\n",channel_id);
			  ControlSystemData(SFIELD_SET_STR1X1, &valueX1, sizeof(valueX1));
			  ControlSystemData(SFIELD_SET_STR1Y1, &valueY1, sizeof(valueY1));
			  ControlSystemData(SFIELD_SET_STR1W1, &valueW1, sizeof(valueW1));
			  ControlSystemData(SFIELD_SET_STR1H1, &valueH1, sizeof(valueH1));

			  ControlSystemData(SFIELD_SET_STR1X2, &valueX2, sizeof(valueX2));
			  ControlSystemData(SFIELD_SET_STR1Y2, &valueY2, sizeof(valueY2));
			  ControlSystemData(SFIELD_SET_STR1W2, &valueW2, sizeof(valueW2));
			  ControlSystemData(SFIELD_SET_STR1H2, &valueH2, sizeof(valueH2));

			  ControlSystemData(SFIELD_SET_STR1X3, &valueX3, sizeof(valueX3));
			  ControlSystemData(SFIELD_SET_STR1Y3, &valueY3, sizeof(valueY3));
			  ControlSystemData(SFIELD_SET_STR1W3, &valueW3, sizeof(valueW3));
			  ControlSystemData(SFIELD_SET_STR1H3, &valueH3, sizeof(valueH3));
			}
			else if(channel_id == 1){
			  //fprintf(stderr,"\r\nInside TAG_RegionCoordinates with channel_id:%d\n",channel_id);
			  ControlSystemData(SFIELD_SET_STR2X1, &valueX1, sizeof(valueX1));
			  ControlSystemData(SFIELD_SET_STR2Y1, &valueY1, sizeof(valueY1));
			  ControlSystemData(SFIELD_SET_STR2W1, &valueW1, sizeof(valueW1));
			  ControlSystemData(SFIELD_SET_STR2H1, &valueH1, sizeof(valueH1));

			  ControlSystemData(SFIELD_SET_STR2X2, &valueX2, sizeof(valueX2));
			  ControlSystemData(SFIELD_SET_STR2Y2, &valueY2, sizeof(valueY2));
			  ControlSystemData(SFIELD_SET_STR2W2, &valueW2, sizeof(valueW2));
			  ControlSystemData(SFIELD_SET_STR2H2, &valueH2, sizeof(valueH2));

			  ControlSystemData(SFIELD_SET_STR2X3, &valueX3, sizeof(valueX3));
			  ControlSystemData(SFIELD_SET_STR2Y3, &valueY3, sizeof(valueY3));
			  ControlSystemData(SFIELD_SET_STR2W3, &valueW3, sizeof(valueW3));
			  ControlSystemData(SFIELD_SET_STR2H3, &valueH3, sizeof(valueH3));
			}
			else if(channel_id == 2){
			  //fprintf(stderr,"\r\nInside TAG_RegionCoordinates with channel_id:%d\n",channel_id);
			  ControlSystemData(SFIELD_SET_STR3X1, &valueX1, sizeof(valueX1));
			  ControlSystemData(SFIELD_SET_STR3Y1, &valueY1, sizeof(valueY1));
			  ControlSystemData(SFIELD_SET_STR3W1, &valueW1, sizeof(valueW1));
			  ControlSystemData(SFIELD_SET_STR3H1, &valueH1, sizeof(valueH1));

			  ControlSystemData(SFIELD_SET_STR3X2, &valueX2, sizeof(valueX2));
			  ControlSystemData(SFIELD_SET_STR3Y2, &valueY2, sizeof(valueY2));
			  ControlSystemData(SFIELD_SET_STR3W2, &valueW2, sizeof(valueW2));
			  ControlSystemData(SFIELD_SET_STR3H2, &valueH2, sizeof(valueH2));

			  ControlSystemData(SFIELD_SET_STR3X3, &valueX3, sizeof(valueX3));
			  ControlSystemData(SFIELD_SET_STR3Y3, &valueY3, sizeof(valueY3));
			  ControlSystemData(SFIELD_SET_STR3W3, &valueW3, sizeof(valueW3));
			  ControlSystemData(SFIELD_SET_STR3H3, &valueH3, sizeof(valueH3));
			}
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(12))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}
/*
int delete_streaming_channels_id_advanced(request *req, node_t *node)
{
	return STATUS_OK;
}
*/
int retrieve_custom_video_advanced_channels_id_capabilities(request *req, node_t *node)
{
	const char *meConfigStr = "opt=\"AUTO\"";

	xadd_stag_attr(req, TAG_VideoAdvancedSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_meConfig, meConfigStr);
	xadd_etag(req, TAG_meConfig);
	xadd_etag(req, TAG_VideoAdvancedSetting);

	return STATUS_OK;
}

int retrieve_streaming_channels_id_status(request *req, node_t *node)
{
	StreamingSessionStatus_psia StreamingSessionStatus;
	SysInfo* pSysInfo = GetSysInfo();
	NET_IPV4 ip;
	ip.int32 = net_get_ifaddr(ETH_NAME);
	int i,channel_count = 0;

	if(pSysInfo->lan_config.nStreamType == 0)
		channel_count = 1;
	else if (pSysInfo->lan_config.nStreamType == 1)
	    channel_count = 2;
	else if (pSysInfo->lan_config.nStreamType == 2)
	    channel_count = 3;

	sprintf(StreamingSessionStatus.ipAddress, "%03d.%03d.%03d.%03d", ip.str[0], ip.str[1], ip.str[2], ip.str[3]);
	sprintf(StreamingSessionStatus.clientUserName,"NA");
	sprintf(StreamingSessionStatus.startDateTime,"NA");
	sprintf(StreamingSessionStatus.elapsedTime,"NA");
	sprintf(StreamingSessionStatus.bandwidth,"NA");

	xadd_stag_attr(req, TAG_StreamingSessionStatusList, RootAttrStr);

	for(i=0; i< channel_count; i++)
	{
	  xadd_stag_attr(req, TAG_StreamingSessionStatus, RootAttrStr);
	  xadd_stag(req, TAG_clientAddress);
	  xadd_elem(req, TAG_ipAddress,StreamingSessionStatus.ipAddress);
	  xadd_etag(req, TAG_clientAddress);
	//xadd_elem(req, TAG_clientUserName,StreamingSessionStatus.clientUserName);
	//xadd_elem(req, TAG_startDateTime, StreamingSessionStatus.startDateTime);
	//xadd_elem(req, TAG_elapsedTime, StreamingSessionStatus.elapsedTime);
	//xadd_elem(req, TAG_bandwidth, StreamingSessionStatus.bandwidth);
	  xadd_etag(req, TAG_StreamingSessionStatus);
	}
	xadd_etag(req, TAG_StreamingSessionStatusList);

	return STATUS_OK;
}

int retrieve_streaming_channels_id_http(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(strstr(req->request_uri, "0")){
	     uri_mjpeg(req);
    }
	else if(strstr(req->request_uri, "1")){
		if(pSysInfo->lan_config.nVideocodecmode == 9)
		   uri_mpeg4cif(req);
		else
		   uri_mpeg4(req);
	}
	else if(strstr(req->request_uri, "2")){
		if(pSysInfo->lan_config.nVideocodecmode == 8)
		   uri_avccif(req);
		else
		   uri_avc(req);
    }
	return STATUS_OK;
}

int retrieve_streaming_channels_id_picture(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_streaming_channels_id_http_capabilities(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();

	const char *streamingTransportStr = "opt=\"HTTP,RTSP\"";
	const char *videoInputChannelIDStr = "opt=\"Single,Dual,Tri-Stream\"";
	const char *videoCodecTypeStr = NULL;
	const char *videoResolutionWidthStr = NULL;
	const char *videoResolutionHeightStr = NULL;
	const char *videoQualityControlTypeStr = "opt=\"OFF,VBR,CBR\"";
	const char *maxFrameRateStr = NULL;
	const char *audioCompressionTypeStr = "opt=\"G.711ulaw\"";
	const char *audioBitRateStr = "opt=\"64,128\"";
	const char *audioSamplingRateStr = "opt=\"8,16\"";

	if(pSysInfo->lan_config.nStreamType == 0){
        videoCodecTypeStr = "opt=\"H.264,MPEG4,MegaPixel\"";
	    if(pSysInfo->lan_config.nVideocombo == 0){
	       videoResolutionWidthStr = "opt=\"H264:720,H264:D1,H264:SXVGA,H264:1080\"";
           videoResolutionHeightStr = "opt=\"1280X720,720X480,1280X960,1920X1072\"";
		   if(pSysInfo->lan_config.nVideocodecres == 0)
              maxFrameRateStr = "opt=\"30,24,15,8\"";
		   else if(pSysInfo->lan_config.nVideocodecres == 1)
	          maxFrameRateStr = "opt=\"30,24,15,8\"";
		   else if(pSysInfo->lan_config.nVideocodecres == 2)
	          maxFrameRateStr = "opt=\"30,24,15,8\"";
		   else if(pSysInfo->lan_config.nVideocodecres == 3)
	          maxFrameRateStr = "opt=\"23,15,8\"";
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       videoResolutionWidthStr = "opt=\"MPEG4:720,MPEG4:D1,MPEG4:SXVGA,MPEG4:1080\"";
		   videoResolutionHeightStr = "opt=\"1280X720,720X480,1280X960,1920X1072\"";
		   if(pSysInfo->lan_config.nVideocodecres == 0)
              maxFrameRateStr = "opt=\"30,24,15,8\"";
		   else if(pSysInfo->lan_config.nVideocodecres == 1)
	          maxFrameRateStr = "opt=\"30,24,15,8\"";
		   else if(pSysInfo->lan_config.nVideocodecres == 2)
	          maxFrameRateStr = "opt=\"30,24,15,8\"";
		   else if(pSysInfo->lan_config.nVideocodecres == 3)
	          maxFrameRateStr = "opt=\"23,15,8\"";
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 2){
	       videoResolutionWidthStr = "opt=\"JPG:2MP(1600X1200),JPG:3MP(2048X1536),JPG:5MP(2592x1920)\"";
		   videoResolutionHeightStr = "opt=\"1600X1200,2048X1536,2592x1920\"";
		   if(pSysInfo->lan_config.nVideocodecres == 0)
              maxFrameRateStr = "\"opt=24,15,8\"";
		   else if(pSysInfo->lan_config.nVideocodecres == 1)
	          maxFrameRateStr = "\"opt=15,12,8\"";
		   else if(pSysInfo->lan_config.nVideocodecres == 2)
	          maxFrameRateStr = "\"opt=Auto\"";
	     }
	}
	else if(pSysInfo->lan_config.nStreamType == 1){
		videoCodecTypeStr = "opt=\"H.264 + JPEG,MPEG4 + JPEG,Dual H.264,Dual MPEG4,H264 + MPEG4\"";
		maxFrameRateStr = "opt=\"30,24,15,8;30,24,15,8\"";
	    if(pSysInfo->lan_config.nVideocombo == 0){
	       videoResolutionWidthStr = "opt=\"H264:720 JPEG:VGA,H264:D1 JPEG:D1,H264:720 JPEG:720\"";
		   videoResolutionHeightStr = "opt=\"1280X720,640X352;720X480,720X480;1280X720,1280X720\"";
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       videoResolutionWidthStr = "opt=\"MPEG4:720 JPEG:VGA,MPEG4:D1 JPEG:D1,MPEG4:720 JPEG:720\"";
		    videoResolutionHeightStr = "opt=\"1280X720,640X352;720X480,720X480;1280X720,1280X720\"";
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 2){
	       videoResolutionWidthStr = "opt=\"H264:720 H264:QVGA,H264:D1 H264:D1,H264:D1 H264:QVGA,H264:1080 H264:QVGA\"";
		    videoResolutionHeightStr = "opt=\"1280X720,320X192;720X480,720X480;720X480,320X192;1920X1072,320X192\"";
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 3){
	       videoResolutionWidthStr = "opt=\"MPEG4:720 MPEG4:QVGA,MPEG4:D1 MPEG4:D1,MPEG4:D1 MPEG4:QVGA,MPEG4:1080 MPEG4:QVGA\"";
		   videoResolutionHeightStr = "opt=\"1280X720,320X192;720X480,720X480;720X480,320X192;1920X1072,320X192\"";
	     }
		 else if(pSysInfo->lan_config.nVideocombo == 4){
	       videoResolutionWidthStr = "opt=\"H264:D1 MPEG4:D1\"";
		   videoResolutionHeightStr = "opt=\"720X480,720X480\"";
	     }
	}
	else if(pSysInfo->lan_config.nStreamType == 2){
		videoCodecTypeStr = "opt=\"Dual H264 + JPEG,Dual MPEG4 + JPEG\"";
	    if(pSysInfo->lan_config.nVideocombo == 0){
	       videoResolutionWidthStr = "opt=\"H264:720 JPEG:VGA H264:QVGA\"";
		   videoResolutionHeightStr = "opt=\"1280X720,640X352,320X192\"";
		   maxFrameRateStr = "opt=\"30,24,15,8;30,24,15,8;30,24,15,8\"";
	     }
	     else if(pSysInfo->lan_config.nVideocombo == 1){
	       videoResolutionWidthStr = "opt=\"MPEG4:720 JPEG:VGA MPEG4:QVGA\"";
		   videoResolutionHeightStr = "opt=\"1280X720,640X352,320X192\"";
		   maxFrameRateStr = "opt=\"30,24,15,8;30,24,15,8;30,24,15,8\"";
	     }
	}

	xadd_stag_attr(req, TAG_StreamingChannel, RootAttrStr);
	xadd_stag(req, TAG_ControlProtocolList);
	xadd_stag(req, TAG_ControlProtocol);
	xadd_stag_attr(req, TAG_streamingTransport, streamingTransportStr);
	xadd_etag(req, TAG_streamingTransport);
	xadd_etag(req, TAG_ControlProtocol);
	xadd_etag(req, TAG_ControlProtocolList);
    xadd_stag(req, TAG_Video);
	xadd_stag_attr(req, TAG_videoInputChannelID, videoInputChannelIDStr);
	xadd_etag(req, TAG_videoInputChannelID);
	xadd_stag_attr(req, TAG_videoCodecType, videoCodecTypeStr);
	xadd_etag(req, TAG_videoCodecType);
	xadd_stag_attr(req, TAG_videoResolutionWidth, videoResolutionWidthStr);
	xadd_etag(req, TAG_videoResolutionWidth);
	xadd_stag_attr(req, TAG_videoResolutionHeight, videoResolutionHeightStr);
	xadd_etag(req, TAG_videoResolutionHeight);
	xadd_stag_attr(req, TAG_videoQualityControlType, videoQualityControlTypeStr);
	xadd_etag(req, TAG_videoQualityControlType);
	xadd_stag_attr(req, TAG_maxFrameRate, maxFrameRateStr);
	xadd_etag(req, TAG_maxFrameRate);
	xadd_etag(req, TAG_Video);
	xadd_stag(req, TAG_Audio);
	xadd_stag_attr(req, TAG_audioCompressionType, audioCompressionTypeStr);
	xadd_etag(req, TAG_audioCompressionType);
    xadd_stag_attr(req, TAG_audioBitRate, audioBitRateStr);
	xadd_etag(req, TAG_audioBitRate);
	xadd_stag_attr(req, TAG_audioSamplingRate, audioSamplingRateStr);
	xadd_etag(req, TAG_audioSamplingRate);
	xadd_etag(req, TAG_Audio);
    xadd_etag(req, TAG_StreamingChannel);

	return STATUS_OK;
}

int retrieve_streaming_channels_id_picture_capabilities(request *req, node_t *node)
{
	return STATUS_OK;
}

/*******************************************************************/
/*                            System/IO                            */
/*******************************************************************/
int retrieve_system_io(request *req, node_t *node)
{
	xadd_stag_attr(req, TAG_IOPortList, RootAttrStr);

	retrieve_system_io_inputs(req, node);
	retrieve_system_io_outputs(req, node);

	xadd_etag(req, TAG_IOPortList);

	return STATUS_OK;
}

int retrieve_system_io_inputs(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_IOInputPortList, RootAttrStr);
	for(n = 0; n < get_port_count(); n++)
		retrieve_system_io_inputs_id(req, node);
	xadd_etag(req, TAG_IOInputPortList);

	return STATUS_OK;
}

int retrieve_system_io_inputs_id(request *req, node_t *node)
{
	IOInputPort_psia IOInputPort;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(IOInputPort.id,"0");

	if(pSysInfo->lan_config.giointype == 0)
        sprintf(IOInputPort.triggering,"LOW");
	else if(pSysInfo->lan_config.giointype == 1)
        sprintf(IOInputPort.triggering,"HIGH");

	xadd_stag_attr(req, TAG_IOInputPort, RootAttrStr);
	xadd_elem(req, TAG_id, IOInputPort.id);
	xadd_elem(req, TAG_triggering, IOInputPort.triggering);
	xadd_etag(req, TAG_IOInputPort);

	return STATUS_OK;
}

int update_system_io_inputs_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	IOInputPort_psia IOInputPort;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_IOInputPort) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(IOInputPort.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_triggering) == 0) {
			strcpy(IOInputPort.triggering, tag->value);

			if(strncmp(IOInputPort.triggering,"LOW",3) == 0)
               strcpy(IOInputPort.triggering,"0");
			else
               strcpy(IOInputPort.triggering,"1");

			required |= xml_required_field(2);
			value = atoi(IOInputPort.triggering);
			ControlSystemData(SFIELD_SET_GIOINTYPE, &value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fileds were present */
	//if(required != xml_required_mask(2))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_io_inputs_id_capabilities(request *req, node_t *node)
{
	const char *triggeringStr = "opt=\"LOW,HIGH\"";

	xadd_stag_attr(req, TAG_IOInputPort, RootAttrStr);
	xadd_stag_attr(req, TAG_triggering, triggeringStr);
	xadd_etag(req, TAG_triggering);
	xadd_etag(req, TAG_IOInputPort);

	return STATUS_OK;
}

int retrieve_system_io_outputs(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_IOOutputPortList, RootAttrStr);
	for(n = 0; n < get_port_count(); n++)
		retrieve_system_io_outputs_id(req, node);
	xadd_etag(req, TAG_IOOutputPortList);

	return STATUS_OK;
}

int retrieve_system_io_outputs_id(request *req, node_t *node)
{
	IOOutputPort_psia IOOutputPort;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(IOOutputPort.id,"0");
	sprintf(IOOutputPort.defaultState,"LOW");

	if(pSysInfo->lan_config.gioouttype == 0)
        sprintf(IOOutputPort.outputState,"LOW");
	else if(pSysInfo->lan_config.gioouttype == 1)
        sprintf(IOOutputPort.outputState,"HIGH");


	xadd_stag_attr(req, TAG_IOOutputPort, RootAttrStr);
	xadd_elem(req, TAG_id, IOOutputPort.id);
	xadd_stag(req, TAG_PowerOnState);
	xadd_elem(req, TAG_defaultState, IOOutputPort.defaultState);
	xadd_elem(req, TAG_outputState, IOOutputPort.outputState);
	xadd_etag(req, TAG_PowerOnState);
	xadd_etag(req, TAG_IOOutputPort);

	return STATUS_OK;
}

int update_system_io_outputs_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	IOOutputPort_psia IOOutputPort;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_IOOutputPort) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(IOOutputPort.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_PowerOnState) == 0) {
			tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_defaultState) == 0) {
			    strcpy(IOOutputPort.defaultState, tag->value);
			    required |= xml_required_field(2);
		      }
			  else if(strcmp(tag->name, TAG_outputState) == 0) {
			    strcpy(IOOutputPort.outputState, tag->value);

			   if(strncmp(IOOutputPort.outputState,"LOW",3) == 0)
                 strcpy(IOOutputPort.outputState,"0");
			   else
                 strcpy(IOOutputPort.outputState,"1");

			   required |= xml_required_field(3);
			   value = atoi(IOOutputPort.outputState);
			   ControlSystemData(SFIELD_SET_GIOOUTTYPE, &value, sizeof(value));
		      }
			  if(!tag->next)
				break;
			  tag = tag->next;
	        }
			tag = tag->parent;
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fileds were present */
	//if(required != xml_required_mask(3))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_io_outputs_id_capabilities(request *req, node_t *node)
{
	const char *outputStateStr = "opt=\"LOW,HIGH\"";

	xadd_stag_attr(req, TAG_IOOutputPort, RootAttrStr);
	xadd_stag(req, TAG_PowerOnState);
	xadd_stag_attr(req, TAG_outputState, outputStateStr);
	xadd_etag(req, TAG_outputState);
	xadd_etag(req, TAG_PowerOnState);
	xadd_etag(req, TAG_IOOutputPort);

	return STATUS_OK;
}

int get_port_count(void)
{
	return 1;
}

/*******************************************************************/
/*                            System/Serial                        */
/*******************************************************************/
int retrieve_system_serial_ports(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_SerialPortList, RootAttrStr);
	for(n = 0; n < get_port_count(); n++)
		retrieve_system_serial_ports_id(req, node);
	xadd_etag(req, TAG_SerialPortList);

	return STATUS_OK;
}

int retrieve_system_serial_ports_id(request *req, node_t *node)
{
	SerialPort_psia SerialPort;
	//SysInfo* pSysInfo = GetSysInfo();

	sprintf(SerialPort.id,"0");
	sprintf(SerialPort.enabled,"1");
	sprintf(SerialPort.serialPortType,"RS232");
	sprintf(SerialPort.duplexMode,"full");
	sprintf(SerialPort.direction,"bdirectional");
	sprintf(SerialPort.baudRate,"115200");
	sprintf(SerialPort.dataBits,"8");
	sprintf(SerialPort.parityType,"None");
	sprintf(SerialPort.stopBits,"1");

	xadd_stag_attr(req, TAG_SerialPort, RootAttrStr);
	xadd_elem(req, TAG_id, SerialPort.id);
	xadd_elem(req, TAG_enabled, SerialPort.enabled);
	xadd_elem(req, TAG_serialPortType, SerialPort.serialPortType);
	xadd_elem(req, TAG_duplexMode, SerialPort.duplexMode);
	xadd_elem(req, TAG_direction, SerialPort.direction);
	xadd_elem(req, TAG_baudRate, SerialPort.baudRate);
	xadd_elem(req, TAG_dataBits, SerialPort.dataBits);
	xadd_elem(req, TAG_parityType, SerialPort.parityType);
	xadd_elem(req, TAG_stopBits, SerialPort.stopBits);
	xadd_etag(req, TAG_SerialPort);

	return STATUS_OK;
}

int update_system_serial_ports_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	SerialPort_psia SerialPort;
	//__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_SerialPort) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(SerialPort.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(SerialPort.enabled, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_serialPortType) == 0) {
			strcpy(SerialPort.serialPortType, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_duplexMode) == 0) {
			strcpy(SerialPort.duplexMode, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_direction) == 0) {
			strcpy(SerialPort.direction, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_baudRate) == 0) {
			strcpy(SerialPort.baudRate, tag->value);
			required |= xml_required_field(6);
		}
        else if(strcmp(tag->name, TAG_dataBits) == 0) {
			strcpy(SerialPort.dataBits, tag->value);
			required |= xml_required_field(7);
		}
		else if(strcmp(tag->name, TAG_parityType) == 0) {
			strcpy(SerialPort.parityType, tag->value);
			required |= xml_required_field(8);
		}
		else if(strcmp(tag->name, TAG_stopBits) == 0) {
			strcpy(SerialPort.stopBits, tag->value);
			required |= xml_required_field(9);
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fileds were present */
	//if(required != xml_required_mask(9))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_serial_ports_id_capabilities(request *req, node_t *node)
{
	return STATUS_OK;
}

/*******************************************************************/
/*                            Custom/Event                         */
/*******************************************************************/
int get_event_count(void)
{
	return 1;
}

int retrieve_process_custom_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Event");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"MotionDetection");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Analytics");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Live");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"User");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Video");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Camera");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Audio");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"DateTime");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Network");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Alarm");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Storage");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Reload");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"Maintenance");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);

	retrieve_standard_resource(req, node);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_process_custom_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"Custom");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "Custom Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);
/*
	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);
*/
	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

int retrieve_custom_event(request *req, node_t *node)
{
	xadd_stag_attr(req, TAG_EventNotification, RootAttrStr);

	retrieve_custom_event_triggers(req, node);
	retrieve_custom_event_schedule(req, node);
	retrieve_custom_event_notification(req, node);

	xadd_etag(req, TAG_EventNotification);

	return STATUS_OK;
}

int update_custom_event(request *req, node_t *node)
{
	update_custom_event_triggers(req, node);
	update_custom_event_schedule(req, node);
	update_custom_event_notification(req, node);

	return STATUS_OK;
}

int retrieve_custom_event_triggers(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_EventTriggerList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_triggers_id(req, node);
	xadd_etag(req, TAG_EventTriggerList);

	return STATUS_OK;
}

int update_custom_event_triggers(request *req, node_t *node)
{
	int n;

	for(n = 0; n < get_event_count(); n++)
		update_custom_event_triggers_id(req, node);

	return STATUS_OK;
}

int create_custom_event_triggers(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int delete_custom_event_triggers(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_triggers_id(request *req, node_t *node)
{
	EventTrigger_psia EventTrigger;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(EventTrigger.id,"0");
	sprintf(EventTrigger.inputIOPortID,"NA");

	xadd_stag_attr(req, TAG_EventTrigger, RootAttrStr);
	xadd_elem(req, TAG_id, EventTrigger.id);

	if(pSysInfo->motion_config.motionenable == 1){
       sprintf(EventTrigger.eventType,"Motion Detection");
	   xadd_elem(req, TAG_eventType, EventTrigger.eventType);
	}
	if(pSysInfo->lan_config.lostalarm == 1){
       sprintf(EventTrigger.eventType,"Ethernet Lost");
	   xadd_elem(req, TAG_eventType, EventTrigger.eventType);
	}
	if(pSysInfo->audio_config.alarmON == 1){
       sprintf(EventTrigger.eventType,"Audio Alarm");
	   xadd_elem(req, TAG_eventType, EventTrigger.eventType);
    }
	if(pSysInfo->lan_config.nExtAlarm == 1){
       sprintf(EventTrigger.eventType,"External Triggers");
	   xadd_elem(req, TAG_eventType, EventTrigger.eventType);
	}
	if(pSysInfo->motion_config.motionenable == 0 && pSysInfo->lan_config.lostalarm == 0 && pSysInfo->lan_config.nExtAlarm == 0){
       sprintf(EventTrigger.eventType,"No Event");
	   xadd_elem(req, TAG_eventType, EventTrigger.eventType);
	}
	if(pSysInfo->motion_config.motionenable == 1){
       sprintf(EventTrigger.eventDescription,"Motion Alarm");
	   xadd_elem(req, TAG_eventDescription, EventTrigger.eventDescription);
	}
	if(pSysInfo->lan_config.lostalarm == 1){
       sprintf(EventTrigger.eventDescription,"Ethernet Lost ALarm");
	   xadd_elem(req, TAG_eventDescription, EventTrigger.eventDescription);
	}
	if(pSysInfo->audio_config.alarmON == 1){
      sprintf(EventTrigger.eventDescription,"High Audio Alarm");
      xadd_elem(req, TAG_eventDescription, EventTrigger.eventDescription);
	}
	if(pSysInfo->lan_config.nExtAlarm == 1){
      sprintf(EventTrigger.eventDescription,"External Trigger Alarm");
	  xadd_elem(req, TAG_eventDescription, EventTrigger.eventDescription);
    }
	if(pSysInfo->motion_config.motionenable == 0 && pSysInfo->lan_config.lostalarm == 0 && pSysInfo->lan_config.nExtAlarm == 0){
       sprintf(EventTrigger.eventDescription,"No Event");
	   xadd_elem(req, TAG_eventDescription, EventTrigger.eventDescription);
    }

	xadd_elem(req, TAG_inputIOPortID, EventTrigger.inputIOPortID);
	retrieve_custom_event_triggers_id_notifications(req,node);
	xadd_etag(req, TAG_EventTrigger);

	return STATUS_OK;
}

int update_custom_event_triggers_id(request *req, node_t *node)
{
	//int ret;
	tag_t	*tag;
	unsigned long required;
	EventTrigger_psia EventTrigger;
	//__u8 value;
	char bEnable = 1,bDisable = 0;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_EventTrigger) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(EventTrigger.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_eventType) == 0) {
			strcpy(EventTrigger.eventType, tag->value);
			required |= xml_required_field(2);

			if(strncmp(EventTrigger.eventType,"Motion",6) == 0)
				ControlSystemData(SFIELD_SET_MOTIONENABLE, &bEnable, sizeof(bEnable));
			else if(strncmp(EventTrigger.eventType,"Ethernet",8) == 0)
				ControlSystemData(SFIELD_SET_LOSTALARM, &bEnable, sizeof(bEnable));
			else if(strncmp(EventTrigger.eventType,"External",8) == 0)
				ControlSystemData(SFIELD_SET_EXT_ALARM, &bEnable, sizeof(bEnable));
			else if(strncmp(EventTrigger.eventType,"No Event",8) == 0){
				ControlSystemData(SFIELD_SET_MOTIONENABLE, &bDisable, sizeof(bDisable));
			    ControlSystemData(SFIELD_SET_LOSTALARM, &bDisable, sizeof(bDisable));
				ControlSystemData(SFIELD_SET_EXT_ALARM, &bDisable, sizeof(bDisable));
			}
		}
		else if(strcmp(tag->name, TAG_eventDescription) == 0) {
			strcpy(EventTrigger.eventDescription, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_inputIOPortID) == 0) {
			strcpy(EventTrigger.inputIOPortID, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_EventTriggerNotificationList) == 0) {
			update_custom_event_triggers_id_notifications(req,node);
			required |= xml_required_field(5);
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(5))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_triggers_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_triggers_id_capabilities(request *req, node_t *node)
{
	const char *eventTypeStr = "opt=\"Motion Detection,Ethernet Lost,External Triggers\"";

	xadd_stag_attr(req, TAG_EventTrigger, RootAttrStr);
	xadd_stag_attr(req, TAG_eventType, eventTypeStr);
	xadd_etag(req, TAG_eventType);
	xadd_etag(req, TAG_EventTrigger);

	return STATUS_OK;
}

int retrieve_custom_event_triggers_id_notifications(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_EventTriggerNotificationList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_triggers_id_notifications_id(req, node);
	xadd_etag(req, TAG_EventTriggerNotificationList);

	return STATUS_OK;
}

int update_custom_event_triggers_id_notifications(request *req, node_t *node)
{
	int n;

	for(n = 0; n < get_event_count(); n++)
		update_custom_event_triggers_id_notifications_id(req, node);

	return STATUS_OK;
}

int create_custom_event_triggers_id_notifications(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int delete_custom_event_triggers_id_notifications(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_triggers_id_notifications_id(request *req, node_t *node)
{
	EventTriggerNotification_psia EventTriggerNotification;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(EventTriggerNotification.id,"0");
	sprintf(EventTriggerNotification.notificationRecurrence,"NA");

	if(pSysInfo->lan_config.nAlarmDuration == 0)
        sprintf(EventTriggerNotification.notificationInterval,"10 seconds");
	else if(pSysInfo->lan_config.nAlarmDuration == 1)
        sprintf(EventTriggerNotification.notificationInterval,"30 seconds");
	else if(pSysInfo->lan_config.nAlarmDuration == 2)
        sprintf(EventTriggerNotification.notificationInterval,"1 minute");
	else if(pSysInfo->lan_config.nAlarmDuration == 3)
        sprintf(EventTriggerNotification.notificationInterval,"5 minutes");
	else if(pSysInfo->lan_config.nAlarmDuration == 4)
        sprintf(EventTriggerNotification.notificationInterval,"10 minutes");
	else if(pSysInfo->lan_config.nAlarmDuration == 5)
        sprintf(EventTriggerNotification.notificationInterval,"Non-Stop");


	xadd_stag_attr(req, TAG_EventTriggerNotification, RootAttrStr);
	xadd_elem(req, TAG_id, EventTriggerNotification.id);

	if(pSysInfo->lan_config.bAFtpEnable == 1){
       sprintf(EventTriggerNotification.notificationMethod,"FTP");
	   xadd_elem(req, TAG_notificationMethod, EventTriggerNotification.notificationMethod);
	}
	if(pSysInfo->lan_config.bASmtpEnable == 1){
       sprintf(EventTriggerNotification.notificationMethod,"SMTP");
	   xadd_elem(req, TAG_notificationMethod, EventTriggerNotification.notificationMethod);
	}
	if(pSysInfo->lan_config.bSdaEnable == 1){
       sprintf(EventTriggerNotification.notificationMethod,"Local Storage");
	   xadd_elem(req, TAG_notificationMethod, EventTriggerNotification.notificationMethod);
    }
	if((pSysInfo->lan_config.bAFtpEnable == 0) && (pSysInfo->lan_config.bSdaEnable == 0) && (pSysInfo->lan_config.bASmtpEnable == 0)){
       sprintf(EventTriggerNotification.notificationMethod,"No Notifications");
	   xadd_elem(req, TAG_notificationMethod, EventTriggerNotification.notificationMethod);
	}

	xadd_elem(req, TAG_notificationRecurrence, EventTriggerNotification.notificationRecurrence);
	xadd_elem(req, TAG_notificationInterval, EventTriggerNotification.notificationInterval);
	xadd_etag(req, TAG_EventTriggerNotification);

	return STATUS_OK;
}

int update_custom_event_triggers_id_notifications_id(request *req, node_t *node)
{
	//int ret;
	tag_t	*tag;
	unsigned long required;
	EventTriggerNotification_psia EventTriggerNotification;
	__u8 value;
	char bEnable = 1;
	char bDisable = 0;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_EventTriggerNotification) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(EventTriggerNotification.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_notificationMethod) == 0) {
			strcpy(EventTriggerNotification.notificationMethod, tag->value);
			required |= xml_required_field(2);

			if(strncmp(EventTriggerNotification.notificationMethod,"FTP",3) == 0)
				ControlSystemData(SFIELD_SET_AFTPENABLE, &bEnable, sizeof(bEnable));
			else if(strncmp(EventTriggerNotification.notificationMethod,"SMTP",4) == 0)
				ControlSystemData(SFIELD_SET_ASMTPENABLE, &bEnable, sizeof(bEnable));
			else if(strncmp(EventTriggerNotification.notificationMethod,"Local",5) == 0)
				ControlSystemData(SFIELD_SET_SDAENABLE, &bEnable, sizeof(bEnable));
			else if(strncmp(EventTriggerNotification.notificationMethod,"No Notifications",5) == 0){
				ControlSystemData(SFIELD_SET_AFTPENABLE, &bDisable, sizeof(bDisable));
			    ControlSystemData(SFIELD_SET_ASMTPENABLE, &bDisable, sizeof(bDisable));
				ControlSystemData(SFIELD_SET_SDAENABLE, &bDisable, sizeof(bDisable));
		    }
		}
		else if(strcmp(tag->name, TAG_notificationRecurrence) == 0) {
			strcpy(EventTriggerNotification.notificationRecurrence, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_notificationInterval) == 0) {
			strcpy(EventTriggerNotification.notificationInterval, tag->value);

			if(strncmp(EventTriggerNotification.notificationInterval,"10 seconds",5) == 0)
               strcpy(EventTriggerNotification.notificationInterval,"0");
			else if(strncmp(EventTriggerNotification.notificationInterval,"30 seconds",5) == 0)
               strcpy(EventTriggerNotification.notificationInterval,"1");
			else if(strncmp(EventTriggerNotification.notificationInterval,"1 minute",5) == 0)
               strcpy(EventTriggerNotification.notificationInterval,"2");
			else if(strncmp(EventTriggerNotification.notificationInterval,"5 minutes",5) == 0)
               strcpy(EventTriggerNotification.notificationInterval,"3");
			else if(strncmp(EventTriggerNotification.notificationInterval,"10 minutes",5) == 0)
               strcpy(EventTriggerNotification.notificationInterval,"4");
			else if(strncmp(EventTriggerNotification.notificationInterval,"Non-Stop",5) == 0)
               strcpy(EventTriggerNotification.notificationInterval,"5");

			required |= xml_required_field(4);
			value = atoi(EventTriggerNotification.notificationInterval);
			ControlSystemData(SFIELD_SET_ALARMDURATION, &value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(4))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_triggers_id_notifications_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_triggers_id_notifications_id_capabilities(request *req, node_t *node)
{
	const char *notificationIntervalStr = "opt=\"10 seconds,30 seconds,1 minute,5 minutes,10 minutes,Non-Stop\"";
	const char *notificationMethodStr = "opt=\"FTP,SMTP,Local Storage\"";

	xadd_stag_attr(req, TAG_EventTriggerNotification, RootAttrStr);
	xadd_stag_attr(req, TAG_notificationMethod, notificationMethodStr);
	xadd_etag(req, TAG_notificationMethod);
	xadd_stag_attr(req, TAG_notificationInterval, notificationIntervalStr);
	xadd_etag(req, TAG_notificationInterval);
	xadd_etag(req, TAG_EventTriggerNotification);

	return STATUS_OK;
}

int retrieve_custom_event_schedule(request *req, node_t *node)
{
	EventSchedule_psia EventSchedule;
	SysInfo* pSysInfo = GetSysInfo();
	Schedule_t *pSchedule;
	int i;

	xadd_stag_attr(req, TAG_EventSchedule,RootAttrStr);

	/*TimeBlockList*/
	xadd_stag(req, TAG_TimeBlockList);
	for(i = 0; i < SCHDULE_NUM; i ++){
	  pSchedule = &(pSysInfo->lan_config.aSchedules[i]);

	  sprintf(EventSchedule.dayOfWeek,"%02d",pSchedule -> nDay);
	  sprintf(EventSchedule.beginTime,"%02d:%02d:%02d",pSchedule -> tStart.nHour, pSchedule -> tStart.nMin, pSchedule -> tStart.nSec);

	  //fprintf(stderr,"\r\n pSchedule -> tStart.nMin:%d  pSchedule -> tDuration.nMin:%d \n",pSchedule -> tStart.nMin,pSchedule -> tDuration.nMin);

#if 0
	  if((pSchedule -> tStart.nMin + pSchedule -> tDuration.nMin) >= 60)
         sprintf(EventSchedule.endTime,"%02d:%02d:%02d",(pSchedule -> tStart.nHour + pSchedule -> tDuration.nHour + 1),(pSchedule -> tStart.nMin + pSchedule -> tDuration.nMin - 60),(pSchedule -> tStart.nSec + pSchedule -> tDuration.nSec));
      else
	     sprintf(EventSchedule.endTime,"%02d:%02d:%02d",(pSchedule -> tStart.nHour + pSchedule -> tDuration.nHour),(pSchedule -> tStart.nMin + pSchedule -> tDuration.nMin),(pSchedule -> tStart.nSec + pSchedule -> tDuration.nSec));
#else
      sprintf(EventSchedule.endTime,"%02d:%02d:%02d",pSchedule -> tDuration.nHour,pSchedule -> tDuration.nMin,pSchedule -> tDuration.nSec);
#endif

	  xadd_stag(req, TAG_TimeBlock);
	  xadd_elem(req, TAG_dayOfWeek, EventSchedule.dayOfWeek);
	  xadd_stag(req, TAG_TimeRange);
	  xadd_elem(req, TAG_beginTime, EventSchedule.beginTime);
	  xadd_elem(req, TAG_endTime, EventSchedule.endTime);
	  xadd_etag(req, TAG_TimeRange);
	  xadd_etag(req, TAG_TimeBlock);
	  }
	xadd_etag(req, TAG_TimeBlockList);

	xadd_etag(req, TAG_EventSchedule);

	return STATUS_OK;
}

int update_custom_event_schedule(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	EventSchedule_psia EventSchedule;
	char scheduleConfig[30],beginTime[8],endTime[8];
	__u8 value,index = 0;
	char *ch1,*ch2,*ch3,*ch4;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_EventSchedule) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_TimeBlockList) == 0) {
            tag = tag->first_child;
			while(tag) {
				if(strcmp(tag->name, TAG_TimeBlock) == 0) {
                  tag = tag->first_child;
				  while(tag) {
					  if(strcmp(tag->name, TAG_dayOfWeek) == 0) {
                         strcpy(EventSchedule.dayOfWeek, tag->value);
			             required |= xml_required_field(1);
					  }
					  else if(strcmp(tag->name, TAG_TimeRange) == 0) {
                         tag = tag->first_child;
				         while(tag) {
							if(strcmp(tag->name, TAG_beginTime) == 0) {
                               strcpy(EventSchedule.beginTime, tag->value);
			                   required |= xml_required_field(2);
					        }
							else if(strcmp(tag->name, TAG_endTime) == 0) {
                               strcpy(EventSchedule.endTime, tag->value);
			                   required |= xml_required_field(3);
					        }
							if(!tag->next)
				               break;
                            tag = tag->next;
						 }
						 tag = tag->parent;
					  }
					  if(!tag->next)
				        break;
				      tag = tag->next;
				  }
				  tag = tag->parent;
                }

				value = atoi(EventSchedule.dayOfWeek);

				ch1 = EventSchedule.beginTime;
				ch2 = beginTime;

				while(*ch1 != '\0')
				{
				  if(*ch1 != ':')
				  {
					 *ch2 = *ch1;
					 ch2++;
				  }
				  ch1++;
				}
				*ch2 = '\0';

				ch3 = EventSchedule.endTime;
				ch4 = endTime;

				while(*ch3 != '\0')
				{
				  if(*ch3 != ':')
				  {
					 *ch4 = *ch3;
					 ch4++;
				  }
				  ch3++;
				}
				*ch4 = '\0';

				sprintf(scheduleConfig,"%02d%1d%02d%s%s",index,scheduleEnable[index],value,beginTime,endTime);
				//fprintf(stderr,"\r\nscheduleConfig:%s\n",scheduleConfig);

                ControlSystemData(SFIELD_SET_SCHEDULE, &scheduleConfig, strlen(scheduleConfig));

				index++;

				if(!tag->next)
				   break;
                tag = tag->next;
			}
			tag = tag->parent;
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(3))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_event_notification(request *req, node_t *node)
{
	EvenNotificationMethods_psia EvenNotificationMethods;
	SysInfo* pSysInfo = GetSysInfo();
	int supportMpeg4=0,supportMpeg4cif=0 ,supportH264=0, supportH264cif=0,supportJPG=0;

	supportJPG = pSysInfo->lan_config.Supportstream1;
	supportMpeg4 = pSysInfo->lan_config.Supportstream2;
	supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
	supportH264 = pSysInfo->lan_config.Supportstream5;
	supportH264cif = pSysInfo->lan_config.Supportstream6;
	int mpeg41Xsize= pSysInfo->lan_config.Mpeg41Xsize;
	int mpeg41Ysize= pSysInfo->lan_config.Mpeg41Ysize;
	int mpeg42Xsize=pSysInfo->lan_config.Mpeg42Xsize;
	int mpeg42Ysize=pSysInfo->lan_config.Mpeg42Ysize;
	int avc1Xsize=pSysInfo->lan_config.Avc1Xsize;
	int avc1Ysize=pSysInfo->lan_config.Avc1Ysize;
	int avc2Xsize=pSysInfo->lan_config.Avc2Xsize;
	int avc2Ysize=pSysInfo->lan_config.Avc2Ysize;

	sprintf(EvenNotificationMethods.uploadVideoClipEnabled,"%d",pSysInfo->ftp_config.rftpenable);
	sprintf(EvenNotificationMethods.senderEmailAddress,"%s",pSysInfo->smtp_config.sender_email);
	sprintf(EvenNotificationMethods.receiverEmailAddress,"%s",pSysInfo->smtp_config.receiver_email);
	sprintf(EvenNotificationMethods.attachedVideoClipEnabled,"%d",pSysInfo->lan_config.bASmtpEnable);

	if(supportH264){
		if(supportH264cif){
		   if(pSysInfo->lan_config.aviformat == 0)
	          sprintf(EvenNotificationMethods.videoClipFormatType,"H.264(%d x %d)",avc1Xsize,avc1Ysize);
	       else if(pSysInfo->lan_config.aviformat == 1)
	          sprintf(EvenNotificationMethods.videoClipFormatType,"H.264(%d x %d)",avc2Xsize,avc2Ysize);
		}
		else if(supportMpeg4cif){
		   if(pSysInfo->lan_config.aviformat == 0)
	          sprintf(EvenNotificationMethods.videoClipFormatType,"H.264(%d x %d)",avc1Xsize,avc1Ysize);
	       else if(pSysInfo->lan_config.aviformat == 1)
	          sprintf(EvenNotificationMethods.videoClipFormatType,"MPEG4(%d x %d)",mpeg42Xsize,mpeg42Ysize);
		}else
		   sprintf(EvenNotificationMethods.videoClipFormatType,"H.264(%d x %d)",avc1Xsize,avc1Ysize);
	}
	else if(supportMpeg4){
		if(supportH264cif){
		   if(pSysInfo->lan_config.aviformat == 0)
	          sprintf(EvenNotificationMethods.videoClipFormatType,"MPEG4(%d x %d)",mpeg41Xsize,mpeg41Ysize);
	       else if(pSysInfo->lan_config.aviformat == 1)
	          sprintf(EvenNotificationMethods.videoClipFormatType,"H.264(%d x %d)",avc2Xsize,avc2Ysize);
		}
		else if(supportMpeg4cif){
		   if(pSysInfo->lan_config.aviformat == 0)
	          sprintf(EvenNotificationMethods.videoClipFormatType,"MPEG4(%d x %d)",mpeg41Xsize,mpeg41Ysize);
	       else if(pSysInfo->lan_config.aviformat == 1)
	          sprintf(EvenNotificationMethods.videoClipFormatType,"MPEG4(%d x %d)",mpeg42Xsize,mpeg42Ysize);
		}else
		   sprintf(EvenNotificationMethods.videoClipFormatType,"MPEG4(%d x %d)",mpeg41Xsize,mpeg41Ysize);
	}
	else{
		sprintf(EvenNotificationMethods.videoClipFormatType,"NA");
	}

	xadd_stag_attr(req, TAG_EvenNotificationMethods, RootAttrStr);
	/* MailingNotificationList */
	retrieve_custom_event_notification_mailing(req,node);
	/* FTPNotificationList */
	retrieve_custom_event_notification_ftp(req,node);
	/* HttpHostNotificationList */
	retrieve_custom_event_notification_httpHost(req,node);
	xadd_stag(req, TAG_FTPFormat);
	xadd_elem(req, TAG_uploadVideoClipEnabled, EvenNotificationMethods.uploadVideoClipEnabled);
	xadd_etag(req, TAG_FTPFormat);
	xadd_stag(req, TAG_EmailFormat);
	xadd_elem(req, TAG_senderEmailAddress, EvenNotificationMethods.senderEmailAddress);
	xadd_elem(req, TAG_receiverEmailAddress, EvenNotificationMethods.receiverEmailAddress);
	xadd_stag(req, TAG_BodySetting);
    xadd_elem(req, TAG_attachedVideoClipEnabled, EvenNotificationMethods.attachedVideoClipEnabled);
	xadd_etag(req, TAG_BodySetting);
	xadd_etag(req, TAG_EmailFormat);
	xadd_stag(req, TAG_MediaFormat);
	xadd_elem(req, TAG_videoClipFormatType, EvenNotificationMethods.videoClipFormatType);
	xadd_etag(req, TAG_MediaFormat);
	xadd_etag(req, TAG_EvenNotificationMethods);

	return STATUS_OK;
}

int update_custom_event_notification(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	EvenNotificationMethods_psia EvenNotificationMethods;
	__u8 value;
	unsigned char bEnable;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_EvenNotificationMethods) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_MailingNotificationList) == 0) {
			update_custom_event_notification_mailing(req,node);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_FTPNotificationList) == 0) {
			update_custom_event_notification_ftp(req,node);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_HttpHostNotificationList) == 0) {
			update_custom_event_notification_httpHost(req,node);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_FTPFormat) == 0) {
			tag = tag->first_child;
			if(strcmp(tag->name, TAG_uploadVideoClipEnabled) == 0) {
			   strcpy(EvenNotificationMethods.uploadVideoClipEnabled, tag->value);
			   required |= xml_required_field(4);
			   value = atoi(EvenNotificationMethods.uploadVideoClipEnabled);
			   ControlSystemData(SFIELD_SET_FTP_RFTPENABLE, (void *)&value, sizeof(value));
		    }
			tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_EmailFormat) == 0) {
			tag = tag->first_child;
			while(tag) {
		      if(strcmp(tag->name, TAG_senderEmailAddress) == 0) {
			    strcpy(EvenNotificationMethods.senderEmailAddress, tag->value);
			    required |= xml_required_field(5);
				ControlSystemData(SFIELD_SET_SMTP_SENDER_EMAIL_ADDRESS, (void *)EvenNotificationMethods.senderEmailAddress, strlen(EvenNotificationMethods.senderEmailAddress));
		      }
		      else if(strcmp(tag->name, TAG_receiverEmailAddress) == 0) {
			    strcpy(EvenNotificationMethods.receiverEmailAddress, tag->value);
			    required |= xml_required_field(6);
				ControlSystemData(SFIELD_SET_SMTP_RECEIVER_EMAIL_ADDRESS, (void *)EvenNotificationMethods.receiverEmailAddress, strlen(EvenNotificationMethods.receiverEmailAddress));
		      }
			  else if(strcmp(tag->name, TAG_BodySetting) == 0) {
				  tag = tag->first_child;
			      while(tag) {
		           if(strcmp(tag->name, TAG_attachedVideoClipEnabled) == 0) {
			          strcpy(EvenNotificationMethods.attachedVideoClipEnabled, tag->value);
			          required |= xml_required_field(7);
					  bEnable = atoi(EvenNotificationMethods.attachedVideoClipEnabled);
					  ControlSystemData(SFIELD_SET_ASMTPENABLE, &bEnable, sizeof(bEnable));
		            }
					if(!tag->next)
				      break;
		            tag = tag->next;
	              }
				  tag = tag->parent;
		      }
			  if(!tag->next)
				 break;
              tag = tag->next;
			}
			tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_MediaFormat) == 0) {
			tag = tag->first_child;
			while(tag) {
		      if(strcmp(tag->name, TAG_videoClipFormatType) == 0) {
			    strcpy(EvenNotificationMethods.videoClipFormatType, tag->value);
			    required |= xml_required_field(8);
				value = atoi(EvenNotificationMethods.videoClipFormatType);
				//ControlSystemData(SFIELD_SET_AVIFORMAT, (void *)&value, sizeof(value));
		      }
			  if(!tag->next)
				 break;
			  tag = tag->next;
			}
			tag = tag->parent;
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(9))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_event_notification_capabilities(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	char videoClipFormatTypeStr[60];
	//const char *ftpFileFormatTypeStr;
	//const char *attachedFileFormatTypeStr;
	//const char *sdFileFormatTypeStr;
	int supportMpeg4=0,supportMpeg4cif=0 ,supportH264=0, supportH264cif=0,supportJPG=0;

	supportJPG = pSysInfo->lan_config.Supportstream1;
	supportMpeg4 = pSysInfo->lan_config.Supportstream2;
	supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
	supportH264 = pSysInfo->lan_config.Supportstream5;
	supportH264cif = pSysInfo->lan_config.Supportstream6;
	int mpeg41Xsize= pSysInfo->lan_config.Mpeg41Xsize;
	int mpeg41Ysize= pSysInfo->lan_config.Mpeg41Ysize;
	int mpeg42Xsize=pSysInfo->lan_config.Mpeg42Xsize;
	int mpeg42Ysize=pSysInfo->lan_config.Mpeg42Ysize;
	int avc1Xsize=pSysInfo->lan_config.Avc1Xsize;
	int avc1Ysize=pSysInfo->lan_config.Avc1Ysize;
	int avc2Xsize=pSysInfo->lan_config.Avc2Xsize;
	int avc2Ysize=pSysInfo->lan_config.Avc2Ysize;

	if ( supportH264 ) {
		if ( supportH264cif ){
			sprintf(videoClipFormatTypeStr,"opt=\"H.264(%d x %d),H.264(%d x %d)\"", avc1Xsize,avc1Ysize,avc2Xsize,avc2Ysize);
		}
		else if( supportMpeg4cif ){
			sprintf(videoClipFormatTypeStr,"opt=\"H.264(%d x %d),MPEG4(%d x %d)\"",avc1Xsize,avc1Ysize,mpeg42Xsize,mpeg42Ysize);
		}else
			sprintf(videoClipFormatTypeStr,"opt=\"H.264(%d x %d)\"",avc1Xsize,avc1Ysize);
	}
	else if ( supportMpeg4 ){
		if ( supportH264cif ){
			sprintf(videoClipFormatTypeStr,"opt=\"MPEG4(%d x %d),H.264(%d x %d)\"",mpeg41Xsize,mpeg41Ysize,avc2Xsize,avc2Ysize);
		}
		else if( supportMpeg4cif ){
			sprintf(videoClipFormatTypeStr,"opt=\"MPEG4(%d x %d),MPEG4(%d x %d)\"",mpeg41Xsize,mpeg41Ysize,mpeg42Xsize,mpeg42Ysize);
		}else
			sprintf(videoClipFormatTypeStr,"opt=\"MPEG4(%d x %d)\"",mpeg41Xsize,mpeg41Ysize);
	}
	else{
		sprintf(videoClipFormatTypeStr,"opt=\"NA\"");
	}

	xadd_stag_attr(req, TAG_EvenNotificationMethods, RootAttrStr);
	xadd_stag(req, TAG_MediaFormat);
	xadd_stag_attr(req, TAG_videoClipFormatType, videoClipFormatTypeStr);
	xadd_etag(req, TAG_videoClipFormatType);
	xadd_etag(req, TAG_MediaFormat);
	xadd_etag(req, TAG_EvenNotificationMethods);

	return STATUS_OK;
}

int retrieve_custom_event_notification_mailing(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_MailingNotificationList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_notification_mailing_id(req, node);
	xadd_etag(req, TAG_MailingNotificationList);

	return STATUS_OK;
}

int update_custom_event_notification_mailing(request *req, node_t *node)
{
	int n;

	for(n = 0; n < get_event_count(); n++)
		update_custom_event_notification_mailing_id(req, node);

	return STATUS_OK;
}

int create_custom_event_notification_mailing(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int delete_custom_event_notification_mailing(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_notification_mailing_id(request *req, node_t *node)
{
	MailingNotification_psia MailingNotification;
	SysInfo* pSysInfo = GetSysInfo();

    sprintf(MailingNotification.id,"0");
	sprintf(MailingNotification.authenticationMode,"%d",pSysInfo->smtp_config.authentication);
	sprintf(MailingNotification.addressingFormatType,"ipaddress");
	sprintf(MailingNotification.ipAddress,"%s",pSysInfo->smtp_config.servier_ip);
	sprintf(MailingNotification.accountName,"%s",pSysInfo->smtp_config.username);
	sprintf(MailingNotification.password,"%s",pSysInfo->smtp_config.password);

	xadd_stag_attr(req, TAG_MailingNotification, RootAttrStr);
	xadd_elem(req, TAG_id, MailingNotification.id);
	xadd_elem(req, TAG_authenticationMode, MailingNotification.authenticationMode);
	xadd_elem(req, TAG_addressingFormatType, MailingNotification.addressingFormatType);
	xadd_elem(req, TAG_ipAddress, MailingNotification.ipAddress);
	xadd_elem(req, TAG_accountName, MailingNotification.accountName);
	xadd_elem(req, TAG_password, MailingNotification.password);
	xadd_etag(req, TAG_MailingNotification);

	return STATUS_OK;
}

int update_custom_event_notification_mailing_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	MailingNotification_psia MailingNotification;
	unsigned char value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_MailingNotification) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(MailingNotification.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_authenticationMode) == 0) {
			strcpy(MailingNotification.authenticationMode, tag->value);
			required |= xml_required_field(2);
			value = atoi(MailingNotification.authenticationMode);
			ControlSystemData(SFIELD_SET_SMTP_AUTHENTICATION, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_addressingFormatType) == 0) {
			strcpy(MailingNotification.addressingFormatType, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_hostName) == 0) {
			strcpy(MailingNotification.hostName, tag->value);
			required |= xml_required_field(4);
			ControlSystemData(SFIELD_SET_SMTP_SENDER_EMAIL_ADDRESS, (void *)MailingNotification.hostName, strlen(MailingNotification.hostName));
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(MailingNotification.ipAddress, tag->value);
			required |= xml_required_field(5);
			ControlSystemData(SFIELD_SET_SMTP_SERVER_IP, (void *)MailingNotification.ipAddress, strlen(MailingNotification.ipAddress));
		}
		else if(strcmp(tag->name, TAG_accountName) == 0) {
			strcpy(MailingNotification.accountName, tag->value);
			required |= xml_required_field(6);
			ControlSystemData(SFIELD_SET_SMTP_USERNAME, (void *)MailingNotification.accountName, strlen(MailingNotification.accountName));
		}
		else if(strcmp(tag->name, TAG_password) == 0) {
			strcpy(MailingNotification.password, tag->value);
			required |= xml_required_field(7);
			ControlSystemData(SFIELD_SET_SMTP_PASSWORD, (void *)MailingNotification.password, strlen(MailingNotification.password));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(7))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_notification_mailing_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_notification_ftp(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_FTPNotificationList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_notification_ftp_id(req, node);
	xadd_etag(req, TAG_FTPNotificationList);

	return STATUS_OK;
}

int update_custom_event_notification_ftp(request *req, node_t *node)
{
	int n;

	for(n = 0; n < get_event_count(); n++)
		update_custom_event_notification_ftp_id(req, node);

	return STATUS_OK;
}

int create_custom_event_notification_ftp(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int delete_custom_event_notification_ftp(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_notification_ftp_id(request *req, node_t *node)
{
	FTPNotification_psia FTPNotification;
	SysInfo* pSysInfo = GetSysInfo();

    sprintf(FTPNotification.id,"0");
	sprintf(FTPNotification.addressingFormatType,"ipaddress");
	sprintf(FTPNotification.ipAddress,"%s",pSysInfo->ftp_config.servier_ip);
	sprintf(FTPNotification.portNo,"%d",pSysInfo->ftp_config.port);
	sprintf(FTPNotification.userName,"%s",pSysInfo->ftp_config.username);
	sprintf(FTPNotification.password,"%s",pSysInfo->ftp_config.password);
	sprintf(FTPNotification.uploadPath,"%s",pSysInfo->ftp_config.foldername);

	xadd_stag_attr(req, TAG_FTPNotification, RootAttrStr);
	xadd_elem(req, TAG_id, FTPNotification.id);
	xadd_elem(req, TAG_addressingFormatType, FTPNotification.addressingFormatType);
	xadd_elem(req, TAG_ipAddress, FTPNotification.ipAddress);
    xadd_elem(req, TAG_portNo, FTPNotification.portNo);
	xadd_elem(req, TAG_userName, FTPNotification.userName);
	xadd_elem(req, TAG_password, FTPNotification.password);
	xadd_elem(req, TAG_uploadPath, FTPNotification.uploadPath);
	xadd_etag(req, TAG_FTPNotification);

	return STATUS_OK;
}

int update_custom_event_notification_ftp_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	FTPNotification_psia FTPNotification;
	unsigned short port;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_FTPNotification) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(FTPNotification.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_addressingFormatType) == 0) {
			strcpy(FTPNotification.addressingFormatType, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(FTPNotification.ipAddress, tag->value);
			required |= xml_required_field(3);
			ControlSystemData(SFIELD_SET_FTP_SERVER_IP, (void *)FTPNotification.ipAddress, strlen(FTPNotification.ipAddress));
		}
		else if(strcmp(tag->name, TAG_portNo) == 0) {
			strcpy(FTPNotification.portNo, tag->value);
			required |= xml_required_field(4);
			port = atoi(FTPNotification.portNo);
			ControlSystemData(SFIELD_SET_FTP_PORT, (void *)&port, sizeof(port));
		}
		else if(strcmp(tag->name, TAG_userName) == 0) {
			strcpy(FTPNotification.userName, tag->value);
			required |= xml_required_field(5);
			ControlSystemData(SFIELD_SET_FTP_USERNAME, (void *)FTPNotification.userName, strlen(FTPNotification.userName));
		}
		else if(strcmp(tag->name, TAG_password) == 0) {
			strcpy(FTPNotification.password, tag->value);
			required |= xml_required_field(6);
			ControlSystemData(SFIELD_SET_FTP_PASSWORD, (void *)FTPNotification.password, strlen(FTPNotification.password));
		}
		else if(strcmp(tag->name, TAG_uploadPath) == 0) {
			strcpy(FTPNotification.uploadPath, tag->value);
			required |= xml_required_field(7);
			ControlSystemData(SFIELD_SET_FTP_FOLDNAME, (void *)FTPNotification.uploadPath, strlen(FTPNotification.uploadPath));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(7))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_notification_ftp_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_notification_httpHost(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_HttpHostNotificationList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_notification_httpHost_id(req, node);
	xadd_etag(req, TAG_HttpHostNotificationList);

	return STATUS_OK;
}

int update_custom_event_notification_httpHost(request *req, node_t *node)
{
	int n;

	for(n = 0; n < get_event_count(); n++)
		update_custom_event_notification_httpHost_id(req, node);

	return STATUS_OK;
}

int create_custom_event_notification_httpHost(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int delete_custom_event_notification_httpHost(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_event_notification_httpHost_id(request *req, node_t *node)
{
	HttpHostNotification_psia HttpHostNotification;
	SysInfo* pSysInfo = GetSysInfo();
	NET_IPV4 ip;

	ip.int32 = net_get_ifaddr(ETH_NAME);

    sprintf(HttpHostNotification.id,"0");
	sprintf(HttpHostNotification.url,"http://%03d.%03d.%03d.%03d", ip.str[0], ip.str[1], ip.str[2], ip.str[3]);
	sprintf(HttpHostNotification.protocolType,"http");
	sprintf(HttpHostNotification.addressingFormatType,"ipaddress");
	sprintf(HttpHostNotification.ipAddress,"%03d.%03d.%03d.%03d", ip.str[0], ip.str[1], ip.str[2], ip.str[3]);
	sprintf(HttpHostNotification.portNo,"%d",pSysInfo->lan_config.net.http_port);

	xadd_stag_attr(req, TAG_HttpHostNotification, RootAttrStr);
	xadd_elem(req, TAG_id, HttpHostNotification.id);
	xadd_elem(req, TAG_url, HttpHostNotification.url);
	xadd_elem(req, TAG_protocolType, HttpHostNotification.protocolType);
	xadd_elem(req, TAG_addressingFormatType, HttpHostNotification.addressingFormatType);
	xadd_elem(req, TAG_ipAddress, HttpHostNotification.ipAddress);
    xadd_elem(req, TAG_portNo, HttpHostNotification.portNo);
	xadd_etag(req, TAG_HttpHostNotification);

	return STATUS_OK;
}

int update_custom_event_notification_httpHost_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	HttpHostNotification_psia HttpHostNotification;
	struct in_addr ipaddr; //sys_ip;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_HttpHostNotification) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(HttpHostNotification.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_url) == 0) {
			strcpy(HttpHostNotification.url, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_protocolType) == 0) {
			strcpy(HttpHostNotification.protocolType, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_addressingFormatType) == 0) {
			strcpy(HttpHostNotification.addressingFormatType, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(HttpHostNotification.ipAddress, tag->value);
			required |= xml_required_field(5);
			ipv4_str_to_num(HttpHostNotification.ipAddress, &ipaddr);
            ControlSystemData(SFIELD_SET_IP, (void *)&ipaddr.s_addr, sizeof(ipaddr.s_addr));
		}
		else if(strcmp(tag->name, TAG_portNo) == 0) {
			strcpy(HttpHostNotification.portNo, tag->value);
			required |= xml_required_field(6);
			value = atoi(HttpHostNotification.portNo);
			ControlSystemData(SFIELD_SET_HTTPPORT, &value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(6))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_notification_httpHost_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

/*******************************************************************/
/*                            Custom/Analytics                     */
/*******************************************************************/
int retrieve_custom_analytics_motiondetection_index(request *req, node_t *node)
{
	Index_psia Index;

	sprintf(Index.version,"1.0");

	xadd_stag_attr(req, TAG_ResourceList, ResourceListAttrStr);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"index");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"description");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_stag_attr(req, TAG_Resource, IndexAttrStr);
	sprintf(Index.name,"ID");
	sprintf(Index.type,"resource");
	xadd_elem(req, TAG_name, Index.name);
	xadd_elem(req, TAG_version, Index.version);
	xadd_elem(req, TAG_type, Index.type);
	xadd_etag(req, TAG_Resource);

	xadd_etag(req, TAG_ResourceList);

	return STATUS_OK;
}

int retrieve_custom_analytics_motiondetection_description(request *req, node_t *node)
{
	Description_psia Description;

	sprintf(Description.name,"CustomMotionDetection");
	sprintf(Description.version,"1.0");
	sprintf(Description.type,"service");
	sprintf(Description.inboundData,"none");
	sprintf(Description.returnResult,"none");
	sprintf(Description.function,"none");
	sprintf(Description.notes,"none");

	xadd_stag_attr(req, TAG_ResourceDescription, RootAttrStr);
	xadd_elem(req, TAG_name, Description.name);
	xadd_elem(req, TAG_version, Description.version);
	xadd_elem(req, TAG_type, Description.type);

	xadd_stag(req, TAG_get);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, "Motion Detection Service");
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_get);

	xadd_stag(req, TAG_put);
    xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_put);

	xadd_stag(req, TAG_post);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_post);

	xadd_stag(req, TAG_delete);
	xadd_stag(req, TAG_QueryStringParameterList);
	xadd_etag(req, TAG_QueryStringParameterList);
	xadd_elem(req, TAG_inboundData, Description.inboundData);
	xadd_elem(req, TAG_returnResult, Description.returnResult);
	xadd_elem(req, TAG_function, Description.function);
	xadd_elem(req, TAG_notes, Description.notes);
	xadd_etag(req, TAG_delete);

	xadd_etag(req, TAG_ResourceDescription);

	return STATUS_OK;
}

int get_region_count(void)
{
	return 1;
}

int retrieve_custom_analytics_motiondetection(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_MotionDetectionList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
		retrieve_custom_analytics_motiondetection_id(req, node);
	xadd_etag(req, TAG_MotionDetectionList);

	return STATUS_OK;
}

int retrieve_custom_analytics_motiondetection_id(request *req, node_t *node)
{
	MotionDetection_psia MotionDetection;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(MotionDetection.id,"0");
	sprintf(MotionDetection.enabled,"%d",pSysInfo->motion_config.motionenable);
	sprintf(MotionDetection.samplingInterval,"0");
    sprintf(MotionDetection.startTriggerTime,"0");
	sprintf(MotionDetection.endTriggerTime,"0");

	if(pSysInfo->face_config.fddirection == 0)
	  sprintf(MotionDetection.directionSensitivity,"up-down");
	else if(pSysInfo->face_config.fddirection == 1)
	  sprintf(MotionDetection.directionSensitivity,"left-right");
	else if(pSysInfo->face_config.fddirection == 2)
	  sprintf(MotionDetection.directionSensitivity,"right-left");

	sprintf(MotionDetection.regionType,"grid");
	sprintf(MotionDetection.minObjectSize,"1");
	sprintf(MotionDetection.maxObjectSize,"99");
	sprintf(MotionDetection.rowGranularity,"3");
	sprintf(MotionDetection.columnGranularity,"4");

	xadd_stag_attr(req, TAG_MotionDetection, RootAttrStr);
	xadd_elem(req, TAG_id, MotionDetection.id);
	xadd_elem(req, TAG_enabled, MotionDetection.enabled);
	xadd_elem(req, TAG_samplingInterval, MotionDetection.samplingInterval);
    xadd_elem(req, TAG_startTriggerTime, MotionDetection.startTriggerTime);
	xadd_elem(req, TAG_endTriggerTime, MotionDetection.endTriggerTime);
	xadd_elem(req, TAG_directionSensitivity, MotionDetection.directionSensitivity);
	xadd_elem(req, TAG_regionType, MotionDetection.regionType);
	xadd_elem(req, TAG_minObjectSize, MotionDetection.minObjectSize);
	xadd_elem(req, TAG_maxObjectSize, MotionDetection.maxObjectSize);
	xadd_stag(req, TAG_Grid);
	xadd_elem(req, TAG_rowGranularity, MotionDetection.rowGranularity);
	xadd_elem(req, TAG_columnGranularity, MotionDetection.columnGranularity);
	xadd_etag(req, TAG_Grid);
	/* MotionDetectionRegionList */
	retrieve_custom_analytics_motiondetection_id_regions(req,node);
	xadd_etag(req, TAG_MotionDetection);

	return STATUS_OK;
}

int update_custom_analytics_motiondetection_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	MotionDetection_psia MotionDetection;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_MotionDetection) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(MotionDetection.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(MotionDetection.enabled, tag->value);
			required |= xml_required_field(2);
			value = atoi(MotionDetection.enabled);
			ControlSystemData(SFIELD_SET_MOTIONENABLE, &value, strlen(MotionDetection.enabled));
		}
		else if(strcmp(tag->name, TAG_directionSensitivity) == 0) {
			strcpy(MotionDetection.directionSensitivity, tag->value);

			if(strncmp(MotionDetection.directionSensitivity,"up-down",4) == 0)
               strcpy(MotionDetection.directionSensitivity,"0");
			else if(strncmp(MotionDetection.directionSensitivity,"left-right",4) == 0)
               strcpy(MotionDetection.directionSensitivity,"1");
			else if(strncmp(MotionDetection.directionSensitivity,"right-left",4) == 0)
               strcpy(MotionDetection.directionSensitivity,"2");

			required |= xml_required_field(3);
			value = atoi(MotionDetection.directionSensitivity);
			ControlSystemData(SFIELD_SET_FD_DIRECTION, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_regionType) == 0) {
			strcpy(MotionDetection.regionType, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_Grid) == 0) {
			tag = tag->first_child;
	        while(tag) {
				if(strcmp(tag->name, TAG_rowGranularity) == 0) {
			      strcpy(MotionDetection.rowGranularity, tag->value);
			      required |= xml_required_field(5);
		        }
		        else if(strcmp(tag->name, TAG_columnGranularity) == 0) {
			      strcpy(MotionDetection.columnGranularity, tag->value);
			      required |= xml_required_field(6);
		        }
				if(!tag->next)
				 break;
                tag = tag->next;
			}
			tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_MotionDetectionRegionList) == 0) {
			update_custom_analytics_motiondetection_id_regions(req,node);
			required |= xml_required_field(7);
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(7))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_analytics_motiondetection_id_capabilities(request *req, node_t *node)
{
	const char *directionSensitivityStr = "opt=\"up-down,left-right,right-left\"";

	xadd_stag_attr(req, TAG_MotionDetection, RootAttrStr);
	xadd_stag_attr(req, TAG_directionSensitivity, directionSensitivityStr);
	xadd_etag(req, TAG_directionSensitivity);
	xadd_etag(req, TAG_MotionDetection);

	return STATUS_OK;
}

int retrieve_custom_analytics_facedetection(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_FaceDetectionList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
		retrieve_custom_analytics_facedetection_id(req, node);
	xadd_etag(req, TAG_FaceDetectionList);

	return STATUS_OK;
}

int retrieve_custom_analytics_facedetection_id(request *req, node_t *node)
{
	FaceDetection_psia FaceDetection;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(FaceDetection.id,"0");
    sprintf(FaceDetection.enabled,"%d",pSysInfo->face_config.fdetect);
	sprintf(FaceDetection.conflevel,"%d",pSysInfo->face_config.fdconflevel);
	sprintf(FaceDetection.positionX,"%d",pSysInfo->face_config.startX);
	sprintf(FaceDetection.positionY,"%d",pSysInfo->face_config.startY);
	sprintf(FaceDetection.width,"%d",pSysInfo->face_config.width);
	sprintf(FaceDetection.height,"%d",pSysInfo->face_config.height);

	if(pSysInfo->face_config.maskoption == 0)
	   sprintf(FaceDetection.maskoption,"GREY BOX");

	xadd_stag_attr(req, TAG_FaceDetection, RootAttrStr);
	xadd_elem(req, TAG_id, FaceDetection.id);
	xadd_elem(req, TAG_enabled, FaceDetection.enabled);
	xadd_elem(req, TAG_conflevel, FaceDetection.conflevel);
	xadd_stag(req, TAG_RegionCoordinates);
	xadd_elem(req, TAG_positionX, FaceDetection.positionX);
	xadd_elem(req, TAG_positionY, FaceDetection.positionY);
	xadd_elem(req, TAG_width, FaceDetection.width);
	xadd_elem(req, TAG_height, FaceDetection.height);
	xadd_etag(req, TAG_RegionCoordinates);
	xadd_elem(req, TAG_maskoption, FaceDetection.maskoption);
    xadd_etag(req, TAG_FaceDetection);

	return STATUS_OK;
}

int update_custom_analytics_facedetection_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	FaceDetection_psia FaceDetection;
	__u8 value;
	unsigned int roiX,roiY,roiWidth,roiHeight;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_FaceDetection) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(FaceDetection.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(FaceDetection.enabled, tag->value);
			required |= xml_required_field(2);
			value = atoi(FaceDetection.enabled);
			ControlSystemData(SFIELD_SET_FDETECT, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_conflevel) == 0) {
			strcpy(FaceDetection.conflevel, tag->value);
			required |= xml_required_field(3);
			value = atoi(FaceDetection.conflevel);
			ControlSystemData(SFIELD_SET_FDCONF_LEVEL, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_RegionCoordinates) == 0) {
            tag = tag->first_child;
			while(tag) {
			  if(strcmp(tag->name, TAG_positionX) == 0) {
                 strcpy(FaceDetection.positionX, tag->value);
			     required |= xml_required_field(4);
                 roiX = atoi(FaceDetection.positionX);
				 ControlSystemData(SFIELD_SET_FDX, &roiX, sizeof(roiX));
			  }
			  else if(strcmp(tag->name, TAG_positionY) == 0) {
                 strcpy(FaceDetection.positionY, tag->value);
			     roiY = atoi(FaceDetection.positionY);
				 ControlSystemData(SFIELD_SET_FDY, &roiY, sizeof(roiY));
			  }
			  else if(strcmp(tag->name, TAG_width) == 0) {
                 strcpy(FaceDetection.width, tag->value);
			     required |= xml_required_field(6);
				 roiWidth = atoi(FaceDetection.width);
				 ControlSystemData(SFIELD_SET_FDW, &roiWidth, sizeof(roiWidth));
			  }
			  else if(strcmp(tag->name, TAG_height) == 0) {
                 strcpy(FaceDetection.height, tag->value);
			     required |= xml_required_field(7);
				 roiHeight = atoi(FaceDetection.height);
				 ControlSystemData(SFIELD_SET_FDH, &roiHeight, sizeof(roiHeight));
			  }
			  if(!tag->next)
				 break;
			  tag = tag->next;
			}
			tag = tag->parent;
        }
		else if(strcmp(tag->name, TAG_maskoption) == 0) {
			strcpy(FaceDetection.maskoption, tag->value);

			if(strncmp(FaceDetection.maskoption,"GREY BOX",4) == 0)
                strcpy(FaceDetection.maskoption,"0");

			required |= xml_required_field(8);
			value = atoi(FaceDetection.maskoption);
			ControlSystemData(SFIELD_SET_MASK_OPTIONS, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(8))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_analytics_facedetection_id_capabilities(request *req, node_t *node)
{
	const char *maskoptionStr = "opt=\"GREY BOX\"";

	xadd_stag_attr(req, TAG_FaceDetection, RootAttrStr);
	xadd_stag_attr(req, TAG_maskoption, maskoptionStr);
	xadd_etag(req, TAG_maskoption);
	xadd_etag(req, TAG_FaceDetection);

	return STATUS_OK;
}

int retrieve_custom_analytics_facerecognition(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_FaceRecognitionList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
		retrieve_custom_analytics_facerecognition_id(req, node);
	xadd_etag(req, TAG_FaceRecognitionList);

	return STATUS_OK;
}

int retrieve_custom_analytics_facerecognition_id(request *req, node_t *node)
{
	FaceRecognition_psia FaceRecognition;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(FaceRecognition.id,"0");

	if(pSysInfo->face_config.frecog == 0)
	  sprintf(FaceRecognition.enabled,"OFF");
	else if(pSysInfo->face_config.frecog == 1)
	  sprintf(FaceRecognition.enabled,"RECOGNIZE USER");
	else if(pSysInfo->face_config.frecog == 2)
	  sprintf(FaceRecognition.enabled,"REGISTER USER");
	else if(pSysInfo->face_config.frecog == 2)
	  sprintf(FaceRecognition.enabled,"CLEAR ALL USERS");

	sprintf(FaceRecognition.conflevel,"%d",pSysInfo->face_config.frconflevel);
	sprintf(FaceRecognition.database,"%d",pSysInfo->face_config.frdatabase);

	xadd_stag_attr(req, TAG_FaceRecognition, RootAttrStr);
	xadd_elem(req, TAG_id, FaceRecognition.id);
	xadd_elem(req, TAG_enabled, FaceRecognition.enabled);
	xadd_elem(req, TAG_conflevel, FaceRecognition.conflevel);
	xadd_elem(req, TAG_database, FaceRecognition.database);
    xadd_etag(req, TAG_FaceRecognition);

	return STATUS_OK;
}

int update_custom_analytics_facerecognition_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	FaceRecognition_psia FaceRecognition;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_FaceRecognition) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(FaceRecognition.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			//fprintf(stderr,"\r\nInside frecognition TAG_enabled\n");
			strcpy(FaceRecognition.enabled, tag->value);

			if(strncmp(FaceRecognition.enabled,"OFF",3) == 0)
               strcpy(FaceRecognition.enabled,"0");
			else if(strncmp(FaceRecognition.enabled,"RECOGNIZE USER",5) == 0)
               strcpy(FaceRecognition.enabled,"1");
			else if(strncmp(FaceRecognition.enabled,"REGISTER USER",5) == 0)
               strcpy(FaceRecognition.enabled,"2");
			else if(strncmp(FaceRecognition.enabled,"CLEAR ALL USERS",5) == 0)
               strcpy(FaceRecognition.enabled,"3");

			required |= xml_required_field(2);
			value = atoi(FaceRecognition.enabled);
			ControlSystemData(SFIELD_SET_FRECOGNITION, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_conflevel) == 0) {
			strcpy(FaceRecognition.conflevel, tag->value);
			required |= xml_required_field(3);
			value = atoi(FaceRecognition.conflevel);
			ControlSystemData(SFIELD_SET_FRCONF_LEVEL, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_database) == 0) {
			strcpy(FaceRecognition.database, tag->value);
			required |= xml_required_field(4);
			value = atoi(FaceRecognition.database);
			ControlSystemData(SFIELD_SET_FR_DATABASE, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(4))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_analytics_facerecognition_id_capabilities(request *req, node_t *node)
{
	const char *enabledStr = "opt=\"OFF,RECOGNIZE USER,REGISTER USER,CLEAR ALL USERS\"";

	xadd_stag_attr(req, TAG_FaceRecognition, RootAttrStr);
	xadd_stag_attr(req, TAG_enabled, enabledStr);
	xadd_etag(req, TAG_enabled);
	xadd_etag(req, TAG_FaceRecognition);

	return STATUS_OK;
}


int retrieve_custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_MotionDetectionRegionList, RootAttrStr);
	for(n = 0; n < get_region_count(); n++)
		retrieve_custom_analytics_motiondetection_id_regions_id(req, node);
	xadd_etag(req, TAG_MotionDetectionRegionList);

	return STATUS_OK;
}

int update_custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	int n;

	for(n = 0; n < get_region_count(); n++)
		update_custom_analytics_motiondetection_id_regions_id(req, node);

	return STATUS_OK;
}

int create_custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int delete_custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

unsigned char charToint(unsigned char *pChar)
{
	unsigned char ret;

	if( (*pChar>=0x30)&& (*pChar<=0x39) )
	{
		ret = (unsigned char)*pChar - 0x30;
		return ret;
	} else if( (*pChar>=0x41) && (*pChar<=0x46)  )
	{
		ret = (unsigned char)*pChar - 0x37;
		return ret;
	}
	return 0;
}

int retrieve_custom_analytics_motiondetection_id_regions_id(request *req, node_t *node)
{
	MotionDetectionRegion_psia MotionDetectionRegion;
	SysInfo* pSysInfo = GetSysInfo();
	int i;
	unsigned char xvalue, yvalue, zvalue;

    sprintf(MotionDetectionRegion.id,"0");
	sprintf(MotionDetectionRegion.maskEnabled,"0");
	sprintf(MotionDetectionRegion.enabled,"%d",pSysInfo->motion_config.motioncenable);
	sprintf(MotionDetectionRegion.detectionThreshold,"%d",pSysInfo->motion_config.motioncvalue);

	if(pSysInfo->motion_config.motionlevel == 0)
	   sprintf(MotionDetectionRegion.sensitivityLevel,"20");
	else if(pSysInfo->motion_config.motionlevel == 1)
	   sprintf(MotionDetectionRegion.sensitivityLevel,"60");
	else if(pSysInfo->motion_config.motionlevel == 2)
	   sprintf(MotionDetectionRegion.sensitivityLevel,"100");

	xadd_stag_attr(req, TAG_MotionDetectionRegion, RootAttrStr);
	xadd_elem(req, TAG_id, MotionDetectionRegion.id);
	xadd_elem(req, TAG_enabled, MotionDetectionRegion.enabled);
	xadd_elem(req, TAG_maskEnabled, MotionDetectionRegion.maskEnabled);
    xadd_elem(req, TAG_sensitivityLevel, MotionDetectionRegion.sensitivityLevel);
	xadd_elem(req, TAG_detectionThreshold, MotionDetectionRegion.detectionThreshold);

	xadd_stag(req, TAG_RegionCoordinatesList);

	xvalue = charToint(&pSysInfo->motion_config.motionblock[0]);
	yvalue = charToint(&pSysInfo->motion_config.motionblock[1]);
	zvalue = charToint(&pSysInfo->motion_config.motionblock[2]);

	for (i = 0;i < 4 ;i++)
	{
      if ((xvalue & (1<<i)))
      {
		if (i == 0){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"1");
          sprintf(MotionDetectionRegion.positionY,"1");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}
		else if(i == 1){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"1");
          sprintf(MotionDetectionRegion.positionY,"2");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);

		}
		else if(i == 2){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"1");
          sprintf(MotionDetectionRegion.positionY,"3");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}
		else if(i == 3){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"1");
          sprintf(MotionDetectionRegion.positionY,"4");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}
      }
	  if ((yvalue & (1<<i)))
      {
		if (i == 0){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"2");
          sprintf(MotionDetectionRegion.positionY,"1");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}
		else if(i == 1){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"2");
          sprintf(MotionDetectionRegion.positionY,"2");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);

		}
		else if(i == 2){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"2");
          sprintf(MotionDetectionRegion.positionY,"3");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}
		else if(i == 3){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"2");
          sprintf(MotionDetectionRegion.positionY,"4");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}

      }
	  if ((zvalue & (1<<i)))
      {
		if (i == 0){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"3");
          sprintf(MotionDetectionRegion.positionY,"1");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}
		else if(i == 1){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"3");
          sprintf(MotionDetectionRegion.positionY,"2");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);

		}
		else if(i == 2){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"3");
          sprintf(MotionDetectionRegion.positionY,"3");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}
		else if(i == 3){
		  xadd_stag(req, TAG_RegionCoordinates);
		  sprintf(MotionDetectionRegion.positionX,"3");
          sprintf(MotionDetectionRegion.positionY,"4");
	      xadd_elem(req, TAG_positionX, MotionDetectionRegion.positionX);
	      xadd_elem(req, TAG_positionY, MotionDetectionRegion.positionY);
	      xadd_etag(req, TAG_RegionCoordinates);
		}
      }
	}
	xadd_etag(req, TAG_RegionCoordinatesList);
	xadd_etag(req, TAG_MotionDetectionRegion);

	return STATUS_OK;
}

int update_custom_analytics_motiondetection_id_regions_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	MotionDetectionRegion_psia MotionDetectionRegion;
	__u8 value;
	unsigned int valueX = 0, valueY = 0;
	char motionValue[10];
	int motionBlock,i;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_MotionDetectionRegion) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(MotionDetectionRegion.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(MotionDetectionRegion.enabled, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_maskEnabled) == 0) {
			strcpy(MotionDetectionRegion.maskEnabled, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_sensitivityLevel) == 0) {
			strcpy(MotionDetectionRegion.sensitivityLevel, tag->value);

			if(strncmp(MotionDetectionRegion.sensitivityLevel,"20",2) == 0)
                strcpy(MotionDetectionRegion.sensitivityLevel,"0");
			else if(strncmp(MotionDetectionRegion.sensitivityLevel,"60",2) == 0)
                strcpy(MotionDetectionRegion.sensitivityLevel,"1");
			else if(strncmp(MotionDetectionRegion.sensitivityLevel,"100",2) == 0)
                strcpy(MotionDetectionRegion.sensitivityLevel,"2");

			required |= xml_required_field(4);
			value = atoi(MotionDetectionRegion.sensitivityLevel);
			ControlSystemData(SFIELD_SET_MOTIONLEVEL, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_detectionThreshold) == 0) {
			strcpy(MotionDetectionRegion.detectionThreshold, tag->value);
			required |= xml_required_field(5);
			value = atoi(MotionDetectionRegion.detectionThreshold);
			ControlSystemData(SFIELD_SET_MOTIONCVALUE, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_RegionCoordinatesList) == 0) {
            tag = tag->first_child;
			while(tag) {
				if(strcmp(tag->name, TAG_RegionCoordinates) == 0) {
                  tag = tag->first_child;
				  while(tag) {
					  if(strcmp(tag->name, TAG_positionX) == 0) {
                         strcpy(MotionDetectionRegion.positionX, tag->value);
			             required |= xml_required_field(6);
						 valueX = atoi(MotionDetectionRegion.positionX);
					  }
					  else if(strcmp(tag->name, TAG_positionY) == 0) {
                         strcpy(MotionDetectionRegion.positionY, tag->value);
			             required |= xml_required_field(7);
						 valueY = atoi(MotionDetectionRegion.positionY);
					  }
					  if(!tag->next)
				         break;
				      tag = tag->next;
				  }
				  if(valueX == 1){
					 if(valueY == 1){
						xBlock = xBlock | (1<<8);
					 }
					 else if(valueY == 2){
						xBlock = xBlock | (1<<9);
					 }
					 else if(valueY == 3){
						xBlock = xBlock | (1<<10);
					 }
					 else if(valueY == 4){
						xBlock = xBlock | (1<<11);
					 }
				  }
				  else if(valueX == 2){
					 if(valueY == 1){
						yBlock = yBlock | (1<<4);
					 }
					 else if(valueY == 2){
						yBlock = yBlock | (1<<5);
					 }
					 else if(valueY == 3){
						yBlock = yBlock | (1<<6);
					 }
					 else if(valueY == 4){
						yBlock = yBlock | (1<<7);
					 }
				  }
				  else if(valueX == 3){
					 if(valueY == 1){
						zBlock = zBlock | (1<<0);
					 }
					 else if(valueY == 2){
						zBlock = zBlock | (1<<1);
					 }
					 else if(valueY == 3){
						zBlock = zBlock | (1<<2);
					 }
					 else if(valueY == 4){
						zBlock = zBlock | (1<<3);
					 }
				  }
				  else if(valueX == 0){
					  strcpy(motionValue,"000");
					  ControlSystemData(SFIELD_SET_MOTIONBLOCK, motionValue, (strlen(motionValue)+1));
				  }
				  tag = tag->parent;
                }
				if(!tag->next)
				   break;
                tag = tag->next;
			}
			motionBlock = xBlock | yBlock | zBlock;
			//fprintf(stderr,"\r\nInside motiondetection_id_regions_id with motionBlock:%x\n",motionBlock);

			if(xBlock == 0)
			   sprintf(motionValue,"0%x",motionBlock);
			else
               sprintf(motionValue,"%x",motionBlock);

			for( i = 0; motionValue[ i ]; i++)
                 motionValue[ i ] = toupper( motionValue[ i ] );

			//fprintf(stderr,"\r\nInside motiondetection_id_regions_id with motionValue:%s\n",motionValue);
			if(valueX != 0)
			  ControlSystemData(SFIELD_SET_MOTIONBLOCK, motionValue, (strlen(motionValue)+1));

			xBlock = 0;
			yBlock = 0;
			zBlock = 0;

			tag = tag->parent;
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	//if(required != xml_required_mask(7))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_analytics_motiondetection_id_regions_id(request *req, node_t *node)
{
	return STATUS_NOT_IMPLEMENTED;
}

int retrieve_custom_analytics_motiondetection_id_regions_id_capabilities(request *req, node_t *node)
{
	const char *sensitivityLevelStr = "opt=\"20,60,100\"";

	xadd_stag_attr(req, TAG_MotionDetectionRegion, RootAttrStr);
	xadd_stag_attr(req, TAG_sensitivityLevel, sensitivityLevelStr);
	xadd_etag(req, TAG_sensitivityLevel);
	xadd_etag(req, TAG_MotionDetectionRegion);

	return STATUS_OK;

}

/*******************************************************************/
/*                            Custom                               */
/*******************************************************************/
int retrieve_custom_live_channels(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_LiveSettingList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
	    retrieve_custom_live_channels_id(req, node);
	xadd_etag(req, TAG_LiveSettingList);

	return STATUS_OK;
}

int retrieve_custom_live_channels_id(request *req, node_t *node)
{
	LiveSetting_psia LiveSetting;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(LiveSetting.id,"0");
	sprintf(LiveSetting.clickSnapFileName,"%s", pSysInfo->lan_config.nClickSnapFilename);
	sprintf(LiveSetting.clickSnapStorage,"%d", pSysInfo->lan_config.nClickSnapStorage);

	if(pSysInfo->lan_config.democfg == 0)
	   sprintf(LiveSetting.demoCfg,"NONE");
	else if(pSysInfo->lan_config.democfg == 1)
	   sprintf(LiveSetting.demoCfg,"VS DEMO");
	else if(pSysInfo->lan_config.democfg == 2)
	   sprintf(LiveSetting.demoCfg,"VNF DEMO");
	else if(pSysInfo->lan_config.democfg == 3)
	   sprintf(LiveSetting.demoCfg,"LDC DEMO");
	else if(pSysInfo->lan_config.democfg == 4)
	   sprintf(LiveSetting.demoCfg,"FD DEMO");
	else if(pSysInfo->lan_config.democfg == 5)
	   sprintf(LiveSetting.demoCfg,"FD ROI DEMO");
	else if(pSysInfo->lan_config.democfg == 6)
	   sprintf(LiveSetting.demoCfg,"ROI CENTER DEMO");

	if(pSysInfo->lan_config.alarmstatus & 0xf) {
       sprintf(LiveSetting.notificationStatus, "1");
	}else {
       sprintf(LiveSetting.notificationStatus, "0");
	}

	if(pSysInfo->lan_config.alarmstatus & (1 << 2))
       sprintf(LiveSetting.recordStatus,"1");
	else
       sprintf(LiveSetting.recordStatus,"0");

	xadd_stag_attr(req, TAG_LiveSetting, RootAttrStr);
	xadd_elem(req, TAG_id, LiveSetting.id);
	xadd_elem(req, TAG_clickSnapFileName, LiveSetting.clickSnapFileName);
	xadd_elem(req, TAG_clickSnapStorage, LiveSetting.clickSnapStorage);
	xadd_elem(req, TAG_demoCfg, LiveSetting.demoCfg);
	xadd_elem(req, TAG_notificationStatus, LiveSetting.notificationStatus);
	xadd_elem(req, TAG_recordStatus, LiveSetting.recordStatus);
	xadd_etag(req, TAG_LiveSetting);

	return STATUS_OK;
}

int update_custom_live_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	LiveSetting_psia LiveSetting;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_LiveSetting) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(LiveSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_clickSnapFileName) == 0) {
			strcpy(LiveSetting.clickSnapFileName, tag->value);
			required |= xml_required_field(2);
			ControlSystemData(SFIELD_SET_CLICK_SNAP_FILENAME, &LiveSetting.clickSnapFileName, sizeof(LiveSetting.clickSnapFileName));
		}
		else if(strcmp(tag->name, TAG_clickSnapStorage) == 0) {
			strcpy(LiveSetting.clickSnapStorage, tag->value);
			required |= xml_required_field(3);
			value = atoi(LiveSetting.clickSnapStorage);
			ControlSystemData(SFIELD_SET_CLICK_SNAP_STORAGE, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_demoCfg) == 0) {
			strcpy(LiveSetting.demoCfg, tag->value);

			if(strncmp(LiveSetting.demoCfg,"NONE",4) == 0)
               strcpy(LiveSetting.demoCfg,"0");
			else if(strncmp(LiveSetting.demoCfg,"VS DEMO",4) == 0)
               strcpy(LiveSetting.demoCfg,"1");
			else if(strncmp(LiveSetting.demoCfg,"VNF DEMO",4) == 0)
               strcpy(LiveSetting.demoCfg,"2");
			else if(strncmp(LiveSetting.demoCfg,"LDC DEMO",4) == 0)
               strcpy(LiveSetting.demoCfg,"3");
			else if(strncmp(LiveSetting.demoCfg,"FD DEMO",4) == 0)
               strcpy(LiveSetting.demoCfg,"4");
			else if(strncmp(LiveSetting.demoCfg,"FD ROI DEMO",4) == 0)
               strcpy(LiveSetting.demoCfg,"5");
			else if(strncmp(LiveSetting.demoCfg,"ROI CENTER DEMO",4) == 0)
               strcpy(LiveSetting.demoCfg,"6");

			required |= xml_required_field(4);
			value = atoi(LiveSetting.demoCfg);
			ControlSystemData(SFIELD_SET_DEMOCFG, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_notificationStatus) == 0) {
			strcpy(LiveSetting.notificationStatus, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_recordStatus) == 0) {
			strcpy(LiveSetting.recordStatus, tag->value);
			required |= xml_required_field(6);
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fields were present */
	//if(required != xml_required_mask(6))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_live_channels_id_capabilities(request *req, node_t *node)
{
	const char *demoCfgStr = "opt=\"NONE,VS DEMO,VNF DEMO,LDC DEMO,FD DEMO,FD ROI DEMO,ROI CENTER DEMO\"";

	xadd_stag_attr(req, TAG_LiveSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_demoCfg, demoCfgStr);
	xadd_etag(req, TAG_demoCfg);
	xadd_etag(req, TAG_LiveSetting);

	return STATUS_OK;
}

int retrieve_custom_users(request *req, node_t *node)
{
	int i;

	xadd_stag_attr(req, TAG_UserSettingList, RootAttrStr);
	for(i = 0; i < ACOUNT_NUM; i++) {
		//req->list_index = i;
		//get_user_url(i, idstr);
		retrieve_custom_users_id(req, node);
		userIndex++;
	}
	userIndex = 0;
	xadd_etag(req, TAG_UserSettingList);

	return STATUS_OK;
}

int retrieve_custom_users_id(request *req, node_t *node)
{
	UserSetting_psia UserSetting;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(UserSetting.id,"%d",userIndex);
	sprintf(UserSetting.minNameLen,"4");
	sprintf(UserSetting.maxNameLen,"%d", USER_LEN - 1);
	sprintf(UserSetting.maxPwdLen,"%d", PASSWORD_LEN - 1);

	if(pSysInfo->acounts[userIndex].authority == 0)
	   sprintf(UserSetting.privilegeLevel,"Admin");
	else if(pSysInfo->acounts[userIndex].authority == 1)
	   sprintf(UserSetting.privilegeLevel,"Operator");
	else if(pSysInfo->acounts[userIndex].authority == 2)
	   sprintf(UserSetting.privilegeLevel,"Viewer");
	else if(pSysInfo->acounts[userIndex].authority == 3)
	   sprintf(UserSetting.privilegeLevel,"Superuser");

	xadd_stag_attr(req, TAG_UserSetting, RootAttrStr);
	xadd_elem(req, TAG_id, UserSetting.id);
	xadd_elem(req, TAG_minNameLen, UserSetting.minNameLen);
	xadd_elem(req, TAG_maxNameLen, UserSetting.maxNameLen);
	xadd_elem(req, TAG_maxPwdLen, UserSetting.maxPwdLen);
	xadd_elem(req, TAG_privilegeLevel, UserSetting.privilegeLevel);
	xadd_etag(req, TAG_UserSetting);

	return STATUS_OK;
}

int update_custom_users_id(request *req, node_t *node)
{
	int ret;
	tag_t  *tag;
	unsigned long required;
	UserSetting_psia UserSetting;
	//__u8 value;
	add_user_t account;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_UserSetting) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(UserSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_minNameLen) == 0) {
			strcpy(UserSetting.minNameLen, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_maxNameLen) == 0) {
			strcpy(UserSetting.maxNameLen, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_maxPwdLen) == 0) {
			strcpy(UserSetting.maxPwdLen, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_privilegeLevel) == 0) {
			strcpy(UserSetting.privilegeLevel, tag->value);
			required |= xml_required_field(5);
		}
		/* Protocol */
		tag = tag->next;
	}
	account.authority = atoi(UserSetting.privilegeLevel);

    ret = ControlSystemData(SFIELD_ADD_USER, (void *)&account, sizeof(account));
	/* check all the required fields were present */
	//if(required != xml_required_mask(5))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_users_id_capabilities(request *req, node_t *node)
{
    const char *privilegeLevelStr = "opt=\"Superuser,Admin,Operator,Viewer\"";

	xadd_stag_attr(req, TAG_UserSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_privilegeLevel, privilegeLevelStr);
	xadd_etag(req, TAG_privilegeLevel);
	xadd_etag(req, TAG_UserSetting);

	return STATUS_OK;
}

int retrieve_custom_video_channels(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();
	int c,channel_count = 0;

	xadd_stag_attr(req, TAG_VideoSettingList, RootAttrStr);
#if 1
	if(pSysInfo->lan_config.nStreamType == 0)
		channel_count = 1;
	else if (pSysInfo->lan_config.nStreamType == 1)
	    channel_count = 2;
	else if (pSysInfo->lan_config.nStreamType == 2)
	    channel_count = 3;
#endif

	for(c = 0; c < channel_count; c++) {
		retrieve_custom_video_channels_id(req, node);
		channel_id ++;
	}
	xadd_etag(req, TAG_VideoSettingList);
	channel_id = 0;

	return STATUS_OK;
}

int retrieve_custom_video_channels_id(request *req, node_t *node)
{
	VideoSetting_psia VideoSetting;
	SysInfo* pSysInfo = GetSysInfo();

	if(channel_id == 0){
	  sprintf(VideoSetting.detailInfoEnabled,"%d",pSysInfo->osd_config[0].nDetailInfo);
	  sprintf(VideoSetting.dateStampEnabled,"%d",pSysInfo->osd_config[0].dstampenable);

	  if(pSysInfo->lan_config.nStreamType == 0){

		if(pSysInfo->lan_config.nVideocombo == 0){
		  sprintf(VideoSetting.videoCodecSel,"H.264");
		  sprintf(VideoSetting.id,"2");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"H264:720");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"H264:D1");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 2){
		    sprintf(VideoSetting.videoResolutionSel,"H264:SXVGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 3){
		    sprintf(VideoSetting.videoResolutionSel,"H264:1080");
		  }
	    }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
	      sprintf(VideoSetting.videoCodecSel,"MPEG4");
		  sprintf(VideoSetting.id,"1");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:720");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:D1");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 2){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:SXVGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 3){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:1080");
		  }
	    }
	    else if (pSysInfo->lan_config.nVideocombo == 2){
	      sprintf(VideoSetting.videoCodecSel,"MegaPixel");
		  sprintf(VideoSetting.id,"0");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"JPG:1600X1200");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"JPG:2048X1536");
		  }
        }
	 }
	 else if (pSysInfo->lan_config.nStreamType == 1){

		if (pSysInfo->lan_config.nVideocombo == 0){
	      sprintf(VideoSetting.videoCodecSel,"H.264 + JPEG");
		  sprintf(VideoSetting.id,"2");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"H264:720 JPEG:VGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"H264:D1 JPEG:D1");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
	      sprintf(VideoSetting.videoCodecSel,"MPEG4 + JPEG");
		  sprintf(VideoSetting.id,"1");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:720 JPEG:VGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:D1 JPEG:D1");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 2){
	      sprintf(VideoSetting.videoCodecSel,"Dual H.264");
		  sprintf(VideoSetting.id,"2");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"H264:720 H264:QVGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"H264:D1 H264:D1");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 3){
	      sprintf(VideoSetting.videoCodecSel,"Dual MPEG4");
		  sprintf(VideoSetting.id,"1");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:720 MPEG4:QVGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:D1 MPEG4:D1");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 4){
	      sprintf(VideoSetting.videoCodecSel,"H264 + MPEG4");
		  sprintf(VideoSetting.id,"2");

		  sprintf(VideoSetting.videoResolutionSel,"H264:D1 MPEG4:D1");
        }
	 }
	 else if (pSysInfo->lan_config.nStreamType == 2){

		if (pSysInfo->lan_config.nVideocombo == 0){
	      sprintf(VideoSetting.videoCodecSel,"Dual H264 + JPEG");
		  sprintf(VideoSetting.id,"2");

		  sprintf(VideoSetting.videoResolutionSel,"H264:720 JPEG:VGA H264:QVGA");
        }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
	      sprintf(VideoSetting.videoCodecSel,"Dual MPEG4 + JPEG");
		  sprintf(VideoSetting.id,"1");

		  sprintf(VideoSetting.videoResolutionSel,"MPEG4:720 JPEG:VGA MPEG4:QVGA");
        }
	 }
    }
	else if(channel_id == 1){
	  sprintf(VideoSetting.detailInfoEnabled,"%d",pSysInfo->osd_config[1].nDetailInfo);
	  sprintf(VideoSetting.dateStampEnabled,"%d",pSysInfo->osd_config[1].dstampenable);

	  if (pSysInfo->lan_config.nStreamType == 1){

		if (pSysInfo->lan_config.nVideocombo == 0){
	      sprintf(VideoSetting.videoCodecSel,"H.264 + JPEG");
		  sprintf(VideoSetting.id,"0");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"H264:720 JPEG:VGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"H264:D1 JPEG:D1");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
	      sprintf(VideoSetting.videoCodecSel,"MPEG4 + JPEG");
		  sprintf(VideoSetting.id,"0");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:720 JPEG:VGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:D1 JPEG:D1");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 2){
	      sprintf(VideoSetting.videoCodecSel,"Dual H.264");
		  sprintf(VideoSetting.id,"2");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"H264:720 H264:QVGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"H264:D1 H264:D1");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 3){
	      sprintf(VideoSetting.videoCodecSel,"Dual MPEG4");
		  sprintf(VideoSetting.id,"1");

		  if(pSysInfo->lan_config.nVideocodecres == 0){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:720 MPEG4:QVGA");
		  }
		  else if(pSysInfo->lan_config.nVideocodecres == 1){
		    sprintf(VideoSetting.videoResolutionSel,"MPEG4:D1 MPEG4:D1");
		  }
        }
	    else if (pSysInfo->lan_config.nVideocombo == 4){
	      sprintf(VideoSetting.videoCodecSel,"H264 + MPEG4");
		  sprintf(VideoSetting.id,"1");

		  sprintf(VideoSetting.videoResolutionSel,"H264:D1 MPEG4:D1");
        }
	 }
	 else if (pSysInfo->lan_config.nStreamType == 2){

		sprintf(VideoSetting.id,"0");

		if (pSysInfo->lan_config.nVideocombo == 0){
	      sprintf(VideoSetting.videoCodecSel,"Dual H264 + JPEG");
		  sprintf(VideoSetting.videoResolutionSel,"H264:720 JPEG:VGA H264:QVGA");
        }
	    else if (pSysInfo->lan_config.nVideocombo == 1){
	      sprintf(VideoSetting.videoCodecSel,"Dual MPEG4 + JPEG");
		  sprintf(VideoSetting.videoResolutionSel,"MPEG4:720 JPEG:VGA MPEG4:QVGA");
        }
	 }
    }
	else if(channel_id == 2){
	  sprintf(VideoSetting.detailInfoEnabled,"%d",pSysInfo->osd_config[2].nDetailInfo);
	  sprintf(VideoSetting.dateStampEnabled,"%d",pSysInfo->osd_config[2].dstampenable);

	  if (pSysInfo->lan_config.nVideocombo == 0){
	     sprintf(VideoSetting.videoCodecSel,"Dual H264 + JPEG");
		 sprintf(VideoSetting.id,"2");
		 sprintf(VideoSetting.videoResolutionSel,"H264:720 JPEG:VGA H264:QVGA");
      }
	  else if (pSysInfo->lan_config.nVideocombo == 1){
	     sprintf(VideoSetting.videoCodecSel,"Dual MPEG4 + JPEG");
		 sprintf(VideoSetting.id,"1");
		 sprintf(VideoSetting.videoResolutionSel,"MPEG4:720 JPEG:VGA MPEG4:QVGA");
      }
    }

	if(pSysInfo->lan_config.nLocalDisplay == 0)
	   sprintf(VideoSetting.localDisplay,"OFF");
	else if(pSysInfo->lan_config.nLocalDisplay == 1)
	   sprintf(VideoSetting.localDisplay,"NTSC");
	else if(pSysInfo->lan_config.nLocalDisplay == 2)
	   sprintf(VideoSetting.localDisplay,"PAL");

	if(pSysInfo->lan_config.mirror == 0)
	   sprintf(VideoSetting.mirrorMode,"OFF");
	else if(pSysInfo->lan_config.mirror == 1)
	   sprintf(VideoSetting.mirrorMode,"HORIZONTAL");
	else if(pSysInfo->lan_config.mirror == 2)
	   sprintf(VideoSetting.mirrorMode,"VERTICAL");
	else if(pSysInfo->lan_config.mirror == 3)
	   sprintf(VideoSetting.mirrorMode,"BOTH");

	if(pSysInfo->sdcard_config.sdinsert==3){
	  sprintf(VideoSetting.captureLength,"NA");
	}
	else{
      if(pSysInfo->lan_config.aviduration == 0)
		 sprintf(VideoSetting.captureLength,"5 Sec");
	  else if(pSysInfo->lan_config.aviduration == 1)
         sprintf(VideoSetting.captureLength,"10 Sec");
	}

	xadd_stag_attr(req, TAG_VideoSetting, RootAttrStr);
	xadd_elem(req, TAG_id, VideoSetting.id);
	xadd_elem(req, TAG_videoCodecSel, VideoSetting.videoCodecSel);
	xadd_elem(req, TAG_videoResolutionSel, VideoSetting.videoResolutionSel);
	xadd_elem(req, TAG_localDisplay, VideoSetting.localDisplay);
	xadd_elem(req, TAG_mirrorMode, VideoSetting.mirrorMode);
	xadd_elem(req, TAG_captureLength, VideoSetting.captureLength);
	xadd_elem(req, TAG_dateStampEnabled, VideoSetting.dateStampEnabled);
	xadd_elem(req, TAG_detailInfoEnabled, VideoSetting.detailInfoEnabled);
	xadd_etag(req, TAG_VideoSetting);

	return STATUS_OK;
}

int retrieve_custom_video_channels_id_capabilities(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();

	const char *videoCodecTypeListStr;
	const char *videoResolutionWidthListStr;
	const char *videoResolutionHeightListStr;
	const char *captureLengthStr;
	const char *localDisplayStr = "opt=\"OFF,NTSC,PAL\"";
	const char *mirrorModestr = "opt=\"OFF,HORIZONTAL,VERTICAL,BOTH\"";
	char frameRateList1Str[360];
	char frameRateList2Str[360];
	char frameRateList3Str[360];

	videoCodecTypeListStr = "opt=\"H.264,MPEG4,MegaPixel @ H.264 + JPEG,MPEG4 + JPEG,Dual H.264,Dual MPEG4,H264 + MPEG4 @ Dual H264 + JPEG,Dual MPEG4 + JPEG\"";
    videoResolutionWidthListStr = "opt=\"H264:720,H264:D1,H264:SXVGA,H264:1080 @ MPEG4:720,MPEG4:D1,MPEG4:SXVGA,MPEG4:1080 @ JPG:2MP(1600x1200),JPG:3MP(2048x1536),JPG:5MP(2592x1920) @ H264:720 JPEG:VGA,H264:D1 JPEG:D1,H264:720 JPEG:720 @ MPEG4:720 JPEG:VGA,MPEG4:D1 JPEG:D1,MPEG4:720 JPEG:720 @ H264:720 H264:QVGA,H264:D1 H264:D1,H264:D1 H264:QVGA,H264:1080 H264:QVGA @ MPEG4:720 MPEG4:QVGA,MPEG4:D1 MPEG4:D1,MPEG4:D1 MPEG4:QVGA,MPEG4:1080 MPEG4:QVGA @ H264:D1 MPEG4:D1 @ H264:720 JPEG:VGA H264:QVGA @ MPEG4:720 JPEG:VGA MPEG4:QVGA\"";
	videoResolutionHeightListStr = "opt=\"1280X720,720X480,1280X960,1920X1072 @ 1280X720,720X480,1280X960,1920X1072 @ 1600X1200,2048X1536,2592X1920 @ 1280X720,640X352;720X480,720X480;1280X720,1280X720 @ 1280X720,640X352;720X480,720X480;1280X720,1280X720 @ 1280X720,320X192;720X480,720X480;720X480,320X192;1920X1072,320X192 @ 1280X720,320X192;720X480,720X480;720X480,320X192;1920X1072,320X192 @ 720X480,720X480 @ 1280X720,640X352,320X192 @ 1280X720,640X352,320X192\"";

	if (pSysInfo->lan_config.nDayNight == 0) {
		sprintf(frameRateList1Str, "opt=\"30,15,5;30,15,5;30,15,5;23,15,5 @ 30,15,5;30,15,5;30,15,5;23,15,5 @ 30,24,15,5;15,5;Auto @ 30,15,5;30,15,5;30,15,5 @ 30,15,5;30,15,5;30,15,5 @ 30,15,5;30,15,5;30,15,5;23,15,5 @ 30,15,5;30,15,5;30,15,5;23,15,5 @ 30,15,5 @ 30,15,5 @ 30,15,5\"");
	}
	else
	{
		char *resname_list1[] =
		{
			"opt=\"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8", "30,24,15,8;30,24,15,8;30,24,15,8;23,15,8", "30,24,15,8;15,12,8;Auto",
			"30,24,15,8;30,24,15,8;30,24,15,8", "30,24,15,8;30,24,15,8;30,24,15,8",
			"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8", "30,24,15,8;30,24,15,8;30,24,15,8;23,15,8",
			"30,24,15,8",
			"30,24,15,8", "30,24,15,8\""
		};

		int tblsize = sizeof(resname_list1)/sizeof(resname_list1[0]);
		int i = 0;
		char rtn_msg[360] = "\0";

		for (i = 0; i < tblsize; i++)
		{
		  if (i > 0)
			 strcat(rtn_msg, " @ ");
		  strcat(rtn_msg, resname_list1[i]);
		}
		sprintf(frameRateList1Str, "%s", rtn_msg);

    }

	if (pSysInfo->lan_config.nDayNight == 0) {
		sprintf(frameRateList2Str, "opt=\"NA;NA;NA;NA @ NA;NA;NA;NA @ NA;NA;NA @ 30,15,5;30,15,5;30,15,5 @ 30,15,5;30,15,5;30,15,5 @ 30,15,5;30,15,5;30,15,5;23,15,5 @ 30,15,5;30,15,5;30,15,5;23,15,5 @ 30,15,5 @ 30,15,5 @ 30,15,5\"");
	}
	else
	{
		char *resname_list2[] =
		{
			"opt=\"NA;NA;NA", "NA;NA;NA;NA", "NA;NA;NA",
			"30,24,15,8;30,24,15,8;30,24,15,8", "30,24,15,8;30,24,15,8;30,24,15,8",
			"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8", "30,24,15,8;30,24,15,8;30,24,15,8;23,15,8",
			"30,24,15,8",
			"30,24,15,8", "30,24,15,8\""
		};
		int tblsize = sizeof(resname_list2)/sizeof(resname_list2[0]);
		int i = 0;
		char rtn_msg[360] = "\0";

		for (i = 0; i < tblsize; i++)
		{
		  if (i > 0)
			strcat(rtn_msg, " @ ");  //opt=\"
		  strcat(rtn_msg, resname_list2[i]);
		}
		sprintf(frameRateList2Str, "%s", rtn_msg);

	}

	if (pSysInfo->lan_config.nDayNight == 0) {
		sprintf(frameRateList3Str, "opt=\"NA;NA;NA;NA @ NA;NA;NA;NA @ NA;NA;NA @ NA;NA;NA @ NA;NA;NA @ NA;NA;NA;NA @ NA;NA;NA;NA @ NA @ 30,15,5 @ 30,15,5\"");
	}
	else
	{
		char *resname_list3[] =
		{
			"opt=\"NA;NA;NA;NA", "NA;NA;NA;NA", "NA;NA;NA",
			"NA;NA;NA", "NA;NA;NA",
			"NA;NA;NA", "NA;NA;NA",
			"NA",
			"30,24,15,8", "30,24,15,8\""
		};
		int tblsize = sizeof(resname_list3)/sizeof(resname_list3[0]);
		int i = 0;
		char rtn_msg[360] = "\0";

		for (i = 0; i < tblsize; i++)
		{
		  if (i > 0)
			strcat(rtn_msg, " @ ");
				strcat(rtn_msg, resname_list3[i]);
		}
		sprintf(frameRateList3Str, "%s", rtn_msg);
	}

	if(pSysInfo->sdcard_config.sdinsert==3)
	   captureLengthStr = "opt=\"NA\"";
	else
	   captureLengthStr = "opt=\"5 Sec,10 Sec\"";

	xadd_stag_attr(req, TAG_VideoSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_videoCodecTypeList, videoCodecTypeListStr);
	xadd_etag(req, TAG_videoCodecTypeList);
	xadd_stag_attr(req, TAG_videoResolutionWidthList, videoResolutionWidthListStr);
	xadd_etag(req, TAG_videoResolutionWidthList);
	xadd_stag_attr(req, TAG_videoResolutionHeightList, videoResolutionHeightListStr);
	xadd_etag(req, TAG_videoResolutionHeightList);
	xadd_stag_attr(req, TAG_frameRateList1, frameRateList1Str);
	xadd_etag(req, TAG_frameRateList1);
	xadd_stag_attr(req, TAG_frameRateList2, frameRateList2Str);
	xadd_etag(req, TAG_frameRateList2);
	xadd_stag_attr(req, TAG_frameRateList3, frameRateList3Str);
	xadd_etag(req, TAG_frameRateList3);
	xadd_stag_attr(req, TAG_localDisplay, localDisplayStr);
	xadd_etag(req, TAG_localDisplay);
	xadd_stag_attr(req, TAG_mirrorMode, mirrorModestr);
	xadd_etag(req, TAG_mirrorMode);
	xadd_stag_attr(req, TAG_captureLength, captureLengthStr);
	xadd_etag(req, TAG_captureLength);
	xadd_etag(req, TAG_VideoSetting);

	return STATUS_OK;
}

int update_custom_video_channels(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_VideoSettingList) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_VideoSetting) == 0) {
			idflag = 1;
			update_custom_video_channels_id(req, node, tag);
		    required |= xml_required_field(1);
			channel_id++;
		}
	 /* Protocol */
		tag = tag->next;
	}
	idflag = 0;
	channel_id = 0;
	/* check all the required fileds were present */
	//if(required != xml_required_mask(1))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int update_custom_video_channels_id(request *req, node_t *node, tag_t *parentTag)
{
	SysInfo* pSysInfo = GetSysInfo();
	tag_t	*tag = parentTag;
	unsigned long required;
	VideoSetting_psia VideoSetting;
	__u8 value;

	fprintf(stderr,"\r\nUpdating Custom Video Channels...\n");

	if(idflag == 0){
	  // returns bytes processed or -1
	  if(xml_validate(req, req->inbuf, req->filesize) < 0)
		  return STATUS_INVALID_XML_FORMAT;

	  tag = req->root_tag;

	  if(!((tag) && (strcmp(tag->name, TAG_VideoSetting) == 0)))
		  return STATUS_INVALID_XML_CONTENT;
	}

	fprintf(stderr,"\r\nInside update_custom_video_channels_id with tag:%s\n",tag->name);
	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(VideoSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_videoCodecSel) == 0) {
			strcpy(VideoSetting.videoCodecSel, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_videoResolutionSel) == 0) {
			strcpy(VideoSetting.videoResolutionSel, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_localDisplay) == 0) {
			//fprintf(stderr,"\r\nInside TAG_localDisplay\n");
			strcpy(VideoSetting.localDisplay, tag->value);

			if(strncmp(VideoSetting.localDisplay,"OFF",3) == 0)
               strcpy(VideoSetting.localDisplay,"0");
			else if(strncmp(VideoSetting.localDisplay,"NTSC",3) == 0)
               strcpy(VideoSetting.localDisplay,"1");
			else if(strncmp(VideoSetting.localDisplay,"PAL",3) == 0)
               strcpy(VideoSetting.localDisplay,"2");

			required |= xml_required_field(4);
			value = atoi(VideoSetting.localDisplay);
			ControlSystemData(SFIELD_SET_LOCAL_DISPLAY, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_mirrorMode) == 0) {
			//fprintf(stderr,"\r\nInside TAG_mirrorMode\n");
			strcpy(VideoSetting.mirrorMode, tag->value);

			if(strncmp(VideoSetting.mirrorMode,"OFF",3) == 0)
               strcpy(VideoSetting.mirrorMode,"0");
			else if(strncmp(VideoSetting.mirrorMode,"HORIZONTAL",3) == 0)
               strcpy(VideoSetting.mirrorMode,"1");
			else if(strncmp(VideoSetting.mirrorMode,"VERTICAL",3) == 0)
               strcpy(VideoSetting.mirrorMode,"2");
			else if(strncmp(VideoSetting.mirrorMode,"BOTH",3) == 0)
               strcpy(VideoSetting.mirrorMode,"3");

			required |= xml_required_field(5);
			value = atoi(VideoSetting.mirrorMode);
			ControlSystemData(SFIELD_SET_MIRROR, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_captureLength) == 0) {
			//fprintf(stderr,"\r\nInside TAG_captureLength\n");
			strcpy(VideoSetting.captureLength, tag->value);

			if(strncmp(VideoSetting.captureLength,"5 Sec",5) == 0)
               strcpy(VideoSetting.captureLength,"0");
			else if(strncmp(VideoSetting.captureLength,"10 Sec",5) == 0)
               strcpy(VideoSetting.captureLength,"1");

			required |= xml_required_field(6);
			value = atoi(VideoSetting.captureLength);

			if(pSysInfo->sdcard_config.sdinsert != 1)
			  ControlSystemData(SFIELD_SET_AVIDURATION, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_dateStampEnabled) == 0) {
			//fprintf(stderr,"\r\nInside TAG_dateStampEnabled\n");
			strcpy(VideoSetting.dateStampEnabled, tag->value);
			required |= xml_required_field(7);
			value = atoi(VideoSetting.dateStampEnabled);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_DSTAMPENABLE1, &value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_DSTAMPENABLE2, &value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_DSTAMPENABLE3, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_detailInfoEnabled) == 0) {
			//fprintf(stderr,"\r\nInside TAG_detailInfoEnabled\n");
			strcpy(VideoSetting.detailInfoEnabled, tag->value);
			required |= xml_required_field(8);
			value = atoi(VideoSetting.detailInfoEnabled);

			if(channel_id ==0)
			  ControlSystemData(SFIELD_SET_DETAIL_INFO1, (void *)&value, sizeof(value));
			else if(channel_id == 1)
			  ControlSystemData(SFIELD_SET_DETAIL_INFO2, (void *)&value, sizeof(value));
			else if(channel_id == 2)
			  ControlSystemData(SFIELD_SET_DETAIL_INFO3, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fields were present */
	//if(required != xml_required_mask(8))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_camera_channels(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_CameraSettingList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
	    retrieve_custom_camera_channels_id(req, node);
	xadd_etag(req, TAG_CameraSettingList);

	return STATUS_OK;
}

int retrieve_custom_camera_channels_id(request *req, node_t *node)
{
	CameraSetting_psia CameraSetting;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(CameraSetting.id,"0");

	if(pSysInfo->lan_config.expPriority == 0)
	   sprintf(CameraSetting.exposurePriority,"Priortize FrameRate");
	else if(pSysInfo->lan_config.expPriority == 1)
	   sprintf(CameraSetting.exposurePriority,"Priortize Exposure");

	sprintf(CameraSetting.blc,"%d",pSysInfo->lan_config.nBacklightControl);

	if(pSysInfo->lan_config.nBackLight == 0)
	   sprintf(CameraSetting.backlight,"Min");
	else if(pSysInfo->lan_config.nBackLight == 1)
	   sprintf(CameraSetting.backlight,"Mid");
	else if(pSysInfo->lan_config.nBackLight == 2)
	   sprintf(CameraSetting.backlight,"Max");

	sprintf(CameraSetting.dynRange,"%d",pSysInfo->lan_config.dynRange);
	sprintf(CameraSetting.histDisplay,"%d",pSysInfo->lan_config.histogram);
	sprintf(CameraSetting.vsEnabled,"%d",(pSysInfo->lan_config.AdvanceMode&FFLAG_VS)?1:0);
	sprintf(CameraSetting.ldcEnabled,"%d",(pSysInfo->lan_config.AdvanceMode&FFLAG_LDC)?1:0);

	if(pSysInfo->lan_config.nBinning == 0)
	   sprintf(CameraSetting.sensorMode,"binning");
	else if(pSysInfo->lan_config.nBinning == 1)
	   sprintf(CameraSetting.sensorMode,"skipping");
	else if(pSysInfo->lan_config.nBinning == 2)
	   sprintf(CameraSetting.sensorMode,"window");

	if(pSysInfo->lan_config.nAEWswitch == 0)
	   sprintf(CameraSetting.AWBEngine,"NONE");
	else if(pSysInfo->lan_config.nAEWswitch == 1)
	   sprintf(CameraSetting.AWBEngine,"APPRO");
	else if(pSysInfo->lan_config.nAEWswitch == 2)
	   sprintf(CameraSetting.AWBEngine,"TI");

	if(pSysInfo->lan_config.nAEWtype == 0)
	   sprintf(CameraSetting.AWBMode,"OFF");
	else if(pSysInfo->lan_config.nAEWtype == 1)
	   sprintf(CameraSetting.AWBMode,"Auto Exposure");
	else if(pSysInfo->lan_config.nAEWtype == 2)
	   sprintf(CameraSetting.AWBMode,"Auto White Balance");
	else if(pSysInfo->lan_config.nAEWtype == 3)
	   sprintf(CameraSetting.AWBMode,"Auto Exposure + Auto White Balance");

	if(pSysInfo->lan_config.AdvanceMode&FFLAG_SNF)
	   sprintf(CameraSetting.snfltCtrl,"ON");
	else
	   sprintf(CameraSetting.snfltCtrl,"OFF");

	sprintf(CameraSetting.tnfltCtrl,"%d",(pSysInfo->lan_config.AdvanceMode&FFLAG_TNF)?1:0);

	xadd_stag_attr(req, TAG_CameraSetting, RootAttrStr);
	xadd_elem(req, TAG_id, CameraSetting.id);
	xadd_elem(req, TAG_exposurePriority, CameraSetting.exposurePriority);
	xadd_elem(req, TAG_blc, CameraSetting.blc);
	xadd_elem(req, TAG_backlight, CameraSetting.backlight);
	xadd_elem(req, TAG_dynRange, CameraSetting.dynRange);
	xadd_elem(req, TAG_histDisplay, CameraSetting.histDisplay);
	xadd_elem(req, TAG_vsEnabled, CameraSetting.vsEnabled);
	xadd_elem(req, TAG_ldcEnabled, CameraSetting.ldcEnabled);
	xadd_elem(req, TAG_sensorMode, CameraSetting.sensorMode);
	xadd_elem(req, TAG_AWBEngine, CameraSetting.AWBEngine);
	xadd_elem(req, TAG_AWBMode, CameraSetting.AWBMode);
	xadd_elem(req, TAG_snfltCtrl, CameraSetting.snfltCtrl);
	xadd_elem(req, TAG_tnfltCtrl, CameraSetting.tnfltCtrl);
	xadd_etag(req, TAG_CameraSetting);

	return STATUS_OK;
}

int update_custom_camera_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	CameraSetting_psia CameraSetting;
	__u8 value;

	//fprintf(stderr,"\r\nInside update_system_video_channels_id\n");

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_CameraSetting) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(CameraSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_exposurePriority) == 0) {
			strcpy(CameraSetting.exposurePriority, tag->value);

			if(strncmp(CameraSetting.exposurePriority,"Priortize FrameRate",15) == 0)
               strcpy(CameraSetting.exposurePriority,"0");
			else if(strncmp(CameraSetting.exposurePriority,"Priortize Exposure",15) == 0)
               strcpy(CameraSetting.exposurePriority,"1");

			required |= xml_required_field(2);
			value = atoi(CameraSetting.exposurePriority);
			ControlSystemData(SFIELD_SET_EXPPRIORITY, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_blc) == 0) {
			strcpy(CameraSetting.blc, tag->value);
			required |= xml_required_field(3);
			value = atoi(CameraSetting.blc);
			ControlSystemData(SFIELD_SET_BLC, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_backlight) == 0) {
			strcpy(CameraSetting.backlight, tag->value);

			if(strncmp(CameraSetting.backlight,"Min",3) == 0)
               strcpy(CameraSetting.backlight,"0");
			else if(strncmp(CameraSetting.backlight,"Mid",3) == 0)
               strcpy(CameraSetting.backlight,"1");
			else if(strncmp(CameraSetting.backlight,"Max",3) == 0)
               strcpy(CameraSetting.backlight,"2");

			required |= xml_required_field(4);
			value = atoi(CameraSetting.backlight);
			ControlSystemData(SFIELD_SET_BACKLIGHT, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_dynRange) == 0) {
			strcpy(CameraSetting.dynRange, tag->value);
			required |= xml_required_field(5);
			value = atoi(CameraSetting.dynRange);
			ControlSystemData(SFIELD_SET_DYNRANGE, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_histDisplay) == 0) {
			strcpy(CameraSetting.histDisplay, tag->value);
			required |= xml_required_field(6);
			value = atoi(CameraSetting.histDisplay);
			ControlSystemData(SFIELD_SET_HISTOGRAM, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_vsEnabled) == 0) {
			strcpy(CameraSetting.vsEnabled, tag->value);
			required |= xml_required_field(7);
			value = atoi(CameraSetting.vsEnabled);
			ControlSystemData(SFIELD_SET_VSTAB, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_ldcEnabled) == 0) {
			strcpy(CameraSetting.ldcEnabled, tag->value);
			required |= xml_required_field(8);
			value = atoi(CameraSetting.ldcEnabled);
			ControlSystemData(SFIELD_SET_LDC, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_sensorMode) == 0) {
			strcpy(CameraSetting.sensorMode, tag->value);

			if(strncmp(CameraSetting.sensorMode,"Binning",3) == 0)
               strcpy(CameraSetting.sensorMode,"0");
			else if(strncmp(CameraSetting.sensorMode,"Skipping",3) == 0)
               strcpy(CameraSetting.sensorMode,"1");
			else if(strncmp(CameraSetting.sensorMode,"Window",2) == 0)
               strcpy(CameraSetting.sensorMode,"2");

			required |= xml_required_field(9);
			value = atoi(CameraSetting.sensorMode);
			ControlSystemData(SFIELD_SET_BINNING, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_AWBEngine) == 0) {
			strcpy(CameraSetting.AWBEngine, tag->value);

			if(strncmp(CameraSetting.AWBEngine,"OFF",3) == 0)
               strcpy(CameraSetting.AWBEngine,"0");
			else if(strncmp(CameraSetting.AWBEngine,"APPRO",3) == 0)
               strcpy(CameraSetting.AWBEngine,"1");
			else if(strncmp(CameraSetting.AWBEngine,"TI",2) == 0)
               strcpy(CameraSetting.AWBEngine,"2");

			required |= xml_required_field(10);
			value = atoi(CameraSetting.AWBEngine);
			ControlSystemData(SFIELD_SET_IMAGE2A, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_AWBMode) == 0) {
			strcpy(CameraSetting.AWBMode, tag->value);
			required |= xml_required_field(11);

			if(strncmp(CameraSetting.AWBMode,"OFF",3) == 0)
               strcpy(CameraSetting.AWBMode,"0");
			else if(strncmp(CameraSetting.AWBMode,"Auto Exposure",10) == 0)
               strcpy(CameraSetting.AWBMode,"1");
			else if(strncmp(CameraSetting.AWBMode,"Auto White Balance",10) == 0)
               strcpy(CameraSetting.AWBMode,"2");
			else if(strncmp(CameraSetting.AWBMode,"Auto Exposure + Auto White Balance",20) == 0)
               strcpy(CameraSetting.AWBMode,"3");

			value = atoi(CameraSetting.AWBMode);
			ControlSystemData(SFIELD_SET_IMAGE2ATYPE, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_snfltCtrl) == 0) {
			strcpy(CameraSetting.snfltCtrl, tag->value);

			if(strncmp(CameraSetting.snfltCtrl,"OFF",2) == 0)
               strcpy(CameraSetting.snfltCtrl,"0");
			else if(strncmp(CameraSetting.snfltCtrl,"ON",2) == 0)
               strcpy(CameraSetting.snfltCtrl,"1");

			required |= xml_required_field(12);
			value = atoi(CameraSetting.snfltCtrl);
			ControlSystemData(SFIELD_SET_SNF, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_tnfltCtrl) == 0) {
			strcpy(CameraSetting.tnfltCtrl, tag->value);
			required |= xml_required_field(13);
			value = atoi(CameraSetting.tnfltCtrl);
			ControlSystemData(SFIELD_SET_TNF, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fields were present */
	//if(required != xml_required_mask(13))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_camera_channels_id_capabilities(request *req, node_t *node)
{
	const char *exposurePriorityStr = "opt=\"Priortize FrameRate,Priortize Exposure\"";
	const char *backlightStr = "opt=\"Min,Mid,Max\"";
	const char *sensorModeStr = "opt=\"binning,skipping,window\"";
	const char *AWBEngineStr = "opt=\"NONE,APPRO,TI\"";
	const char *AWBModeStr = "opt=\"OFF,Auto Exposure,Auto White Balance,Auto Exposure + Auto White Balance\"";
	const char *snfltCtrlStr = "opt=\"OFF,ON\"";

	xadd_stag_attr(req, TAG_CameraSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_exposurePriority, exposurePriorityStr);
	xadd_etag(req, TAG_exposurePriority);
	xadd_stag_attr(req, TAG_backlight, backlightStr);
	xadd_etag(req, TAG_backlight);
	xadd_stag_attr(req, TAG_sensorMode, sensorModeStr);
	xadd_etag(req, TAG_sensorMode);
	xadd_stag_attr(req, TAG_AWBEngine, AWBEngineStr);
	xadd_etag(req, TAG_AWBEngine);
	xadd_stag_attr(req, TAG_AWBMode, AWBModeStr);
	xadd_etag(req, TAG_AWBMode);
	xadd_stag_attr(req, TAG_snfltCtrl, snfltCtrlStr);
	xadd_etag(req, TAG_snfltCtrl);
	xadd_etag(req, TAG_CameraSetting);

	return STATUS_OK;
}

int retrieve_custom_audio_channels(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_AudioSettingList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
	    retrieve_custom_audio_channels_id(req, node);
	xadd_etag(req, TAG_AudioSettingList);

	return STATUS_OK;
}

int retrieve_custom_audio_channels_id(request *req, node_t *node)
{
	AudioSetting_psia AudioSetting;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(AudioSetting.id,"0");
	sprintf(AudioSetting.inputVolume,"%d",pSysInfo->audio_config.audioinvolume);
	sprintf(AudioSetting.alarmLevel,"%d",pSysInfo->audio_config.alarmlevel);

	xadd_stag_attr(req, TAG_AudioSetting, RootAttrStr);
	xadd_elem(req, TAG_id, AudioSetting.id);
	xadd_elem(req, TAG_inputVolume, AudioSetting.inputVolume);
	xadd_elem(req, TAG_alarmLevel, AudioSetting.alarmLevel);
	xadd_etag(req, TAG_AudioSetting);

	return STATUS_OK;
}

int update_custom_audio_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	AudioSetting_psia AudioSetting;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_AudioSetting) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(AudioSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_inputVolume) == 0) {
			strcpy(AudioSetting.inputVolume, tag->value);
			required |= xml_required_field(2);
			value = atoi(AudioSetting.inputVolume);
			ControlSystemData(SFIELD_SET_AUDIOINVOLUME, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_alarmLevel) == 0) {
			strcpy(AudioSetting.alarmLevel, tag->value);
			required |= xml_required_field(3);
			value = atoi(AudioSetting.alarmLevel);
			ControlSystemData(SFIELD_SET_AUDIO_ALARMLEVEL, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fields were present */
	//if(required != xml_required_mask(3))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_datetime_channels(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_DateTimeSettingList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
	    retrieve_custom_datetime_channels_id(req, node);
	xadd_etag(req, TAG_DateTimeSettingList);

	return STATUS_OK;
}

int retrieve_custom_datetime_channels_id(request *req, node_t *node)
{
	DateTimeSetting_psia DateTimeSetting;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(DateTimeSetting.id,"0");
	sprintf(DateTimeSetting.daylight,"%d",pSysInfo->lan_config.net.daylight_time);

	if(pSysInfo->lan_config.dateposition == 0)
	  sprintf(DateTimeSetting.datePosition,"Bottom-Left");
	else if(pSysInfo->lan_config.dateposition == 1)
      sprintf(DateTimeSetting.datePosition,"Bottom-Right");

	if(pSysInfo->lan_config.timeposition == 0)
	  sprintf(DateTimeSetting.timePosition,"Bottom-Left");
	else if(pSysInfo->lan_config.timeposition == 1)
	  sprintf(DateTimeSetting.timePosition,"Bottom-Right");

	if(pSysInfo->lan_config.dateformat == 0)
	   sprintf(DateTimeSetting.dateFormat,"YYYY/MM/DD");
	else if(pSysInfo->lan_config.dateformat == 1)
	   sprintf(DateTimeSetting.dateFormat,"MM/DD/YYYY");
	else if(pSysInfo->lan_config.dateformat == 2)
	   sprintf(DateTimeSetting.dateFormat,"DD/MM/YYYY");

	if(pSysInfo->lan_config.tstampformat == 0)
	   sprintf(DateTimeSetting.timeFormat,"12Hrs");
	else if(pSysInfo->lan_config.tstampformat == 1)
	   sprintf(DateTimeSetting.timeFormat,"24Hrs");

	xadd_stag_attr(req, TAG_DateTimeSetting, RootAttrStr);
	xadd_elem(req, TAG_id, DateTimeSetting.id);
	xadd_elem(req, TAG_daylight, DateTimeSetting.daylight);
	xadd_elem(req, TAG_dateFormat, DateTimeSetting.dateFormat);
	xadd_elem(req, TAG_timeFormat, DateTimeSetting.timeFormat);
	xadd_elem(req, TAG_datePosition, DateTimeSetting.datePosition);
	xadd_elem(req, TAG_timePosition, DateTimeSetting.timePosition);
	xadd_etag(req, TAG_DateTimeSetting);

	return STATUS_OK;
}

int update_custom_datetime_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	DateTimeSetting_psia DateTimeSetting;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_DateTimeSetting) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(DateTimeSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_daylight) == 0) {
			strcpy(DateTimeSetting.daylight, tag->value);
			required |= xml_required_field(2);
			value = atoi(DateTimeSetting.daylight);
			ControlSystemData(SFIELD_SET_DAYLIGHT, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_dateFormat) == 0) {
			strcpy(DateTimeSetting.dateFormat, tag->value);

			if(strncmp(DateTimeSetting.dateFormat,"YYYY/MM/DD",4) == 0)
               strcpy(DateTimeSetting.dateFormat,"0");
			else if(strncmp(DateTimeSetting.dateFormat,"MM/DD/YYYY",4) == 0)
               strcpy(DateTimeSetting.dateFormat,"1");
			else if(strncmp(DateTimeSetting.dateFormat,"DD/MM/YYYY",4) == 0)
               strcpy(DateTimeSetting.dateFormat,"2");

			required |= xml_required_field(3);
			value = atoi(DateTimeSetting.dateFormat);
			ControlSystemData(SFIELD_SET_DATEFORMAT, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_timeFormat) == 0) {
			strcpy(DateTimeSetting.timeFormat, tag->value);

			if(strncmp(DateTimeSetting.timeFormat,"12Hrs",3) == 0)
               strcpy(DateTimeSetting.timeFormat,"0");
			else if(strncmp(DateTimeSetting.timeFormat,"24Hrs",3) == 0)
               strcpy(DateTimeSetting.timeFormat,"1");

			required |= xml_required_field(4);
			value = atoi(DateTimeSetting.timeFormat);
			ControlSystemData(SFIELD_SET_TSTAMPFORMAT, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_datePosition) == 0) {
			strcpy(DateTimeSetting.datePosition, tag->value);

			if(strncmp(DateTimeSetting.datePosition,"Bottom-Left",10) == 0)
               strcpy(DateTimeSetting.datePosition,"0");
			else if(strncmp(DateTimeSetting.datePosition,"Bottom-Right",10) == 0)
               strcpy(DateTimeSetting.datePosition,"1");

			required |= xml_required_field(5);
			value = atoi(DateTimeSetting.datePosition);
			ControlSystemData(SFIELD_SET_DATEPPOS, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_timePosition) == 0) {
			strcpy(DateTimeSetting.timePosition, tag->value);

			if(strncmp(DateTimeSetting.timePosition,"Bottom-Left",10) == 0)
               strcpy(DateTimeSetting.timePosition,"0");
			else if(strncmp(DateTimeSetting.timePosition,"Bottom-Right",10) == 0)
               strcpy(DateTimeSetting.timePosition,"1");

			required |= xml_required_field(6);
			value = atoi(DateTimeSetting.timePosition);
			ControlSystemData(SFIELD_SET_TIMEPOS, &value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fields were present */
	//if(required != xml_required_mask(6))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_datetime_channels_id_capabilities(request *req, node_t *node)
{
	const char *dateFormatStr = "opt=\"YYYY/MM/DD,MM/DD/YYYY,DD/MM/YYYY\"";
	const char *timeFormatStr = "opt=\"12Hrs,24Hrs\"";
	const char *datePositionStr = "opt=\"Bottom-Left,Bottom-Right\"";
	const char *timePositionStr = "opt=\"Bottom-Left,Bottom-Right\"";

	xadd_stag_attr(req, TAG_DateTimeSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_dateFormat, dateFormatStr);
	xadd_etag(req, TAG_dateFormat);
	xadd_stag_attr(req, TAG_timeFormat, timeFormatStr);
	xadd_etag(req, TAG_timeFormat);
	xadd_stag_attr(req, TAG_datePosition, datePositionStr);
	xadd_etag(req, TAG_datePosition);
	xadd_stag_attr(req, TAG_timePosition, timePositionStr);
	xadd_etag(req, TAG_timePosition);
	xadd_etag(req, TAG_DateTimeSetting);

	return STATUS_OK;
}

int retrieve_custom_network_channels(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_NetworkSettingList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
	    retrieve_custom_network_channels_id(req, node);
	xadd_etag(req, TAG_NetworkSettingList);

	return STATUS_OK;
}

int retrieve_custom_network_channels_id(request *req, node_t *node)
{
	NetworkSetting_psia NetworkSetting;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(NetworkSetting.id,"0");
	sprintf(NetworkSetting.httpsPortNo,"%d",pSysInfo->lan_config.net.https_port);

	if(pSysInfo->lan_config.rs485config == 0)
        sprintf(NetworkSetting.serialPortMode,"OFF");

	xadd_stag_attr(req, TAG_NetworkSetting, RootAttrStr);
	xadd_elem(req, TAG_id, NetworkSetting.id);
	xadd_elem(req, TAG_httpsPortNo, NetworkSetting.httpsPortNo);
	xadd_elem(req, TAG_serialPortMode, NetworkSetting.serialPortMode);
	xadd_etag(req, TAG_NetworkSetting);

	return STATUS_OK;
}

int update_custom_network_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	NetworkSetting_psia NetworkSetting;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_NetworkSetting) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(NetworkSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_httpsPortNo) == 0) {
			strcpy(NetworkSetting.httpsPortNo, tag->value);
			required |= xml_required_field(2);
			value = atoi(NetworkSetting.httpsPortNo);
            ControlSystemData(SFIELD_SET_HTTPSPORT, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_serialPortMode) == 0) {
			strcpy(NetworkSetting.serialPortMode, tag->value);

			if(strncmp(NetworkSetting.serialPortMode,"OFF",3) == 0)
               strcpy(NetworkSetting.serialPortMode,"0");

			required |= xml_required_field(3);
			value = atoi(NetworkSetting.serialPortMode);
			ControlSystemData(SFIELD_SET_RS485, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fields were present */
	//if(required != xml_required_mask(3))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_network_channels_id_capabilities(request *req, node_t *node)
{
	const char *serialPortModeStr = "opt=\"OFF\"";

	xadd_stag_attr(req, TAG_NetworkSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_serialPortMode, serialPortModeStr);
	xadd_etag(req, TAG_serialPortMode);
	xadd_etag(req, TAG_NetworkSetting);

	return STATUS_OK;
}

int retrieve_custom_alarm_triggers(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_AlarmSettingList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
	    retrieve_custom_alarm_triggers_id(req, node);
	xadd_etag(req, TAG_AlarmSettingList);

	return STATUS_OK;
}

int retrieve_custom_alarm_triggers_id(request *req, node_t *node)
{
	AlarmSetting_psia AlarmSetting;
	SysInfo* pSysInfo = GetSysInfo();
	int supportMpeg4=0,supportMpeg4cif=0 ,supportH264=0, supportH264cif=0,supportJPG=0;
	int supportAVI=0;

	supportJPG = pSysInfo->lan_config.Supportstream1;
	supportMpeg4 = pSysInfo->lan_config.Supportstream2;
	supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
	supportH264 = pSysInfo->lan_config.Supportstream5;
	supportH264cif = pSysInfo->lan_config.Supportstream6;
	//int mpeg41Xsize= pSysInfo->lan_config.Mpeg41Xsize;
	//int mpeg41Ysize= pSysInfo->lan_config.Mpeg41Ysize;
	//int mpeg42Xsize=pSysInfo->lan_config.Mpeg42Xsize;
	//int mpeg42Ysize=pSysInfo->lan_config.Mpeg42Ysize;
	//int avc1Xsize=pSysInfo->lan_config.Avc1Xsize;
	//int avc1Ysize=pSysInfo->lan_config.Avc1Ysize;
	//int avc2Xsize=pSysInfo->lan_config.Avc2Xsize;
	//int avc2Ysize=pSysInfo->lan_config.Avc2Ysize;

	supportAVI = supportH264|supportH264cif|supportMpeg4|supportMpeg4cif;

	sprintf(AlarmSetting.id,"0");
	sprintf(AlarmSetting.enabled,"%d",pSysInfo->lan_config.nAlarmEnable);

	if(pSysInfo->lan_config.gioinenable == 0)
        sprintf(AlarmSetting.inputType,"OFF");
	else if(pSysInfo->lan_config.gioinenable == 1)
        sprintf(AlarmSetting.inputType,"ON");

	if(pSysInfo->lan_config.giooutenable == 0)
        sprintf(AlarmSetting.outputType,"OFF");
	else if(pSysInfo->lan_config.giooutenable == 1)
        sprintf(AlarmSetting.outputType,"ON");

	if (pSysInfo->sdcard_config.sdinsert==3) {
       sprintf(AlarmSetting.ftpFileFormatType,"JPEG");
	   sprintf(AlarmSetting.attachedFileFormatType,"JPEG");
	}
	else{
	   if ((supportAVI&&supportJPG)){
		 if(pSysInfo->ftp_config.ftpfileformat == 0)
	         sprintf(AlarmSetting.ftpFileFormatType,"AVI");
	     else if(pSysInfo->ftp_config.ftpfileformat == 1)
	         sprintf(AlarmSetting.ftpFileFormatType,"JPEG");

		 if(pSysInfo->smtp_config.attfileformat == 0)
	         sprintf(AlarmSetting.attachedFileFormatType,"AVI");
	     else if(pSysInfo->smtp_config.attfileformat == 1)
	         sprintf(AlarmSetting.attachedFileFormatType,"JPEG");
	   }
	   else if (supportAVI){
		 sprintf(AlarmSetting.ftpFileFormatType,"AVI");
		 sprintf(AlarmSetting.attachedFileFormatType,"AVI");
	   }
	   else if (supportJPG){
		 sprintf(AlarmSetting.ftpFileFormatType,"JPEG");
		 sprintf(AlarmSetting.attachedFileFormatType,"JPEG");
	   }
	}

	if ((supportAVI&&supportJPG)){
	  if(pSysInfo->sdcard_config.sdfileformat == 0)
	     sprintf(AlarmSetting.sdFileFormatType,"AVI");
	  else if(pSysInfo->sdcard_config.sdfileformat == 1)
	     sprintf(AlarmSetting.sdFileFormatType,"JPEG");
	}
	else if (supportAVI){
		sprintf(AlarmSetting.sdFileFormatType,"AVI");
	}
	else if (supportJPG){
		sprintf(AlarmSetting.sdFileFormatType,"JPEG");
	}

	sprintf(AlarmSetting.numFilesAttach,"%d",pSysInfo->smtp_config.attachments);
	sprintf(AlarmSetting.minFilesAttach,"1");
	sprintf(AlarmSetting.maxFilesAttach,"20");
	sprintf(AlarmSetting.localstorageneabled,"%d",pSysInfo->sdcard_config.sdrenable);

	if(pSysInfo->lan_config.nAlarmAudioFile == 0)
	   sprintf(AlarmSetting.alarmAudioFile,"alarm_1.wav");
	else if(pSysInfo->lan_config.nAlarmAudioFile == 1)
	   sprintf(AlarmSetting.alarmAudioFile,"alarm_2.wav");

	xadd_stag_attr(req, TAG_AlarmSetting, RootAttrStr);
	xadd_elem(req, TAG_id, AlarmSetting.id);
	xadd_elem(req, TAG_enabled, AlarmSetting.enabled);
	xadd_elem(req, TAG_inputType, AlarmSetting.inputType);
	xadd_elem(req, TAG_outputType, AlarmSetting.outputType);
	xadd_elem(req, TAG_ftpFileFormatType, AlarmSetting.ftpFileFormatType);
	xadd_elem(req, TAG_attachedFileFormatType, AlarmSetting.attachedFileFormatType);
	xadd_elem(req, TAG_numFilesAttach, AlarmSetting.numFilesAttach);
	xadd_elem(req, TAG_minFilesAttach, AlarmSetting.minFilesAttach);
	xadd_elem(req, TAG_maxFilesAttach, AlarmSetting.maxFilesAttach);
	xadd_elem(req, TAG_localstorageneabled, AlarmSetting.localstorageneabled);
	xadd_elem(req, TAG_sdFileFormatType, AlarmSetting.sdFileFormatType);
	xadd_elem(req, TAG_alarmAudioFile, AlarmSetting.alarmAudioFile);
	xadd_etag(req, TAG_AlarmSetting);

	return STATUS_OK;
}

int update_custom_alarm_triggers_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	AlarmSetting_psia AlarmSetting;
	__u8 value;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_AlarmSetting) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(AlarmSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(AlarmSetting.enabled, tag->value);
			required |= xml_required_field(2);
			value = atoi(AlarmSetting.enabled);
            ControlSystemData(SFIELD_SET_ALARM_ENABLE, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_inputType) == 0) {
			strcpy(AlarmSetting.inputType, tag->value);

			if(strncmp(AlarmSetting.inputType,"OFF",2) == 0)
               strcpy(AlarmSetting.inputType,"0");
			else if(strncmp(AlarmSetting.inputType,"ON",2) == 0)
               strcpy(AlarmSetting.inputType,"1");

			required |= xml_required_field(3);
			value = atoi(AlarmSetting.inputType);
			ControlSystemData(SFIELD_SET_GIOINENABLE, &value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_outputType) == 0) {
			strcpy(AlarmSetting.outputType, tag->value);

			if(strncmp(AlarmSetting.outputType,"OFF",2) == 0)
               strcpy(AlarmSetting.outputType,"0");
			else if(strncmp(AlarmSetting.outputType,"ON",2) == 0)
               strcpy(AlarmSetting.outputType,"1");

			required |= xml_required_field(4);
			value = atoi(AlarmSetting.outputType);
			ControlSystemData(SFIELD_SET_GIOOUTENABLE, &value, sizeof(value));
		}
	    else if(strcmp(tag->name, TAG_ftpFileFormatType) == 0) {
			strcpy(AlarmSetting.ftpFileFormatType, tag->value);
			required |= xml_required_field(5);
			value = atoi(AlarmSetting.ftpFileFormatType);
			//ControlSystemData(SFIELD_SET_FTPFILEFORMAT, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_attachedFileFormatType) == 0) {
			strcpy(AlarmSetting.attachedFileFormatType, tag->value);
			required |= xml_required_field(6);
			value = atoi(AlarmSetting.attachedFileFormatType);
			//ControlSystemData(SFIELD_SET_ATTFILEFORMAT, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_numFilesAttach) == 0) {
			strcpy(AlarmSetting.numFilesAttach, tag->value);
			required |= xml_required_field(7);
			value = atoi(AlarmSetting.numFilesAttach);
			ControlSystemData(SFIELD_SET_SMTP_ATTACHMENTS, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_minFilesAttach) == 0) {
			strcpy(AlarmSetting.minFilesAttach, tag->value);
			required |= xml_required_field(8);
		}
		else if(strcmp(tag->name, TAG_maxFilesAttach) == 0) {
			strcpy(AlarmSetting.maxFilesAttach, tag->value);
			required |= xml_required_field(9);
		}
		else if(strcmp(tag->name, TAG_localstorageneabled) == 0) {
			strcpy(AlarmSetting.localstorageneabled, tag->value);
			required |= xml_required_field(10);
			value = atoi(AlarmSetting.localstorageneabled);
			ControlSystemData(SFIELD_SET_SD_SDRENABLE, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_sdFileFormatType) == 0) {
			strcpy(AlarmSetting.sdFileFormatType, tag->value);
			required |= xml_required_field(11);
			value = atoi(AlarmSetting.sdFileFormatType);
			//ControlSystemData(SFIELD_SET_SDFILEFORMAT, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_alarmAudioFile) == 0) {
			strcpy(AlarmSetting.alarmAudioFile, tag->value);

			if(strncmp(AlarmSetting.alarmAudioFile,"alarm_1.wav",10) == 0)
               strcpy(AlarmSetting.alarmAudioFile,"0");
			else if(strncmp(AlarmSetting.alarmAudioFile,"alarm_2.wav",10) == 0)
               strcpy(AlarmSetting.alarmAudioFile,"1");

			required |= xml_required_field(12);
			value = atoi(AlarmSetting.alarmAudioFile);
			ControlSystemData(SFIELD_SET_ALARM_AUDIOFILE, (void *)&value, sizeof(value));
		}
		/* Protocol */
		tag = tag->next;
	}
	/* check all the required fields were present */
	//if(required != xml_required_mask(12))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_alarm_triggers_id_capabilities(request *req, node_t *node)
{
	SysInfo* pSysInfo = GetSysInfo();

	const char *inputTypeStr = "opt=\"OFF,ON\"";
	const char *outputTypeStr = "opt=\"OFF,ON\"";
	const char *alarmAudioFileStr = "opt=\"alarm_1.wav,alarm_2.wav\"";
	const char *ftpFileFormatTypeStr = NULL;
	const char *attachedFileFormatTypeStr = NULL;
	const char *sdFileFormatTypeStr = NULL;
	int supportMpeg4=0,supportMpeg4cif=0 ,supportH264=0, supportH264cif=0,supportJPG=0;
	int supportAVI=0;

	supportJPG = pSysInfo->lan_config.Supportstream1;
	supportMpeg4 = pSysInfo->lan_config.Supportstream2;
	supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
	supportH264 = pSysInfo->lan_config.Supportstream5;
	supportH264cif = pSysInfo->lan_config.Supportstream6;
	//int mpeg41Xsize= pSysInfo->lan_config.Mpeg41Xsize;
	//int mpeg41Ysize= pSysInfo->lan_config.Mpeg41Ysize;
	//int mpeg42Xsize=pSysInfo->lan_config.Mpeg42Xsize;
	//int mpeg42Ysize=pSysInfo->lan_config.Mpeg42Ysize;
	//int avc1Xsize=pSysInfo->lan_config.Avc1Xsize;
	//int avc1Ysize=pSysInfo->lan_config.Avc1Ysize;
	//int avc2Xsize=pSysInfo->lan_config.Avc2Xsize;
	//int avc2Ysize=pSysInfo->lan_config.Avc2Ysize;

	supportAVI = supportH264|supportH264cif|supportMpeg4|supportMpeg4cif;

	if (pSysInfo->sdcard_config.sdinsert==3) {
       ftpFileFormatTypeStr = "opt=\"JPEG\"";
	   attachedFileFormatTypeStr = "opt=\"JPEG\"";
	}
	else{
	   if ((supportAVI&&supportJPG)){
		 ftpFileFormatTypeStr = "opt=\"AVI,JPEG\"";
	     attachedFileFormatTypeStr = "opt=\"AVI,JPEG\"";
	   }
	   else if (supportAVI){
		 ftpFileFormatTypeStr = "opt=\"AVI\"";
	     attachedFileFormatTypeStr = "opt=\"AVI\"";
	   }
	   else if (supportJPG){
		 ftpFileFormatTypeStr = "opt=\"JPEG\"";
	     attachedFileFormatTypeStr = "opt=\"JPEG\"";
	   }
	}

	if ((supportAVI&&supportJPG)){
	    sdFileFormatTypeStr = "opt=\"AVI,JPEG\"";
	}
	else if (supportAVI){
		sdFileFormatTypeStr = "opt=\"AVI\"";
	}
	else if (supportJPG){
		sdFileFormatTypeStr = "opt=\"JPEG\"";
	}

	xadd_stag_attr(req, TAG_AlarmSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_inputType, inputTypeStr);
	xadd_etag(req, TAG_inputType);
	xadd_stag_attr(req, TAG_outputType, outputTypeStr);
	xadd_etag(req, TAG_outputType);
	xadd_stag_attr(req, TAG_ftpFileFormatType, ftpFileFormatTypeStr);
	xadd_etag(req, TAG_ftpFileFormatType);
	xadd_stag_attr(req, TAG_attachedFileFormatType, attachedFileFormatTypeStr);
	xadd_etag(req, TAG_attachedFileFormatType);
	xadd_stag_attr(req, TAG_sdFileFormatType, sdFileFormatTypeStr);
	xadd_etag(req, TAG_sdFileFormatType);
	xadd_stag_attr(req, TAG_alarmAudioFile, alarmAudioFileStr);
	xadd_etag(req, TAG_alarmAudioFile);
	xadd_etag(req, TAG_AlarmSetting);

	return STATUS_OK;
}

int retrieve_custom_storage(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_StorageSettingList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
	    retrieve_custom_storage_id(req, node);
	xadd_etag(req, TAG_StorageSettingList);

	return STATUS_OK;
}

int retrieve_custom_storage_id(request *req, node_t *node)
{
	StorageSetting_psia StorageSetting;
	SysInfo* pSysInfo = GetSysInfo();
	Schedule_t *pSchedule;
	int i;

	sprintf(StorageSetting.id,"0");
	sprintf(StorageSetting.storageLocation,"%d",pSysInfo->lan_config.recordlocal);
	sprintf(StorageSetting.expireEnable,"%d",pSysInfo->lan_config.nScheduleRepeatEnable);
	sprintf(StorageSetting.numWeeks,"%d",pSysInfo->lan_config.nScheduleNumWeeks);
	sprintf(StorageSetting.infiniteEnable,"%d",pSysInfo->lan_config.nScheduleInfiniteEnable);
	sprintf(StorageSetting.delschedule,"0");

	xadd_stag_attr(req, TAG_StorageSetting, RootAttrStr);
	xadd_elem(req, TAG_id, StorageSetting.id);
	xadd_elem(req, TAG_storageLocation, StorageSetting.storageLocation);
	xadd_elem(req, TAG_expireEnable, StorageSetting.expireEnable);
	xadd_elem(req, TAG_numWeeks, StorageSetting.numWeeks);
	xadd_elem(req, TAG_infiniteEnable, StorageSetting.infiniteEnable);

	xadd_stag(req, TAG_TimeBlockList);
	for(i = 0; i < SCHDULE_NUM; i ++){
	   pSchedule = &(pSysInfo->lan_config.aSchedules[i]);
       sprintf(StorageSetting.enabled,"%d",pSchedule -> bStatus);

	    xadd_stag(req, TAG_TimeBlock);
		xadd_elem(req, TAG_enabled, StorageSetting.enabled);
	    xadd_etag(req, TAG_TimeBlock);
	}
	xadd_etag(req, TAG_TimeBlockList);
	xadd_elem(req, TAG_delschedule, StorageSetting.delschedule);
	xadd_etag(req, TAG_StorageSetting);

	return STATUS_OK;
}

int update_custom_storage_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	StorageSetting_psia StorageSetting;
	__u8 value,index = 0;;
	int bEnable;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->filesize) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;

	if(!((tag) && (strcmp(tag->name, TAG_StorageSetting) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;

	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(StorageSetting.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_storageLocation) == 0) {
			strcpy(StorageSetting.storageLocation, tag->value);
			required |= xml_required_field(2);
			value = atoi(StorageSetting.storageLocation);
			ControlSystemData(SFIELD_SET_RECORDLOCAL, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_expireEnable) == 0) {
			strcpy(StorageSetting.expireEnable, tag->value);
			required |= xml_required_field(3);
			value = atoi(StorageSetting.expireEnable);
			ControlSystemData(SFIELD_SET_SCHEDULE_REPEAT_ENABLE, (void *)&value, sizeof(value));
		}
		else if(strcmp(tag->name, TAG_numWeeks) == 0) {
			strcpy(StorageSetting.numWeeks, tag->value);
			required |= xml_required_field(4);
			value = atoi(StorageSetting.numWeeks);
			ControlSystemData(SFIELD_SET_SCHEDULE_NUMWEEKS, &value, sizeof(value));
		}
        else if(strcmp(tag->name, TAG_TimeBlockList) == 0) {
            tag = tag->first_child;
			while(tag) {
				if(strcmp(tag->name, TAG_TimeBlock) == 0) {
                  tag = tag->first_child;
				  while(tag) {
					  if(strcmp(tag->name, TAG_enabled) == 0) {
                          strcpy(StorageSetting.enabled, tag->value);
			              required |= xml_required_field(5);
						  value = atoi(StorageSetting.enabled);

						  scheduleEnable[index] = value;
					  }
					  if(!tag->next)
				        break;
				      tag = tag->next;
				  }
				  tag = tag->parent;
                }
				index ++;

				if(!tag->next)
				   break;
                tag = tag->next;
			}
			tag = tag->parent;
		}
		else if(strcmp(tag->name, TAG_delschedule) == 0) {
			strcpy(StorageSetting.delschedule, tag->value);
			required |= xml_required_field(2);
			bEnable = atoi(StorageSetting.delschedule);
			ControlSystemData(SFIELD_DEL_SCHEDULE, &bEnable, sizeof(bEnable));
		}
		/* Protocol */
		tag = tag->next;
	}
	index = 0;
	/* check all the required fields were present */
	//if(required != xml_required_mask(5))
		//return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_storage_id_capabilities(request *req, node_t *node)
{
	const char *storageLocationStr = "opt=\"SD/MMC,USB,NAND,PC\"";

	xadd_stag_attr(req, TAG_StorageSetting, RootAttrStr);
	xadd_stag_attr(req, TAG_storageLocation, storageLocationStr);
	xadd_etag(req, TAG_storageLocation);
	xadd_etag(req, TAG_StorageSetting);

	return STATUS_OK;
}

int retrieve_custom_reload(request *req, node_t *node)
{
	Reload_psia Reload;
	SysInfo* pSysInfo = GetSysInfo();

	sprintf(Reload.time,"20");
	sprintf(Reload.flag,"%d",pSysInfo->lan_config.reloadFlag);

	xadd_stag_attr(req, TAG_Reload, RootAttrStr);
	xadd_elem(req, TAG_reloadTime, Reload.time);
	xadd_elem(req, TAG_reloadFlag, Reload.flag);
	xadd_etag(req, TAG_Reload);

	return STATUS_OK;
}

/* called just before the request in going to to be processed */
int iface_init(request *req)
{
	return R_REQUEST_OK;
}

/* called after the request has completed */
void iface_cleanup(request *req)
{

}


