/*!
 * @file base64_iot.ino
 * @brief 这是一个将摄像头数据通过mqtt上报
 * @copyright   Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2025-04-01
 * @url https://github.com/DFRobot/k10_base64
 */
#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include "k10_base64.h"
#include "unihiker_k10.h"

const char* ssid = "wifi name";//WiFi 名称
const char* password = "wifi password";// WiFi 密码
const char* mqtt_server = "server ip";// IOT 服务器IP
const int mqtt_port = 1883;
const char* mqtt_user = "siot";
const char* mqtt_password = "dfrobot";
const char* mqtt_topic = "topic";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

UNIHIKER_K10 k10;
K10_base64 base64_cam;

unsigned long lastPublishTime = 0; // 上次发布时间
const unsigned long publishInterval = 2000; // 发送间隔

void connectWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) { // 10秒超时
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected.");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi connection failed!");
    }
}

void reconnectMQTT() {
    if (mqttClient.connected()) return; // 如果已连接则跳过

    Serial.print("Connecting to MQTT...");
    mqttClient.setId("ESP32Client");  
    mqttClient.setUsernamePassword(mqtt_user, mqtt_password);

    if (mqttClient.connect(mqtt_server, mqtt_port)) {
        Serial.println("MQTT connected.");

        // 订阅 QoS 1 主题
        if (mqttClient.subscribe(mqtt_topic, 1)) {
            Serial.print("Subscribed to topic: ");
            Serial.println(mqtt_topic);
        } else {
            Serial.println("Failed to subscribe.");
        }
    } else {
        Serial.println("MQTT connection failed!");
    }
}

void setup() {
    Serial.begin(115200);
    k10.begin();
    k10.initScreen();
    k10.initBgCamerImage();
    connectWiFi();
    reconnectMQTT();
    
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi lost, reconnecting...");
        connectWiFi();
    }

    reconnectMQTT();
    mqttClient.poll(); // 处理 MQTT 消息

    // **定时发布消息**
    if (millis() - lastPublishTime >= publishInterval) {
        lastPublishTime = millis();
         String base64Image = "data:image/jpg;base64,";
         base64Image += base64_cam.K10tobase64();
        Serial.println("Publishing message...");
        mqttClient.setTxPayloadSize(base64Image.length());
        mqttClient.beginMessage(mqtt_topic, true, 1); // QoS 1, retain
        mqttClient.print(base64Image);
        mqttClient.endMessage();
        Serial.println("Message sent!");
    }
}
