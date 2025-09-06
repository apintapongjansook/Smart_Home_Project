#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_login() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.login = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff5f1f1), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 116, 36);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff8b4dca), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "USER LOGIN");
        }
        {
            // password_txarea
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.password_txarea = obj;
            lv_obj_set_pos(obj, 125, 120);
            lv_obj_set_size(obj, 150, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff8b4dca), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 46, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Username");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 52, 130);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Password");
        }
        {
            // login_bt
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.login_bt = obj;
            lv_obj_set_pos(obj, 126, 180);
            lv_obj_set_size(obj, 69, 24);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff9d50fa), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff861eae), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Log in");
                }
            }
        }
        {
            // username_txarea
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.username_txarea = obj;
            lv_obj_set_pos(obj, 125, 74);
            lv_obj_set_size(obj, 150, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff8b4dca), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // keyboard_login
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard_login = obj;
            lv_obj_set_pos(obj, 0, 120);
            lv_obj_set_size(obj, 320, 120);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_login();
}

void delete_screen_login() {
    lv_obj_del(objects.login);
    objects.login = 0;
    objects.obj0 = 0;
    objects.password_txarea = 0;
    objects.login_bt = 0;
    objects.username_txarea = 0;
    objects.keyboard_login = 0;
}

void tick_screen_login() {
}

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffadd6fa), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 356, 232);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Hello, world!");
        }
        {
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 32);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 0, 17);
            lv_obj_set_size(obj, 320, 231);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffe4ebf2), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Home");
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 8, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Temp Status");
                        }
                        {
                            lv_obj_t *obj = lv_bar_create(parent_obj);
                            lv_obj_set_pos(obj, 8, 28);
                            lv_obj_set_size(obj, 33, 10);
                            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 44, 26);
                            lv_obj_set_size(obj, 20, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "0");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 8, 51);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "LED");
                        }
                        {
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.obj2 = obj;
                            lv_obj_set_pos(obj, 15, 78);
                            lv_obj_set_size(obj, 15, 18);
                            lv_led_set_color(obj, lv_color_hex(0xff19e740));
                            lv_led_set_brightness(obj, 255);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 41, 79);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "ON/OFF");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 64, 26);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Celsius");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Control");
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.obj3 = obj;
                            lv_obj_set_pos(obj, 17, 33);
                            lv_obj_set_size(obj, 39, 18);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1861d0), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Open LED");
                        }
                        {
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.obj4 = obj;
                            lv_obj_set_pos(obj, 0, 105);
                            lv_obj_set_size(obj, 49, 19);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff3ad432), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "ON");
                                }
                            }
                        }
                        {
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.obj5 = obj;
                            lv_obj_set_pos(obj, 56, 106);
                            lv_obj_set_size(obj, 50, 19);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffe50f0f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "OFF");
                                }
                            }
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, -1, 77);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Text");
                        }
                        {
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.obj6 = obj;
                            lv_obj_set_pos(obj, 147, 38);
                            lv_obj_set_size(obj, 128, 8);
                            lv_slider_set_value(obj, 25, LV_ANIM_OFF);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1dd3ce), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 147, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Temp 0");
                        }
                        {
                            lv_obj_t *obj = lv_checkbox_create(parent_obj);
                            lv_obj_set_pos(obj, 147, 67);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_checkbox_set_text(obj, "Light Auto");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 147, 105);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Test Buzzer");
                        }
                        {
                            lv_obj_t *obj = lv_led_create(parent_obj);
                            objects.obj7 = obj;
                            lv_obj_set_pos(obj, 178, 137);
                            lv_obj_set_size(obj, 21, 20);
                            lv_led_set_color(obj, lv_color_hex(0xff0000ff));
                            lv_led_set_brightness(obj, 255);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Settings");
                }
            }
        }
    }
    
    tick_screen_main();
}

void delete_screen_main() {
    lv_obj_del(objects.main);
    objects.main = 0;
    objects.obj1 = 0;
    objects.obj2 = 0;
    objects.obj3 = 0;
    objects.obj4 = 0;
    objects.obj5 = 0;
    objects.obj6 = 0;
    objects.obj7 = 0;
}

void tick_screen_main() {
}



typedef void (*create_screen_func_t)();
create_screen_func_t create_screen_funcs[] = {
    create_screen_login,
    create_screen_main,
};
void create_screen(int screen_index) {
    create_screen_funcs[screen_index]();
}
void create_screen_by_id(enum ScreensEnum screenId) {
    create_screen_funcs[screenId - 1]();
}

typedef void (*delete_screen_func_t)();
delete_screen_func_t delete_screen_funcs[] = {
    delete_screen_login,
    delete_screen_main,
};
void delete_screen(int screen_index) {
    delete_screen_funcs[screen_index]();
}
void delete_screen_by_id(enum ScreensEnum screenId) {
    delete_screen_funcs[screenId - 1]();
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_login,
    tick_screen_main,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_login();
    create_screen_main();
}
