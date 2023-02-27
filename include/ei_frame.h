#ifndef EI_FRAME_H
#define EI_FRAME_H

#include <stdint.h>
#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_placer.h"

typedef struct {
    ei_widget_t   widget;
    //ei_size_t*  request_size;
    ei_color_t*  color;
    ei_relief_t*   relief;
    int* border_width;
    char** text;
    ei_font_t*  text_font;
    ei_color_t* text_color;
    ei_anchor_t* text_anchor;
    ei_surface_t*  img;
    ei_rect_t** img_rect;
    ei_anchor_t* img_anchor;
}ei_frame_t;

typedef struct {
    ei_widget_t widget;
    ei_color_t* color;
    int* border_width;
    int* corner_radius;
    ei_relief_t* relief;
    char** text;
    ei_font_t* text_font;
    ei_color_t* text_color;
    ei_anchor_t* text_anchor;
    ei_surface_t* img;
    ei_rect_t** img_rect;
    ei_anchor_t* img_anchor;
    ei_callback_t* callback;
    void** user_param;
}ei_button_t;

typedef struct {
    ei_widget_t   widget;
    ei_color_t*  color;
    int* border_width;
    char** title;
    ei_bool_t* closable;
    ei_axis_set_t* resizable;
    ei_size_t** min_size;
}ei_toplevel_t;


void frame_class_reg(void);

void button_class_reg(void);

void toplevel_class_reg(void);

ei_widget_t* frame_allocfunc(void );

void button_setdefault(ei_widget_t* widget);

void frame_setdefault(ei_widget_t* widget);



#endif