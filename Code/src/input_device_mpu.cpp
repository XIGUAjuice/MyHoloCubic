#include <input_device_mpu.h>
#include <global.h>

int16_t InputDevice::ax = 0;
int16_t InputDevice::ay = 0;
int16_t InputDevice::az = 0;
int16_t InputDevice::gx = 0;
int16_t InputDevice::gy = 0;
int16_t InputDevice::gz = 0;
MPU6050 InputDevice::mpu = MPU6050(0x68);
const int InputDevice::buf_size = 5;
std::deque<int> InputDevice::buf;

lv_indev_drv_t indev_drv;
lv_indev_t *my_indev;
lv_group_t *group_tile;

/* 初始化陀螺仪 */
void InputDevice::mpuInit()
{
    Wire.begin(32, 33);
    mpu.initialize();
    Serial.println("陀螺仪初始化中……");
    while (!mpu.testConnection())
        ;
    Serial.println("陀螺仪初始化成功！");

    // 陀螺仪校正
    mpu.setXAccelOffset(-3547);
    mpu.setYAccelOffset(1347);
    mpu.setZAccelOffset(2183);
    mpu.setXGyroOffset(-14);
    mpu.setYGyroOffset(-32);
    mpu.setZGyroOffset(2);

    // 自动校正
    Serial.println("陀螺仪自动校正中，请勿移动......");
    mpu.CalibrateAccel(15);
    mpu.CalibrateGyro(15);
    mpu.PrintActiveOffsets();
}

/* 更新加速度信息 */
void InputDevice::updateMotion()
{
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}

void InputDevice::bufPush(int key)
{
    // 保证缓冲区不会溢出，正常情况是不会溢出的
    if (buf.size() == buf_size)
    {
        Serial.println("按键缓冲区溢出");
    }
    else
    {
        buf.push_back(key);
        // Serial.printf("push %d \t %d/%d", key, buf.size(), buf_size);
    }
}

bool InputDevice::keyRead(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    mpuRead();
    if (buf.size() == buf_size)
    {
        // 按键buffer的权重
        int key_weight[buf_size] = {10, 8, 7, 6, 5};

        // 记录各按键的得分
        std::map<int, int> key_score = {{LV_KEY_NEXT, 0}, {LV_KEY_PREV, 0}, {LV_KEY_ENTER, 0}, {LV_KEY_UP, 0}, {LV_KEY_DOWN, 0}, {LV_KEY_RIGHT, 0}, {LV_KEY_LEFT, 0}};
        for (int i = 0; i < buf_size; i++)
        {
            key_score[buf[i]] = key_score[buf[i]] + key_weight[i];
        }

        // 找出分数最大的按键
        int key, max_score = 0;
        for (auto pair : key_score)
        {
            Serial.printf("key: %d \t score: %d\n", pair.first, pair.second);
            if (pair.second > max_score)
            {
                key = pair.first;
                max_score = pair.second;
            }
        }

        // 按键得分高于一定值才判定为该按键
        if (max_score >= 18)
        {
            data->key = key;
            data->state = LV_INDEV_STATE_PR;
        }
        else
        {
            key = 0;
            data->state = LV_INDEV_STATE_REL;
        }

        switch (key)
        {
        case LV_KEY_NEXT:
            Serial.println("next");
            break;
        case LV_KEY_PREV:
            Serial.println("prev");
            break;
        case LV_KEY_ENTER:
            Serial.println("enter");
            break;
        case LV_KEY_UP:
            Serial.println("up");
            break;
        case LV_KEY_DOWN:
            Serial.println("down");
            break;
        case LV_KEY_RIGHT:
            Serial.println("right");
            break;
        case LV_KEY_LEFT:
            Serial.println("left");
            break;
        default:
            break;
        }
        buf.clear();
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
    return false;
}

/* 输入设备回调，将陀螺仪加速度变化读取为按键输入 */
void InputDevice::mpuRead()
{

    static long prev_read_time = 0; // 上一次读取按键的时间
    static int read_interval = 120; //读取按键的间隔(ms)

    if (millis() - prev_read_time > read_interval)
    {
        prev_read_time = millis();
        updateMotion();
        bool have_action = false;
        // Serial.printf("gx: %5d \t gy: %5d \t gz: %5d \t az: %5d \n", gx, gy, gz, az);

        if (gz < -2900)
        {
            bufPush(LV_KEY_NEXT);
            have_action = true;
        }
        if (gz > 2900)
        {
            bufPush(LV_KEY_PREV);
            have_action = true;
        }
        if (az > 17000)
        {
            bufPush(LV_KEY_ENTER);
            have_action = true;
        }
        if (gy < -2900)
        {
            bufPush(LV_KEY_DOWN);
            have_action = true;
        }
        if (gy > 2900)
        {
            bufPush(LV_KEY_UP);
            have_action = true;
        }
        if (gx < -2500)
        {
            bufPush(LV_KEY_LEFT);
            have_action = true;
        }
        if (gx > 2500)
        {
            bufPush(LV_KEY_RIGHT);
            have_action = true;
        }
        if (!have_action)
        {
            buf.clear();
        }
    }
}

/* LVGL输入设备初始化 */
void InputDevice::inputDriveInit()
{
    // // 初始化陀螺仪
    // mpuInit();

    // 创建输入设备
    Serial.println("创建输入设备...");
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;        // 输入设备类型为键盘
    indev_drv.read_cb = keyRead;                  // 设置输入回调函数
    my_indev = lv_indev_drv_register(&indev_drv); // 注册输入设备

    group_tile = lv_group_create();
    /* *******************将被控部件加入组***************** */
    lv_group_add_obj(group_tile, tile_main);
    /* **************************************************** */

    // 将组关联到输入设备
    lv_indev_set_group(my_indev, group_tile);
    // lv_indev_set_group(my_indev, group_wifi);
}