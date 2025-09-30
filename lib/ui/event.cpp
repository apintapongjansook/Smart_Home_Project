#include "event.h"
#include <Arduino.h>    
#include <ESP32Time.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include "touch.hpp"
#include <ESP32Servo.h>
#include "DHT.h"
#include <TFT_eSPI.h>

ESP32Time rtc(3600*7);
Servo myServo;
lv_timer_t * sound_timer = NULL;
TFT_eSPI tft = TFT_eSPI();

//--------------setup sound sensor---------//
const int micPin = 26;   //sound sensor
const int DHTPIN = 17; //อุณหภูมิกับตัวความชื้น
#define DHTTYPE  DHT11
const int BUZZER = 25; //ขา Buzzer
int lightst;

DHT dht(DHTPIN, DHTTYPE);
static bool buzzerOn = false;   // เก็บสถานะ Buzzer
static float TEMP_THRESHOLD ; // กำหนดอุณหภูมิที่ให้ buzzer ติด

const unsigned long SAMPLE_WINDOW = 50;  // ms
const unsigned long CLAP_TIMEOUT = 800;  // ms รอ clap ที่ 2
const unsigned long REFRACTORY = 250;    // ms กัน detect ซ้ำ

// Thresholds (ปรับตามสภาพจริง)
const int THRESHOLD_HIGH = 2100;
const int THRESHOLD_LOW  = 1800;

bool ledState = false; 

//----------------setup ledc----------------//
#define LED_PIN 16        // ขาที่ต่อ LED
#define LEDC_CHANNEL 0   // ช่องสัญญาณ (0-15)
#define LEDC_TIMER 8    // ความละเอียดบิต (1-20)
#define LEDC_FREQ 5000   // ความถี่ PWM (Hz)

//------------setup touch sleep-------------//
#define TIRQ_PIN 33     // IRQ Pin ถ้ามี (ถ้าไม่ใช้ใส่ -1) (ขาจอ)
int TIMEOUT = 30000 ;  // เวลา timeout 15 วินาที

unsigned long lastTouchMillis = 0;

#define LDR_DO 13 //light sensor pin

#define PIR_PIN 21 //motion sensor pin



void event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    //Login Page
    // TextArea ถูก Focused
    if(obj == objects.username_txarea && code == LV_EVENT_FOCUSED) {
        lv_obj_clear_flag(objects.keyboard_login, LV_OBJ_FLAG_HIDDEN);  
        lv_keyboard_set_textarea(objects.keyboard_login, objects.username_txarea);          
    }
    else if(obj == objects.password_txarea && code == LV_EVENT_FOCUSED) {
        lv_obj_clear_flag(objects.keyboard_login, LV_OBJ_FLAG_HIDDEN);  
        lv_keyboard_set_textarea(objects.keyboard_login, objects.password_txarea);          
    }
    else if(obj == objects.keyboard_login && (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)) {
        lv_obj_add_flag(objects.keyboard_login, LV_OBJ_FLAG_HIDDEN);    
    }
    else if(obj == objects.login_bt && code == LV_EVENT_CLICKED){
        const char *username = lv_textarea_get_text(objects.username_txarea);
        const char *password = lv_textarea_get_text(objects.password_txarea);
        if(strcmp(username ,"admin") == 0 && strcmp(password , "1234") == 0) {
            lv_scr_load(objects.setting_page);
        }
    }
    //หน้า monitor
    else if(obj == objects.setting_bt && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.login_page);
    }
    else if(obj == objects.logout_bt && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.login_page);
    }

    //หน้า Setting
    else if(obj == objects.monitor_bt_1 && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.home_page);
    }
    else if(obj == objects.logout_bt_1 && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.login_page);
    }
    else if(obj == objects.led_sw && code == LV_EVENT_CLICKED){
        if(lv_obj_has_state(objects.led_sw, LV_STATE_CHECKED)){
            lv_timer_del(sound_timer);
            ledState = true;
        }
        else{
            sound_timer = lv_timer_create(sound_timer_cb, 10, NULL);
            ledState = false;
        }
    }

}


void setuprtc(){
    rtc.setTime(1758621217);
    ledcSetup(LEDC_CHANNEL, LEDC_FREQ, LEDC_TIMER);
    ledcAttachPin(LED_PIN, LEDC_CHANNEL);
    lastTouchMillis = millis();
    pinMode(LDR_DO, INPUT);
    myServo.attach(14);

    dht.begin();
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);

    pinMode(PIR_PIN, INPUT);
    sound_timer = lv_timer_create(sound_timer_cb, 10, NULL);

}


void dht_timer(lv_timer_t *timer) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    lv_label_set_text(objects.temp, String(t).c_str());
    lv_label_set_text(objects.humid, String(h).c_str());

    char buf[32];
    lv_roller_get_selected_str(objects.temp_alarm, buf, sizeof(buf));
    TEMP_THRESHOLD = atof(buf);

    static unsigned long buzzerUntil = 0;

    if (t >= TEMP_THRESHOLD) {
        if (!buzzerOn) {
            digitalWrite(BUZZER, HIGH);
            buzzerOn = true;
            buzzerUntil = millis() + 1000;   // เปิด buzzer 1 วิ
        }
    }

    if (buzzerOn && millis() > buzzerUntil) {
        digitalWrite(BUZZER, LOW);
        buzzerOn = false;
    }
}


void updatetime_daytime(lv_timer_t *timer){
    lv_label_set_text(objects.time_display, rtc.getTime("%R").c_str());
    lv_label_set_text(objects.second_time_display_1, rtc.getTime("%S").c_str());
    lv_label_set_text(objects.day_display, rtc.getTime("%A , %B %d").c_str());
}




