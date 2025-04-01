/*!
 * @file base64_cam.ino
 * @brief 这是一个将摄像头的画面数据转换为base64的示例代码
 * @copyright   Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2025-03-21 
 * @url https://github.com/DFRobot/unihiker_k10_webcam
 */
 #include "k10_base64.h"
 #include "unihiker_k10.h"
 
 UNIHIKER_K10 k10;
 K10_base64 base64_cam;
 
 void setup()
 {
     Serial.begin(115200);
     
     //初始化K10
     k10.begin();
 
     //初始化显示
     k10.initScreen();

     //初始化摄像头显示在屏幕上
     k10.initBgCamerImage();
 
 }
 
 void loop()
 {
    String base64Image = base64_cam.K10tobase64();
    if (base64Image.length() > 0) {
        Serial.println("Base64 编码成功！");
        Serial.println(base64Image);
    }else{
        Serial.println("Base64 编码失败！");
    }
 }