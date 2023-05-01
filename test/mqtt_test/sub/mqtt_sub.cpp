#include "mqtt_sub.h"
#include <mosquitto.h>
#include <stdint.h>
#include <stdio.h>

static void on_connect(struct mosquitto *mosq, void *obj, int reason_code) {
    printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
    if (reason_code != 0) {
        mosquitto_disconnect(mosq);
    }

    int32_t rc = mosquitto_subscribe(mosq, NULL, "mqtt_test", 1);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error subscribing: %s\n", mosquitto_strerror(rc));
        mosquitto_disconnect(mosq);
    }
}

static void on_subscribe(struct mosquitto *mosq, void *obj, int32_t mid, int32_t qos_count, const int32_t *granted_qos) {
    int i;
    bool have_subscription = false;

    for (i = 0; i < qos_count; i++) {
        printf("on_subscribe: %d:granted qos = %d\n", i, granted_qos[i]);
        if (granted_qos[i] <= 2) {
            have_subscription = true;
        }
    }
    if (have_subscription == false) {
        fprintf(stderr, "Error: All subscriptions rejected.\n");
        mosquitto_disconnect(mosq);
    }
}

static void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    printf("%s %d %s\n", msg->topic, msg->qos, (char *)msg->payload);
}

namespace MqttCloud {

int32_t MqttSub::InitMqttSub() {
    int32_t ret = mosquitto_lib_init();
    if (MOSQ_ERR_SUCCESS != ret) {
        fprintf(stderr, "mosquitto_lib_init failed.\n");
        return -1;
    }

    m_mosq = mosquitto_new(NULL, true, NULL);
    if (m_mosq == NULL) {
        fprintf(stderr, "Error: Out of memory.\n");
        return -1;
    }

    return 0;
}

void MqttSub::SetMqttSubMsgCallback() {
    mosquitto_connect_callback_set(m_mosq, on_connect);
    mosquitto_subscribe_callback_set(m_mosq, on_subscribe);
    mosquitto_message_callback_set(m_mosq, on_message);
}

int32_t MqttSub::RunMqttSub(const char *host, int32_t port) {
    int32_t ret = mosquitto_connect(m_mosq, host, port, 60);
    if (ret != MOSQ_ERR_SUCCESS) {
        mosquitto_destroy(m_mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(ret));
        return -1;
    }

    mosquitto_loop_forever(m_mosq, -1, 1);
    return 0;
}

void MqttSub::ReleaseMqttSub() { mosquitto_lib_cleanup(); }

}  // namespace MqttCloud

int main(int argc, char **argv)
{
    MqttCloud::MqttSub mqttsub;
    int32_t ret = mqttsub.InitMqttSub();
    if (ret != 0) {
        return -1;
    }
    mqttsub.SetMqttSubMsgCallback();
    ret = mqttsub.RunMqttSub("192.168.56.101", 1883);
    if (ret != 0) {
        return -1;
    }
    mqttsub.ReleaseMqttSub();
    return 0;
}