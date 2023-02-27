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
#include "button.h"
#include "ei_application.h"





//frame:

ei_widget_t* frame_allocfunc(void )
{
    ei_frame_t* nv_frame = calloc(1,sizeof(ei_frame_t));
    return (ei_widget_t*)nv_frame;
}

void frame_releasefunc(struct ei_widget_t* widget)
{
    ei_frame_t* frame = (ei_frame_t*) widget;
    free(frame->text_font);
    free(frame->img);
    free(frame->text_color);
    free(frame->text_anchor);
    free(frame->text);
    free(frame->img_anchor);
    free(frame->img_rect);
    free(frame->border_width);
    //free(frame->color);
    //free(frame->relief);
}

static ei_linked_point_t* transforme_rect(ei_rect_t* rect){
    ei_linked_point_t* top_left = malloc(sizeof(ei_linked_point_t*));
    ei_linked_point_t* top_right = malloc(sizeof(ei_linked_point_t*));
    ei_linked_point_t* bottom_left = malloc(sizeof(ei_linked_point_t*));
    ei_linked_point_t* bottom_right = malloc(sizeof(ei_linked_point_t*));
    ei_linked_point_t* last_point = malloc(sizeof(ei_linked_point_t));
    int largeur = rect->size.width;
    int hauteur = rect->size.height;
    int x = rect->top_left.x;
    int y = rect->top_left.y;

    last_point->point = ei_point(x,y);
    last_point->next = NULL;
    bottom_left->point = ei_point(x, y+hauteur);
    bottom_left->next = last_point;
    bottom_right->point = ei_point(x+largeur, y+hauteur);
    bottom_right->next = bottom_left;
    top_right->point = ei_point(x+largeur, y);
    top_right->next = bottom_right;
    top_left->point = ei_point(x,y);
    top_left->next = top_right;

    return top_left;
}


void frame_drawfunc(struct ei_widget_t*	widget,
                    ei_surface_t		surface,
                    ei_surface_t		pick_surface,
                    ei_rect_t*		clipper)
{
    ei_rect_t rect = widget->screen_location;
    ei_frame_t* frame = (ei_frame_t*)widget;
    ei_linked_point_t* first_point = transforme_rect(&rect);
    if(frame->border_width == NULL){
        ei_draw_polygon(surface, first_point, *(frame->color), clipper);
    }else {
        if (*frame->relief == ei_relief_raised || *frame->relief == ei_relief_none) {
            draw_frame_up(surface, &rect, *(frame->color), *frame->border_width, clipper);
        }else{
            draw_frame_down(surface, &rect, *(frame->color), *frame->border_width, clipper);
        }
    }
    ei_draw_polygon(pick_surface, first_point, *(widget->pick_color), clipper);
    free(first_point);
}


void frame_setdefault(ei_widget_t* widget)
{
    ei_frame_t* frame = (ei_frame_t*)widget;
    ei_color_t couleur = ei_default_background_color;
    ei_color_t txt_clr = ei_font_default_color;
    ei_font_t font = ei_default_font;
    ei_relief_t rf = ei_relief_none;
    ei_anchor_t anc_txt = ei_anc_center;
    ei_anchor_t anc_img = ei_anc_center;
    frame->color = &couleur;
    frame->border_width = NULL;
    frame->relief = &rf;
    frame->text = NULL;
    frame->text_font = &font;
    frame->text_color = &txt_clr;
    frame->text_anchor = &anc_txt;
    frame->img = NULL;
    frame->img_rect = NULL;
    frame->img_anchor = &anc_img;
}

ei_bool_t frame_handlefunc(ei_widget_t* widget, ei_event_t* event)
{
    return EI_FALSE;
}


// Button:

ei_widget_t* button_allocfunc(void)
{
    ei_button_t* button = calloc(1,sizeof(ei_button_t));
    return (ei_widget_t*)button;
}

void button_releasefunc(ei_widget_t* widget)
{
    ei_button_t* button = (ei_button_t*)widget;
    free(button->user_param);
    free(button->callback);
    free(button->img_rect);
    free(button->img_anchor);
    free(button->img);
    free(button->text_anchor);
    free(button->text_color);
    free(button->text_font);
    free(button->text);
    free(button->relief);
    free(button->corner_radius);
    free(button->relief);
}

