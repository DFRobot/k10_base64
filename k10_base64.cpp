/*!
 * @file k10_base64.cpp
 * @brief 这是一个转换数据为base64的库
 * @copyright   Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version  V1.0
 * @date  2025-04-01
 * @url https://github.com/DFRobot/k10_base64
 */

#include "k10_base64.h"
#include "base64.h"

static camera_fb_t *base64_frame = NULL;
extern QueueHandle_t xQueueCamer;
K10_base64::K10_base64(void){}


String K10_base64::K10tobase64(void)
{
    size_t _jpg_buf_len = 0;
    uint8_t *_jpg_buf = NULL;
    // 从队列接收帧数据
    if (xQueueReceive(xQueueCamer, &base64_frame, portMAX_DELAY) != pdTRUE) {
        DBG("continue");
        return "NULL";  // 如果接收失败，继续等待
    }
    // 转换帧为 JPEG
    if (!frame2jpg(base64_frame, 80, &_jpg_buf, &_jpg_buf_len)) {
        DBG("JPEG compression failed");
        esp_camera_fb_return(base64_frame);
        return "NULL";
    }
    esp_camera_fb_return(base64_frame);
    String base64Image = base64::encode(_jpg_buf, _jpg_buf_len);
    // 释放帧数据
    free(_jpg_buf);
    return base64Image;
}

String K10_base64::imgtobase64(String img)
{
    lv_fs_res_t ret = 0;
    lv_fs_file_t phFile;
    ret = lv_fs_open(&phFile, img.c_str(), 0x02);
    //计算文件大小
    uint32_t fileSize = 0;
    uint32_t tempRead;
    char buffer[64]; // 用于遍历文件
    while (lv_fs_read(&phFile, buffer, sizeof(buffer), &tempRead) == LV_FS_RES_OK && tempRead > 0) {
        fileSize += tempRead;
    }

    lv_fs_seek(&phFile, 0, LV_FS_SEEK_SET);

    uint8_t *dataBuffer = (uint8_t *)malloc(fileSize);
    if (!dataBuffer) {
        Serial.println("malloc error");
        lv_fs_close(&phFile);
        return "";
    }

    uint32_t bytesRead;
    lv_fs_read(&phFile, dataBuffer, fileSize, &bytesRead);
    lv_fs_close(&phFile);

    if (bytesRead != fileSize) {
        Serial.println("read error!!!");
        free(dataBuffer);
        return "";
    }

    String base64Data = base64::encode(dataBuffer, fileSize);
    free(dataBuffer);
    return base64Data;
}
