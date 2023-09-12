/*
  指纹密码输入器
  Author     : Lessonwill
  Create Time: 2023-09-12
*/
#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup(){}
void loop(){}
#else

#include "USB.h"
#include "USBHIDKeyboard.h"
#include <HardwareSerial.h>
#include "yfrobot_fpm383.h"

#define PASSWORD "testpassword"

USBHIDKeyboard Keyboard;

HardwareSerial mySerial(0);
YFROBOTFPM383 fpm(&mySerial);
int LEDPIN = LED_BUILTIN;

void setup() {
  // initialize control over the keyboard:
  Keyboard.begin();
  USB.begin();
  delay(5000); //wait for usb init

  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);

  // 初始化
  while (fpm.getChipSN() == "") {
    Serial.println("等待......");
    delay(200);  //等待指纹识别模块初始化完成
  }
  Serial.println(fpm.getChipSN());

  Serial.println("开始");
  delay(100);
}

void sendPassword()
{
    Keyboard.println(PASSWORD);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*验证指纹工作流程：
        1、无手指时，闪烁红绿色（黄色）灯一次
        2、搜索到未认证手指指纹，闪烁红色灯两次
        3、搜索未认证手指指纹，解析并校验数据，正常则返回指纹ID，并闪烁绿灯两次
  */
  int id = fpm.identify(false);  //验证指纹并获取指纹ID，参数true 在无手指状态是否有LED灯提示，您可以false关掉指示灯
  if (id == 0) {
    sendPassword();
    Serial.println("识别到指纹0");
    digitalWrite(LEDPIN, HIGH);
    delay(100);
    digitalWrite(LEDPIN, LOW);
  } 

  delay(500);  // 推荐>500ms
}

#endif