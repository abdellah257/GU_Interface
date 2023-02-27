#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_placer.h"
#include "ei_frame.h"
#include "ei_application.h"





ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
                                             ei_widget_t*		parent,
                                             void*			user_data,
                                             ei_widget_destructor_t destructor)
{
    uint32_t id;
    ei_widgetclass_t* class = ei_widgetclass_from_name(class_name);
    ei_widget_t* widget = class->allocfunc();
    widget->wclass = class;
    widget->pick_id = id;
    ei_color_t* color = malloc(sizeof(ei_color_t*));
    color->red = id & 0xFF;
    color->green = (id >> 8) & 0xFF;
    color->blue = (id >> 16) & 0xFF;
    color->alpha = (id >> 24) & 0xFF;
    widget->pick_color = color;
    widget->parent = parent;
    if(parent->children_head== NULL){
        parent->children_head = widget;
        parent->children_tail = widget;
    }
    else{
        if(parent->children_head== parent->children_tail){
            parent->children_tail = widget;
            parent->children_head->next_sibling = widget;
        }else{
            parent->children_tail->next_sibling = widget;
            parent->children_tail = widget;
        }
    }
    widget->destructor = destructor;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;
    widget->placer_params = NULL;
}


ei_widget_t*		ei_widget_pick			(ei_point_t*		where)
{
    ei_widget_t* widget = ei_app_root_widget();
    while (widget->children_head != NULL){
        ei_point_t p = widget->children_head->screen_location.top_left;
        ei_size_t s = widget->children_head->screen_location.size;
        if(p.x<where->x && where->x < p.x+s.width && p.y< where->y && where->y < p.y+s.height){
            widget = widget->children_head;
        }else{
            if(widget != ei_app_root_widget()){
                return widget;
            }else{
                return NULL;
            }
        }
    }
    if(widget != ei_app_root_widget()){
        return widget;
    }else{
        return NULL;
    }
}


void			ei_widget_destroy		(ei_widget_t*		widget)
{
    (*(widget->wclass->releasefunc))(widget);
    free(widget);
}


void			ei_frame_configure		(ei_widget_t*		widget,
                                           ei_size_t*		requested_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_relief_t*		relief,
                                           char**			text,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color,
                                           ei_anchor_t*		text_anchor,
                                           ei_surface_t*		img,
                                           ei_rect_t**		img_rect,
                                           ei_anchor_t*		img_anchor)
{
    ei_frame_t* frame = (ei_frame_t*)widget;
    ei_relief_t relief_d = ei_relief_raised;
    if (requested_size == NULL){
        widget->requested_size = ei_size(0, 0);
    }else {
        widget->requested_size = *requested_size;
    }if(color != NULL){
        frame->color = (ei_color_t*)color;
    }else{
        frame->color = (ei_color_t*)&ei_font_default_color;
    }if(border_width != NULL){
        frame->border_width = border_width;
    }else{
        //int* x =0;
        frame->border_width = (int*)0;
    }if (relief != NULL){
        frame->relief = relief;
    }else{
        frame->relief = &relief_d;
    }if(text != NULL){
        frame->text = text;
    }else{
        frame->text = NULL;
    }if(text_font != NULL){
        frame->text_font = text_font;
    }else{
        frame->text_font = ei_default_font;
    }if(text_color != NULL){
        frame->text_color = text_color;
    }else{
        frame->text_color = (ei_color_t*)&ei_font_default_color;
    }if(text_anchor != NULL){
        frame->text_anchor = text_anchor;
    }else{
        frame->text_anchor = (ei_anchor_t*)ei_anc_center;
    }if(img != NULL){
        frame->img = img;
    }else{
        frame->img = NULL;
    }if( img_rect != NULL){
        frame->img_rect = img_rect;
    }else{
        frame->img_rect = NULL;
    }if(img_anchor != NULL){
        frame->img_anchor = img_anchor;
    }else{
        frame->img_anchor = (ei_anchor_t*)ei_anc_center;
    }

}

void			ei_button_configure		(ei_widget_t*		widget,
                                            ei_size_t*		requested_size,
                                            const ei_color_t*	color,
                                            int*			border_width,
                                            int*			corner_radius,
                                            ei_relief_t*		relief,
                                            char**			text,
                                            ei_font_t*		text_font,
                                            ei_color_t*		text_color,
                                            ei_anchor_t*		text_anchor,
                                            ei_surface_t*		img,
                                            ei_rect_t**		img_rect,
                                            ei_anchor_t*		img_anchor,
                                            ei_callback_t*		callback,
                                            void**			user_param)
{

    button_setdefault(widget);
    ei_button_t* button = (ei_button_t*)widget;
    if (requested_size == NULL){
        ei_size_t s = ei_size_zero();
        if (text != NULL) {
            if (text_font != NULL){
                hw_text_compute_size(*text, *text_font, &s.width, &s.height);
                widget->requested_size = ei_size(s.width + 4, s.height + 4);
            }else{
                hw_text_compute_size(*text, ei_default_font, &s.width, &s.height);
                widget->requested_size = ei_size(s.width + 4, s.height + 4);
            }
        }else{
            widget->requested_size = ei_size(0, 0);
        }
    }else {
        widget->requested_size = *requested_size;
    }
    if ( color != NULL){
        button->color = (ei_color_t*)color;
    }
    if (border_width != NULL){
        button->border_width = border_width;
    }
    if (corner_radius != NULL){
        button->corner_radius = corner_radius;
    }
    if ( relief != NULL){
        button->relief = relief;
    }
    if ( text != NULL){
        button->text = text;
    }
    if ( text_font != NULL){
        button->text_font = text_font;
    }
    if ( text_color != NULL){
        button->text_color = text_color;
    }
    if ( text_anchor != NULL){
        button->text_anchor = text_anchor;
    }
    if ( img != NULL){
        button->img = img;
    }
    if ( img_rect != NULL){
        button->img_rect = img_rect;
    }
    if ( img_anchor != NULL){
        button->img_anchor = img_anchor;
    }
    if ( callback != NULL){
        button->callback = callback;
    }
    if ( user_param != NULL){
        button->user_param = user_param;
    }
}

void			ei_toplevel_configure		(ei_widget_t*		widget,
                                              ei_size_t*		requested_size,
                                              ei_color_t*		color,
                                              int*			border_width,
                                              char**			title,
                                              ei_bool_t*		closable,
                                              ei_axis_set_t*		resizable,
                                              ei_size_t**		min_size)
{
    (*(widget->wclass->setdefaultsfunc))(widget);
    if (requested_size == NULL){
        widget->requested_size = ei_size(0, 0);
    }else {
        widget->requested_size = *requested_size;
    }
    ei_toplevel_t* top = (ei_toplevel_t*)widget;
    if (color != NULL){
        top->color = color;
    }
    if (border_width != NULL){
        top->border_width = border_width;
    }
    if (title != NULL){
        top->title = title;
    }
    if (closable != NULL){
        top->closable = closable;
    }
    if (resizable != NULL){
        top->resizable = resizable;
    }
    if (min_size != NULL){
        top->min_size = min_size;
    }

}

