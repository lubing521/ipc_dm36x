/*
 * Main header file
 *
 */

#ifndef __PSIA_PSIA_H__
#define __PSIA_PSIA_H__

#include "file_list.h"

#define STATUS_OK			        1
#define STATUS_DEVICE_BUSY		    2
#define STATUS_DEVICE_ERROR		    3
#define STATUS_INVALID_OPERATION	4
#define STATUS_INVALID_XML_FORMAT	5
#define STATUS_INVALID_XML_CONTENT	6
#define STATUS_REBOOT_REQUIRED		7
#define STATUS_NOT_IMPLEMENTED		8

extern const char *PrologString;
extern const char *RootAttrStr;
extern const char *ResourceListAttrStr;
extern const char *IndexAttrStr;
extern const char *ResourceDescriptionAttrStr;

extern const char *SystemAttrStr;
extern const char *SystemRebootAttrStr;
extern const char *SystemUpdateAttrStr;
extern const char *SystemConfigAttrStr;
extern const char *SystemResetAttrStr;
extern const char *SystemDeviceInfoAttrStr;
extern const char *SystemReportAttrStr;
extern const char *SystemStatusAttrStr;
extern const char *SystemTimeAttrStr;
extern const char *SystemLoggingAttrStr;

extern const char *VideoAttrStr;
extern const char *VideoInputAttrStr;

extern const char *NetworkAttrStr;
extern const char *NetworkInterfaceAttrStr;

extern const char *SecurityAttrStr;
extern const char *SecurityUsersAttrStr;

extern const char *StreamingAttrStr;
extern const char *StreamingStatusAttrStr;
extern const char *StreamingChannelAttrStr;

extern const char *MotionAttrStr;
extern const char *MotionIDAttrStr;

extern const char *StatusString[];

/* global debug enable */
#ifdef PSIA_DEBUG
#include <syslog.h>
#define debug_printf(FMT...) syslog (LOG_INFO, FMT)
#else
#define debug_printf(FMT...)
#endif

#include <boa.h>

/* xparse.c */
int xml_validate(request *req, char *xstr, int len);
unsigned long xml_required_field(int bit);
unsigned long xml_required_mask(int count);
int xml_init(request *req);
void xml_cleanup(request *req);

/* psia_root.c */
void add_std_resources(node_t *parent, void *fn_index, void *fn_indexr, void *fn_description, void *fn_capabilities);
int process_psia_request(request *req);
int clean_psia_request(request *req, node_t *root);

/* tree.c */
node_t *add_node(node_t *parent, const char *name, int methods, int type, const char *version, void *fn);
void delete_tree(node_t *node);
int build_tree_error(void);
int process_branch(request *req, node_t *parent);

/* index.c */
// int process_index(request *req, node_t *me);
// int process_indexr(request *req, node_t *me);

/* description.c */
void desc_get_query(node_t *node, const char *str);
void desc_get_inbound(node_t *node, const char *str);
void desc_get_function(node_t *node, const char *str);
void desc_get_result(node_t *node, const char *str);
void desc_get_notes(node_t *node, const char *str);
void desc_put_query(node_t *node, const char *str);
void desc_put_inbound(node_t *node, const char *str);
void desc_put_function(node_t *node, const char *str);
void desc_put_result(node_t *node, const char *str);
void desc_put_notes(node_t *node, const char *str);
void desc_post_query(node_t *node, const char *str);
void desc_post_inbound(node_t *node, const char *str);
void desc_post_function(node_t *node, const char *str);
void desc_post_result(node_t *node, const char *str);
void desc_post_notes(node_t *node, const char *str);
void desc_delete_query(node_t *node, const char *str);
void desc_delete_inbound(node_t *node, const char *str);
void desc_delete_function(node_t *node, const char *str);
void desc_delete_result(node_t *node, const char *str);
void desc_delete_notes(node_t *node, const char *str);
// int process_description(request *req, node_t *me);

/* services */
//int process_system(request *req, node_t *parent);
//int process_streaming(request *req, node_t *parent);
//int process_security(request *req, node_t *parent);

/* xadd.c */
int xadd_init(request *req);
void xadd_text(request *req, const char *fmt, ...);
void xadd_stag(request *req, const char *tag);
void xadd_stag_attr(request *req, const char *tag, const char *fmt, ...);
void xadd_etag(request *req, const char *tag);
void xadd_elem(request *req, const char *tag, const char *val);
void xadd_flush(request *req);
void xadd_cleanup(request *req);

void send_r_psia_get(request * req);
int retrieve_system_video_inputs(request *req, node_t *node);
int retrieve_system_video_channels_id_overlays_image_id_capabilities(request *req, node_t *node);
void reboot_ipcam(void);
void send_r_psia_supportReport(request * req);
int retrieve_system_time_capabilities(request *req, node_t *node);
int update_streaming_channels(request *req, node_t *node);
int create_streaming_channels(request *req, node_t *node);
int delete_streaming_channels(request *req, node_t *node);
int update_streaming_channels_id(request *req, node_t *node, tag_t *parentTag);
int delete_streaming_channels_id(request *req, node_t *node);
void send_r_psia_ok_mjpeg(request * req);
void send_r_psia_ok_mpeg4(request * req);
void send_r_psia_ok_avc(request * req);
int http_sysget_cmd(char *addr, int max_size);
int http_accessget_cmd(char *addr, int max_size);
int http_actvconlistget_cmd(char *addr, int max_size);
int http_paraget_cmd(char *addr, int max_size);

SysInfo* GetSysInfo();
int GetSysLog(int nPageNum, int nItemNum, LogEntry_t * pBuf);
void cleanSystemLog();
void cleanAccessLog();

FILE_INFO *Get_File_List( char *pDir, int *pCnt );
int ipv4_str_to_num(char *data, struct in_addr *ipaddr);
int sys_set_time(int hour, int min, int sec);
int sys_set_date(int year, int month, int day);

void send_r_psia_datetime(request * req,char *local_time);
void send_r_psia_timezone(request * req,char *timezone);
int retrieve_system_logging_messages_id(request *req, node_t *node);

void GetStreamType(char *sType);
void GetCodecCombo(char *cCombo);
void GetResolution(char *resl);

int uri_mjpeg(request * req);
int uri_mpeg4cif(request * req);
int uri_mpeg4(request * req);
int uri_avccif(request * req);
int uri_avc(request * req);

#endif /* __PSIA_PSIA_H__ */
