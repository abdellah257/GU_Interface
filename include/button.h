#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "ei_types.h"




ei_linked_point_t* arc(ei_point_t centre, float rayon, float angle_debut, float angle_fin);


ei_linked_point_t* rounded_frame(ei_rect_t* rectangle, float rayon, int type);

void draw_button_up(ei_surface_t surface, ei_rect_t* rectangle, ei_color_t color, float rayon, int epaisseur, ei_rect_t* clipper);

void draw_button_down(ei_surface_t surface, ei_rect_t* rectangle, ei_color_t color, float rayon, int epaisseur, ei_rect_t* clipper);

void draw_frame_up(ei_surface_t surface, ei_rect_t* rectangle, ei_color_t color, int border_width, ei_rect_t* clipper);

void draw_frame_down(ei_surface_t surface, ei_rect_t* rectangle, ei_color_t color, int border_width, ei_rect_t* clipper);

#endif
