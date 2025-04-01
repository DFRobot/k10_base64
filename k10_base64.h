#ifndef _K10_BASE64_H_
#define _K10_BASE64_H_
#include "Arduino.h"
#include "unihiker_k10.h"

//#define ENABLE_DBG ///< Enable this macro to view the detailed execution process of the program.
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

class K10_base64
{
public:

    K10_base64(void);

    /**
      @fn K10tobase64
      @brief 截取K10当前视频帧并转化为base64编码
      @return base64 编码的字符串
      */
    String K10tobase64(void);

    /**
     @fn imgtobase64
     @brief 将图像（PNG/JPG）转化为base64编码
     @return base64 编码的字符串
     */
    String imgtobase64(String img);

};

#endif