void button_setdefault(ei_widget_t* widget)
{
    int corner = k_default_button_corner_radius;
    int border = k_default_button_border_width;
    ei_relief_t relief = ei_relief_raised;
    ei_font_t font = ei_default_font;
    ei_anchor_t anc = ei_anc_center;
    ei_button_t* button = (ei_button_t*)widget;
    button->color = (ei_color_t*)&ei_default_background_color;
    //frame->request_size = &(widget->requested_size);
    button->border_width = &border;
    button->relief = &relief;
    button->text = NULL;
    button->text_font = &font;
    button->text_color = (ei_color_t*)&ei_font_default_color;
    button->text_anchor = &anc;
    button->img = NULL;
    button->img_rect = NULL;
    button->img_anchor = &anc;
    button->corner_radius = &corner;
    button->callback = NULL;
    button->user_param = NULL;
}

void button_drawfunc(struct ei_widget_t*	widget,
                    ei_surface_t		surface,
                    ei_surface_t		pick_surface,
                    ei_rect_t*		clipper) {
    ei_rect_t rect = widget->screen_location;
    ei_button_t *button = (ei_button_t *) widget;
    if (*button->relief == ei_relief_raised ){
        draw_button_up(surface, &rect, *(button->color), (float) *button->corner_radius, *button->border_width, clipper);
    }else if(*button->relief == ei_relief_sunken){
        draw_button_down(surface, &rect, *(button->color), (float) *button->corner_radius, *button->border_width, clipper);
    }
    draw_button_up(pick_surface, &rect, *(widget->pick_color), (float)*button->corner_radius, 0, clipper);
    if (button->text != NULL) {
        ei_size_t s = ei_size_zero();
        char* text = *button->text;
        hw_text_compute_size(text, ei_default_font, &(s.width), &(s.height));
        ei_point_t where;
        //if (*button->text_anchor== ei_anc_center){
          where = ei_point_add(rect.top_left, ei_point(rect.size.width/2 - s.width/2, rect.size.height/2 - s.height/2));
        //}
        ei_draw_text(surface, &where,text, ei_default_font, *button->text_color, clipper);
    }
}

ei_bool_t button_handlefunc(ei_widget_t* widget,
                            ei_event_t* event )
{
    ei_button_t* button = (ei_button_t*)widget;
    ei_relief_t relief_s = ei_relief_sunken;
    ei_relief_t relief_r = ei_relief_raised;
    ei_point_t p = widget->screen_location.top_left;
    ei_size_t s = widget->screen_location.size;
    ei_point_t where = event->param.mouse.where;
    if ( p.x < where.x && where.x < p.x+s.width && p.y < where.y && where.y < p.y+s.height){
        if (event->type == ei_ev_mouse_buttondown) {
            button->relief = &relief_s;
            return EI_TRUE;
        }
    }
    button->relief = &relief_r;
    return EI_FALSE;

}

// TopLevel:

ei_widget_t* toplevel_allocfunc(void)
{
    ei_toplevel_t* top = malloc(sizeof(ei_toplevel_t));
    return (ei_widget_t*)top;
}

void toplevel_releasefunc(ei_widget_t* widget)
{
    ei_toplevel_t* top = (ei_toplevel_t*)widget;
    free(top->border_width);
    free(top->color);
    free(top->closable);
    free(top->min_size);
    free(top->resizable);
    free(top->title);
}

void toplevel_setdefault(ei_widget_t* widget)
{
    ei_toplevel_t* top = (ei_toplevel_t*)widget;
    int bw = 0;
    ei_bool_t true = EI_TRUE;
    ei_color_t back_clr = ei_default_background_color;
    char* text = "Toplevel";
    ei_size_t s = ei_size(160, 120);
    ei_size_t* min = &s;
    ei_axis_set_t axis = ei_axis_both;
    top->border_width = &bw;
    top->title = &text;
    top->color = &back_clr;
    top->closable = &true;
    top->resizable = &axis;
    top->min_size = &min;
}

