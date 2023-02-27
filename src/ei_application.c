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

ei_surface_t main_window;
ei_surface_t off_screen;
ei_widget_t* widget;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    hw_init();
    main_window = hw_create_window(main_window_size, fullscreen);
    off_screen = hw_surface_create(main_window, main_window_size, EI_FALSE);
    frame_class_reg();
    button_class_reg();
    toplevel_class_reg();
    widget = frame_allocfunc();
    uint32_t id;
    ei_color_t color = { 0xff, 0xff, 0xff, 0xff };
    //ei_size_t main_window_size = hw_surface_get_size(main_window);
    widget->wclass = ei_widgetclass_from_name("frame_class");
    widget->pick_id = id;
    widget->parent = NULL;
    widget->children_tail = NULL;
    widget->children_head = NULL;
    widget->next_sibling = NULL;
    widget->pick_color = &color;
    widget->placer_params = NULL;
    widget->destructor = NULL;
    widget->requested_size = main_window_size;
    widget->screen_location = ei_rect(ei_point(0, 0), main_window_size);
    widget->content_rect = NULL;
    widget->user_data = NULL;
    (*(widget->wclass->setdefaultsfunc))(widget);
}


void ei_app_free(void){

}

ei_widget_t* ei_app_root_widget(void){
    return widget;
}

void draw_arbre(ei_widget_t* widgett)
{
    if(widgett->children_head==NULL){
        (*(widgett->wclass->drawfunc))(widgett, main_window, off_screen, &widgett->screen_location);
    }
    else{
        (*(widgett->wclass->drawfunc))(widgett, main_window, off_screen, &widgett->screen_location);
        widgett = widgett->children_head;
        while(widgett->next_sibling!= NULL){
            draw_arbre(widgett);
            widgett = widgett->next_sibling;
        }
        draw_arbre(widgett);
    }
}

void draw_arbre_evnt(ei_widget_t* widgett, ei_event_t* event)
{
    //(*(widgett->wclass->handlefunc))(widgett, event);
    //(*(widgett->wclass->drawfunc))(widgett, main_window, off_screen, &widgett->screen_location);
    if(widgett->children_head==NULL){
        if ((*(widgett->wclass->handlefunc))(widgett, event) == EI_TRUE){
            (*(widgett->wclass->drawfunc))(widgett, main_window, off_screen, &widgett->screen_location);
            hw_surface_unlock(main_window);
            hw_surface_update_rects(main_window, NULL);
        }else{
            (*(widgett->wclass->drawfunc))(widgett, main_window, off_screen, &widgett->screen_location);
        }
    }
    else{
        if ((*(widgett->wclass->handlefunc))(widgett, event) == EI_TRUE){
            (*(widgett->wclass->drawfunc))(widgett, main_window, off_screen, &widgett->screen_location);
            hw_surface_unlock(main_window);
            hw_surface_update_rects(main_window, NULL);
        }else {
            (*(widgett->wclass->drawfunc))(widgett, main_window, off_screen, &widgett->screen_location);
            widgett = widgett->children_head;
            while (widgett->next_sibling != NULL) {
                draw_arbre_evnt(widgett, event);
                widgett = widgett->next_sibling;
            }
            draw_arbre_evnt(widgett, event);
        }
    }
}


void ei_app_run(void) {
    widget = ei_app_root_widget();
    ei_widget_t *curseur = widget;
    ei_event_t event;
    hw_surface_lock(main_window);
    /*while (curseur != NULL) {
        (*(curseur->wclass->drawfunc))(curseur, main_window, off_screen, NULL);
        curseur = curseur->children_head;
    }
    */
    draw_arbre(widget);
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);
    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown) {
        if (event.type == ei_ev_mouse_buttonup || event.type == ei_ev_mouse_buttondown) {
            hw_surface_lock(main_window);
            widget = ei_app_root_widget();
            draw_arbre_evnt(widget, &event);
            hw_surface_unlock(main_window);
            hw_surface_update_rects(main_window, NULL);
        }
        hw_event_wait_next(&event);
    }
    //hw_surface_unlock(main_window);
    //hw_surface_update_rects(main_window, NULL);
    hw_quit();

}

