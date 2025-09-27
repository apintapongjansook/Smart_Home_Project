#include "event.h"
#include <Arduino.h>    
#include <ESP32Time.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include "touch.hpp"

ESP32Time rtc(3600*7);

//--------------setup sound sensor---------//
const int micPin = 26;   // ขา ADC ของ ESP32

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
#define TIRQ_PIN 33     // IRQ Pin ถ้ามี (ถ้าไม่ใช้ใส่ -1)
int TIMEOUT = 30000 ;  // เวลา timeout 15 วินาที

unsigned long lastTouchMillis = 0;



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
            lv_scr_load(objects.home_page);
        }
    }
    //หน้า monitor
    else if(obj == objects.graph_bt && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.graph_page);
    }
    else if(obj == objects.setting_bt && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.setting_page);
    }
    else if(obj == objects.logout_bt && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.login_page);
    }

    //หน้า Setting
    else if(obj == objects.monitor_bt_1 && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.home_page);
    }
    else if(obj == objects.graph_bt_1 && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.graph_page);
    }
    else if(obj == objects.logout_bt_1 && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.login_page);
    }

    //หน้า Graph
    else if(obj == objects.monitor_bt_2 && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.home_page);
    }
    else if(obj == objects.setting_bt_2 && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.setting_page);
    }
    else if(obj == objects.logout_bt_2 && code == LV_EVENT_CLICKED){
        lv_scr_load(objects.login_page);
    }

}


void setuprtc(){
    rtc.setTime(1758621217);
    ledcSetup(LEDC_CHANNEL, LEDC_FREQ, LEDC_TIMER);
    ledcAttachPin(LED_PIN, LEDC_CHANNEL);
    lastTouchMillis = millis();
}




void updatetime_daytime(lv_timer_t *timer){
    lv_label_set_text(objects.time_display, rtc.getTime("%R").c_str());
    lv_label_set_text(objects.second_time_display_1, rtc.getTime("%S").c_str());
    lv_label_set_text(objects.day_display, rtc.getTime("%A , %B %d").c_str());
}




void sound_timer(lv_timer_t *timer) {
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
                Serial.println("Clap 1 detected!");
            } else if (clapCount == 1 && (now - firstClapTime) < CLAP_TIMEOUT) {
                Serial.println("Clap 2 detected! Toggle LED");
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
            Serial.println("Timeout reset");
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
    Serial.println(">>> เข้าสู่โหมด Deep Sleep ...");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
    esp_deep_sleep_start();
  }
}