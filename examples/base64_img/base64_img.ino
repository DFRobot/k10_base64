/*!
 * @file base64_img.ino
 * @brief 这是一个将内存卡中的画面数据转换为base64的示例代码
 * @copyright   Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2025-04-01
 * @url https://github.com/DFRobot/k10_base64
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

    //初始化SD卡
     k10.initSDFile();
    

}

void loop()
{
   String base64Image = base64_cam.imgtobase64("S:/22.bmp");
   if (base64Image.length() > 0) {
       Serial.println("Base64 编码成功！");
       Serial.println(base64Image);
   }else{
       Serial.println("Base64 编码失败！");
   }
}