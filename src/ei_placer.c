#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_placer.h"
#include "ei_frame.h"
#include "ei_widgetclass.h"




void		ei_place	(struct ei_widget_t*	widget,
                         ei_anchor_t*		anchor,
                         int*			x,
                         int*			y,
                         int*			width,
                         int*			height,
                         float*			rel_x,
                         float*			rel_y,
                         float*			rel_width,
                         float*			rel_height)
{
    ei_placer_params_t* param = malloc(sizeof(ei_placer_params_t));
    param->anchor = anchor;
    param->x = x;
    param->y = y;
    param->w = width;
    param->h = height;
    param->rx = rel_x;
    param->ry = rel_y;
    param->rw = rel_width;
    param->rh = rel_height;
    widget->placer_params = param;
    //ei_frame_t* frame = (ei_frame_t*)widget;
    ei_size_t parent_size = widget->parent->screen_location.size;
    ei_point_t parent_p = widget->parent->screen_location.top_left;
    int p1, p2;
    if (rel_x==NULL){ p1 = 0;}else{ p1 = (int)*rel_x;}
    if (rel_y == NULL){ p2 = 0;}else{ p2 = (int)*rel_y;}
    //if (anchor == (ei_anchor_t*)ei_anc_northwest) {
    ei_point_t top_left = ei_point( parent_p.x +p1 * parent_size.width + *x,  parent_p.y + p2 * parent_size.height + *y);
    ei_rect_t rect;
    ei_size_t s;
    if (width != NULL){
        s.width = *width;
    }
    else if (rel_width != NULL){
        s.width = (int)(*rel_width * (float)parent_size.width);
    }else{
        s.width = widget->requested_size.width;
    }
    if( height != NULL){
        s.height = *height;
    }else if( rel_height != NULL){
        s.height = (int)(*rel_height * (float)parent_size.height);
    }else{
        s.height = widget->requested_size.height;
    }
    rect = ei_rect(top_left, s);
    widget->screen_location = rect;
    //widget->parent->content_rect = &rect;
}


void ei_placer_run(struct ei_widget_t* widget) {
    ei_frame_t *frame = (ei_frame_t *) widget;
    if (widget->placer_params != NULL) {
        hw_text_compute_size(*frame->text, frame->text_font, &widget->requested_size.width,
                             &widget->requested_size.height);

    }
}


void ei_placer_forget(struct ei_widget_t* widget){

}