#ifndef INPUT_DEVICE_MPU_H
#define INPUT_DEVICE_MPU_H

#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>
#include <control.h>
#include <queue>
#include <map>

class InputDevice
{
private:
    static int16_t ax, ay, az;
    static int16_t gx, gy, gz;
    static MPU6050 mpu;
    static std::deque<int> buf;
    static void bufPush(int key);

public:
    void mpuInit(); // 陀螺仪初始化
    void inputDriveInit();      // LVGL输入设备初始化
    static void updateMotion(); // 更新加速度数据
    static void mpuRead();      // 将陀螺仪加速度变化读取为按键输入
    static const int buf_size;
    static bool keyRead(lv_indev_drv_t *drv, lv_indev_data_t *data);
};
extern InputDevice indev;

#endif
