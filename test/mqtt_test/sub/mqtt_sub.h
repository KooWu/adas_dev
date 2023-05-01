#ifndef __MQTT_SUB_H__
#define __MQTT_SUB_H__

#include <mosquitto.h>

#include <functional>

namespace MqttCloud {
//! Handler on cloud message
// using connectCallback = std::function<void(struct mosquitto *mosq, void *obj, int reason_code)>;
// using subCallback = std::function<void(struct mosquitto *mosq, void *obj, int32_t mid, int32_t qos_count, const int32_t *granted_qos)>;
// using msgCallback = std::function<void(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)>;

class MqttSub {
public:
    int32_t InitMqttSub();
    void SetMqttSubMsgCallback();
    int32_t RunMqttSub(const char *host, int32_t port);
    void ReleaseMqttSub();

private:
    struct mosquitto *m_mosq;
};
}  // namespace MqttCloud

#endif
