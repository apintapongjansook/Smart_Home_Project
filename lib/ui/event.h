#ifndef EVENT_H
#define EVENT_H
#include <lvgl.h>
#include "ui.h"
#if defined(EEZ_FOR_LVGL)
#include <eez/flow/lvgl_api.h>
#endif
#if !defined(EEZ_FOR_LVGL)
#include "screens.h"
#endif
#ifdef __cplusplus
extern "C"
{
#endif
    void event_handler(lv_event_t *e);
    void updatetime_daytime(lv_timer_t *timer);
    void setuprtc();
    void sound_timer(lv_timer_t *timer);
    void setupledc();
    void ledc(lv_timer_t *timer);
    void touchsleep(lv_timer_t *timer);

#ifdef __cplusplus
}
#endif
#endif