#ifndef __NEW_MQTT_H__
#define __NEW_MQTT_H__

#include "../new_common.h"

#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#if PLATFORM_XR809
#include "my_lwip2_mqtt_replacement.h"
#else
#include "lwip/apps/mqtt.h"
#endif

extern ip_addr_t mqtt_ip;
extern mqtt_client_t* mqtt_client;

//void mqtt_example_init(void);
//void example_do_connect(mqtt_client_t *client);
void example_publish(mqtt_client_t *client, int channel, int iVal);
void MQTT_init();
int MQTT_RunEverySecondUpdate();

enum OBK_Publish_Result_e {
	OBK_PUBLISH_OK,
	OBK_PUBLISH_MUTEX_FAIL,
	OBK_PUBLISH_WAS_DISCONNECTED,
	OBK_PUBLISH_WAS_NOT_REQUIRED,
};

#define OBK_PUBLISH_FLAG_MUTEX_SILENT	1
#define OBK_PUBLISH_FLAG_RETAIN			2

// ability to register callbacks for MQTT data
typedef struct mqtt_request_tag {
    const unsigned char *received; // note: NOT terminated, may be binary
    int receivedLen;
    char topic[128];
} mqtt_request_t;

// callback function for mqtt.
// return 0 to allow the incoming topic/data to be processed by others/channel set.
// return 1 to 'eat the packet and terminate further processing.
typedef int (*mqtt_callback_fn)(mqtt_request_t *request);

// topics must be unique (i.e. you can't have /about and /aboutme or /about/me)
// ALL topics currently must start with main device topic root.
// ID is unique and non-zero - so that callbacks can be replaced....
int MQTT_RegisterCallback( const char *basetopic, const char *subscriptiontopic, int ID, mqtt_callback_fn callback);
int MQTT_RemoveCallback(int ID);

void MQTT_GetStats(int *outUsed, int *outMax, int *outFreeMem);

OBK_Publish_Result MQTT_PublishMain_StringFloat(const char *sChannel, float f);
OBK_Publish_Result MQTT_PublishMain_StringInt(const char *sChannel, int val);
OBK_Publish_Result MQTT_PublishMain_StringString(const char *sChannel, const char *valueStr, int flags);
OBK_Publish_Result MQTT_ChannelChangeCallback(int channel, int iVal);
void MQTT_PublishOnlyDeviceChannelsIfPossible();

#endif // __NEW_MQTT_H__