void sound_timer_cb(lv_timer_t *timer) {
    // static variables สำหรับเก็บ state
    static int signalMax = 0, signalMin = 4095;
    static unsigned long windowStart = millis();
    static int clapCount = 0;
    static unsigned long firstClapTime = 0;
    static unsigned long lastClapTime = 0;
    static bool triggered = false;

    // เก็บ sample
    if (millis() - windowStart < SAMPLE_WINDOW) {
        int sample = analogRead(micPin);
        if (sample > signalMax) signalMax = sample;
        if (sample < signalMin) signalMin = sample;
    } else {
        // ประมวลผล peak-to-peak
        int peakToPeak = signalMax - signalMin;
        unsigned long now = millis();

        // Hysteresis detection
        if (!triggered && peakToPeak > THRESHOLD_HIGH && (now - lastClapTime) > REFRACTORY) {
            triggered = true;
            lastClapTime = now;

            if (clapCount == 0) {
                clapCount = 1;
                firstClapTime = now;
            } else if (clapCount == 1 && (now - firstClapTime) < CLAP_TIMEOUT) {
                ledState = !ledState;
                clapCount = 0; // reset
            }
        }

        // ออกจาก trigger เมื่อเสียงตกต่ำกว่า threshold low
        if (triggered && peakToPeak < THRESHOLD_LOW) {
            triggered = false;
        }

        // timeout ถ้าเจอ clap แรก แต่ไม่มี clap ที่สอง
        if (clapCount == 1 && (now - firstClapTime) > CLAP_TIMEOUT) {
            clapCount = 0;
        }

        // reset window
        signalMax = 0;
        signalMin = 4095;
        windowStart = now;
    }
}





void ledc(lv_timer_t *timer){
    int slvalue = lv_slider_get_value(objects.slider_pwm_led);
    if(ledState == true){
        ledcWrite(LEDC_CHANNEL,slvalue);
    }
    else{
        ledcWrite(LEDC_CHANNEL,0);
    }
}


void touchsleep(lv_timer_t *timer){
    
    char buf[32];
    lv_roller_get_selected_str(objects.sleep_time_select, buf, sizeof(buf));
    TIMEOUT = atoi(buf)*60*1000;

  if (ts.touched()) {
    lastTouchMillis = millis();   // อัปเดตเวลาเมื่อมีการแตะ
  }

  // ถ้าไม่ได้แตะเกินเวลาที่กำหนด → Sleep
  if (millis() - lastTouchMillis > TIMEOUT) {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);

    digitalWrite(32, LOW);
    tft.writecommand(0x28);

    esp_light_sleep_start();

    digitalWrite(32, HIGH);
    tft.writecommand(0x29);  
    lastTouchMillis = millis();

  }
}
void lightsensor(lv_timer_t *timer){
    lightst = digitalRead(LDR_DO);
    if(lightst == 0){
        lv_label_set_text(objects.light_status,"Light ON");
    }
    else if(lightst == 1){
        lv_label_set_text(objects.light_status,"Light OFF");
    }
}

void motion(lv_timer_t *timer){
    int motion = digitalRead(PIR_PIN);

    if (motion == HIGH){
        lv_label_set_text(objects.pir_stat,"Detect");
        if(lightst==1){
            ledState = true;
        }else{
            ledState = false;
        }
    } 
    else{
        lv_label_set_text(objects.pir_stat,"Undetect");
    }
}


void opendoor(lv_timer_t *timer){
    char command = 0;
    if(Serial.available() > 0 ){
        command = Serial.read();
    }

    if(command == 'a'){
        opendoor2_start();
        command = 0;
    }
    opendoor2_update(lv_timer_t *timer);
}

// ===== Non-blocking Door Control =====
enum DoorState {
  DOOR_IDLE,
  DOOR_OPENING,
  DOOR_OPENED,
  DOOR_CLOSING
};

DoorState doorState = DOOR_IDLE;
int servoPos = 180;               // เริ่มที่ 180°
int targetPos = 180;
unsigned long previousMillisDoor = 0;
unsigned long stayOpenUntil = 0;
const int stepDelayDoor = 5;      // ความเร็วเซอร์โว (ms ต่อ 1 step)

void opendoor2_start() 
{
  servoPos = 180;         // เริ่มต้นที่ 180
  targetPos = 130;        // เป้าหมายแรกคือ 130
  doorState = DOOR_OPENING;
  previousMillisDoor = millis();
  myServo.write(servoPos);
}

void opendoor2_update(lv_timer_t *timer) 
{
  unsigned long currentMillis = millis();

  switch (doorState) {
    case DOOR_OPENING:
      if (currentMillis - previousMillisDoor >= stepDelayDoor) {
        previousMillisDoor = currentMillis;
        if (servoPos > targetPos) {
          servoPos--;
          myServo.write(servoPos);
        }
        if (servoPos == targetPos) {
          doorState = DOOR_OPENED;
          stayOpenUntil = currentMillis + 10000; // ค้าง 10 วิ
        }
      }
      break;

    case DOOR_OPENED:
      if (currentMillis >= stayOpenUntil) {
        targetPos = 180;
        doorState = DOOR_CLOSING;
      }
      break;

    case DOOR_CLOSING:
      if (currentMillis - previousMillisDoor >= stepDelayDoor) {
        previousMillisDoor = currentMillis;
        if (servoPos < targetPos) {
          servoPos++;
          myServo.write(servoPos);
        }
        if (servoPos == targetPos) {
          doorState = DOOR_IDLE;  // เสร็จสิ้น
        }
      }
      break;

    case DOOR_IDLE:
    default:
      break;
  }
}