void toplevel_drawfunc(ei_widget_t* widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t* clipper)
{
    ei_toplevel_t* top = (ei_toplevel_t*)widget;
    ei_color_t couleur_cadre = {100, 100, 100, 255};
    ei_size_t s;
    hw_text_compute_size(*top->title, ei_default_font, &s.width, &s.height);
    draw_button_up(surface, &widget->screen_location, couleur_cadre, 0, 0, clipper);
    ei_point_t p = widget->screen_location.top_left;
    ei_size_t sc = widget->screen_location.size;
    ei_point_t nv = ei_point(p.x+*top->border_width, p.y+s.height+*top->border_width*2);
    ei_rect_t nv_rect = ei_rect(nv, ei_size(sc.width- *top->border_width*2, sc.height - s.height -*top->border_width*3));
    draw_button_up(surface, &nv_rect, *top->color, 0, 0, clipper);
    if( *top->closable == EI_TRUE){
        ei_widget_t* button = ei_widget_create("button", widget, NULL, NULL);
        ei_color_t clr ={0, 255, 255, 255};
        int corn = 10;
        ei_anchor_t anc = ei_anc_northwest;
        ei_size_t rs = ei_size(20, 20);
        int y = s.height/2 + *top->border_width - rs.height/2;
        ei_button_configure(button, &rs, &clr, top->border_width, &corn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_place(button, &anc, top->border_width, &y, NULL, NULL, NULL, NULL, NULL, NULL);
        (*(button->wclass->drawfunc))(button, surface, pick_surface, clipper);
    }
    ei_color_t black = {0, 0, 0, 255};
    ei_point_t where = ei_point_add(widget->screen_location.top_left, ei_point(*top->border_width*2+s.height, *top->border_width));
    ei_draw_text(surface, &where,*top->title, ei_default_font, black, &widget->screen_location);

}

ei_bool_t toplevel_handlefunc(ei_widget_t* widget, ei_event_t* event)
{
    return EI_FALSE;
}


// les classes:



ei_widgetclass_t* registered_classes;

void ei_widgetclass_register	(ei_widgetclass_t* widgetclass)
{

    if (registered_classes == NULL){
        registered_classes = widgetclass;
    }else{
        widgetclass->next = registered_classes;
        registered_classes = widgetclass;
    }

}

void frame_class_reg(void)
{
    ei_widgetclass_t* frame_class = malloc(sizeof(ei_widgetclass_t));
    strcpy(frame_class->name, "frame");
    frame_class->drawfunc = &frame_drawfunc;
    frame_class->allocfunc = &frame_allocfunc;
    frame_class->releasefunc = &frame_releasefunc;
    frame_class->setdefaultsfunc = &frame_setdefault;
    frame_class->handlefunc = &frame_handlefunc;
    ei_widgetclass_register(frame_class);
}

void button_class_reg(void)
{
    ei_widgetclass_t* button_class = malloc(sizeof(ei_widgetclass_t));
    strcpy(button_class->name, "button");
    button_class->drawfunc = &button_drawfunc;
    button_class->allocfunc = &button_allocfunc;
    button_class->releasefunc = &button_releasefunc;
    button_class->setdefaultsfunc = &button_setdefault;
    button_class->handlefunc = &button_handlefunc;
    ei_widgetclass_register(button_class);
}

void toplevel_class_reg(void)
{
    ei_widgetclass_t* toplevel_class = malloc(sizeof(ei_widgetclass_t));
    strcpy(toplevel_class->name, "toplevel");
    toplevel_class->drawfunc = &toplevel_drawfunc;
    toplevel_class->allocfunc = &toplevel_allocfunc;
    toplevel_class->releasefunc = &toplevel_releasefunc;
    toplevel_class->setdefaultsfunc = &toplevel_setdefault;
    toplevel_class->handlefunc = &toplevel_handlefunc;
    ei_widgetclass_register(toplevel_class);
}

ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name)
{
    ei_widgetclass_t* cel = registered_classes;
    while (cel != NULL){
        if( cel->name[0] == *name){
            return cel;
        }
        cel = cel->next;
    }
    return NULL;
}