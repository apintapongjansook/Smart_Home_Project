#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_temp;
extern const lv_img_dsc_t img_sun;
extern const lv_img_dsc_t img_exit;
extern const lv_img_dsc_t img_setting;
extern const lv_img_dsc_t img_monitor;
extern const lv_img_dsc_t img_graph;
extern const lv_img_dsc_t img_humid;
extern const lv_img_dsc_t img_motion;
extern const lv_img_dsc_t img_idea;
extern const lv_img_dsc_t img_brightness;
extern const lv_img_dsc_t img_moon;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[11];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/