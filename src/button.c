#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"





static ei_linked_point_t* inserer_queue(ei_linked_point_t* p, ei_linked_point_t* q)
{
    ei_linked_point_t *Q1 = NULL;
    ei_linked_point_t *Q2 = NULL;
    ei_linked_point_t *tmp = NULL;
    tmp = p;
    if( q == NULL){
        return p;
    }else{
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        Q1 = tmp;
        tmp = q;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        Q2 = tmp;
        Q1->next = q;
        Q1 = Q2;
        return p;
    }
}



static ei_linked_point_t* semi_arc_montant(int x0, int y0, float rayon, float angle_debut, float angle_fin)
{
    int xi = (int)roundf(rayon*cosf(angle_debut));
    int yi = -(int)roundf(rayon*sinf(angle_debut));
    int xf = (int)roundf(rayon*cosf(angle_fin));
    int yf = -(int)roundf(rayon*sinf(angle_fin));
    ei_linked_point_t* cellule = malloc(sizeof(ei_linked_point_t));
    int x = xi;
    int y = yi;
    float theta = angle_debut;
    cellule->point = ei_point(x0+xi, y0+yi);
    ei_linked_point_t* curseur = cellule;
    while (y >= yf) {
        ei_linked_point_t* nouv = malloc(sizeof(ei_linked_point_t));
        nouv->point = ei_point(x0+x,y0+y);
        nouv->next = NULL;
        curseur->next = nouv;
        y -= 1;
        theta = - asinf( (float)y/rayon);
        x = (int)roundf(rayon * cosf(theta));
        curseur = curseur->next;
    }
    curseur = NULL;
    return cellule;
}

static ei_linked_point_t* semi_arc_descandant(int x0, int y0, float rayon, float angle_debut, float angle_fin)
{
    int xi = (int)roundf(rayon*cosf(angle_debut));
    int yi = -(int)roundf(rayon*sinf(angle_debut));
    int xf = (int)roundf(rayon*cosf(angle_fin));
    int yf = -(int)roundf(rayon*sinf(angle_fin));
    ei_point_t first_point = ei_point(x0+xi,y0+yi);
    ei_linked_point_t* cellule = malloc(sizeof(ei_linked_point_t));
    cellule->point = first_point;
    int x = xi;
    int y =yi;
    float theta = angle_debut;
    ei_linked_point_t* curseur = cellule;
    if (angle_debut == angle_fin){
        curseur->next = NULL;
        return cellule;
    }
    while (y < yf) {
        y += 1;
        theta = -asinf((float) y / rayon);
        x = -(int) roundf(rayon * cosf(theta));
        ei_linked_point_t* nouv = malloc(sizeof(ei_linked_point_t));
        nouv->point = ei_point(x0 + x, y + y0);
        nouv->next = NULL;
        curseur->next = nouv;
        curseur = curseur->next;
    }

    curseur = NULL;
    return cellule;
}

ei_linked_point_t* arc(ei_point_t centre, float rayon, float angle_debut, float angle_fin)
{
    int x0 = centre.x;
    int y0 = centre.y;
    ei_linked_point_t* first_point;
    if(rayon == (float)0){
        first_point = malloc(sizeof(ei_linked_point_t));
        first_point->point = centre;
        first_point->next = NULL;
        return first_point;
    }else {
        if (angle_debut >= -(float) M_PI / 2 && angle_debut < (float) M_PI / 2) {
            if (angle_fin >= -M_PI / 2 && angle_fin <= M_PI / 2) {
                if (angle_debut <= angle_fin) {
                    first_point = semi_arc_montant(x0, y0, rayon, angle_debut, angle_fin);
                    return first_point;
                } else {
                    ei_linked_point_t *T1 = semi_arc_montant(x0, y0, rayon, angle_debut, (float) M_PI / 2);
                    ei_linked_point_t *T2 = semi_arc_descandant(x0, y0, rayon, (float) M_PI / 2, -(float) M_PI / 2);
                    ei_linked_point_t *T3 = semi_arc_montant(x0, y0, rayon, -(float) M_PI / 2, angle_fin);
                    T2 = inserer_queue(T1, T2);
                    first_point = inserer_queue(T2, T3);
                    return first_point;
                }
            } else {
                ei_linked_point_t *T1 = semi_arc_montant(x0, y0, rayon, angle_debut, M_PI / 2);
                ei_linked_point_t *T2 = semi_arc_descandant(x0, y0, rayon, M_PI / 2, angle_fin);
                first_point = inserer_queue(T1, T2);
                return first_point;
            }
        } else {
            if (angle_fin <= -(float) M_PI / 2 || angle_fin >= (float) M_PI / 2) {
                if (angle_fin * angle_debut > 0) {
                    if (angle_debut <= angle_fin) {
                        first_point = semi_arc_descandant(x0, y0, rayon, angle_debut, angle_fin);
                        return first_point;
                    } else {
                        ei_linked_point_t *T1 = semi_arc_descandant(x0, y0, rayon, angle_debut, -(float) M_PI / 2);
                        ei_linked_point_t *T2 = semi_arc_montant(x0, y0, rayon, -(float) M_PI / 2, (float) M_PI / 2);
                        ei_linked_point_t *T3 = semi_arc_descandant(x0, y0, rayon, (float) M_PI / 2, angle_fin);
                        T2 = inserer_queue(T1, T2);
                        first_point = inserer_queue(T2, T3);
                        return first_point;
                    }
                } else {
                    if (angle_debut >= angle_fin) {
                        first_point = semi_arc_descandant(x0, y0, rayon, angle_debut, angle_fin);
                        return first_point;
                    } else {
                        ei_linked_point_t *T1 = semi_arc_descandant(x0, y0, rayon, angle_debut, -(float) M_PI / 2);
                        ei_linked_point_t *T2 = semi_arc_montant(x0, y0, rayon, -(float) M_PI / 2, (float) M_PI / 2);
                        ei_linked_point_t *T3 = semi_arc_descandant(x0, y0, rayon, (float) M_PI / 2, angle_fin);
                        T2 = inserer_queue(T1, T2);
                        first_point = inserer_queue(T2, T3);
                        return first_point;
                    }
                }
            } else {
                ei_linked_point_t *T1 = semi_arc_descandant(x0, y0, rayon, angle_debut, -(float) M_PI / 2);
                ei_linked_point_t *T2 = semi_arc_montant(x0, y0, rayon, -(float) M_PI / 2, angle_fin);
                first_point = inserer_queue(T1, T2);
                return first_point;
            }
        }
    }

}

int min_int(int x, int y)
{
    if (x > y)
    {
        return y;
    }else{
        return x;
    }
}




ei_linked_point_t* rounded_frame_total(ei_rect_t* rectangle, float rayon)
{
    int h = rectangle->size.height;
    int l = rectangle->size.width;
    int r = (int)roundf(rayon);
    ei_point_t p1 = rectangle->top_left;
    ei_point_t c1 = ei_point(p1.x+r, p1.y+r);
    ei_point_t c2 = ei_point(p1.x+r, p1.y+h-r);
    ei_point_t c3 = ei_point(p1.x+l-r, p1.y+h-r);
    ei_point_t c4 = ei_point(p1.x+l-r, p1.y+r);
    ei_linked_point_t* frame = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* arc1 = arc(c1, rayon, (float) M_PI/2, (float)M_PI);
    ei_linked_point_t* arc2 = arc(c2, rayon, -(float) M_PI, -(float)M_PI/2);
    ei_linked_point_t* arc3 = arc(c3, rayon, -(float) M_PI/2, (float)0);
    ei_linked_point_t* arc4 = arc(c4, rayon, (float) 0, (float)M_PI/2);
    ei_point_t first_point = arc1->point;
    arc1 = inserer_queue(arc1, arc2);
    arc1 = inserer_queue(arc1, arc3);
    arc1 = inserer_queue(arc1, arc4);
    frame->point = first_point;
    frame->next = NULL;
    frame = inserer_queue(arc1, frame);
    return frame;
}


ei_linked_point_t* rounded_semi_frame_haut(ei_rect_t* rectangle, float rayon)
{
    int haut = rectangle->size.height;
    int larg = rectangle->size.width;
    int h = min_int(haut/2, larg/2);
    int r = (int)roundf(rayon);
    ei_point_t p1 = rectangle->top_left;
    ei_point_t c1 = ei_point(p1.x+r, p1.y+r);
    ei_point_t c2 = ei_point(p1.x+r, p1.y+haut-r);
    ei_point_t c3 = ei_point(p1.x+larg-r, p1.y+haut-r);
    ei_point_t c4 = ei_point(p1.x+larg-r, p1.y+r);
    ei_point_t c5 = ei_point(p1.x+h, p1.y+haut-h);
    ei_point_t c6 = ei_point(p1.x+larg-h, p1.y+h);
    ei_linked_point_t* nv1 = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* nv2 = malloc(sizeof(ei_linked_point_t));
    nv1->point = c5;
    nv2->point = c6;
    nv2->next = NULL;
    nv1->next = nv2;
    ei_linked_point_t* frame = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* arc1 = arc(c1, rayon, (float) M_PI/2, (float)M_PI);
    ei_linked_point_t* arc2 = arc(c2, rayon, -(float) M_PI, -(float)(3*M_PI/4));
    ei_linked_point_t* arc3 = arc(c4, rayon, (float) M_PI/4, (float)M_PI/2);
    ei_point_t first_point = arc1->point;
    arc1 = inserer_queue(arc1, arc2);
    arc1 = inserer_queue(arc1, nv1);
    arc1 = inserer_queue(arc1, arc3);
    frame->point = first_point;
    frame->next = NULL;
    frame = inserer_queue(arc1, frame);
    return frame;
}

ei_linked_point_t* rounded_semi_frame_bas(ei_rect_t* rectangle, float rayon)
{
    int haut = rectangle->size.height;
    int larg = rectangle->size.width;
    int r = (int)roundf(rayon);
    int h = min_int(haut/2, larg/2);
    ei_point_t p1 = rectangle->top_left;
    ei_point_t c1 = ei_point(p1.x+r, p1.y+r);
    ei_point_t c2 = ei_point(p1.x+r, p1.y+haut-r);
    ei_point_t c3 = ei_point(p1.x+larg-r, p1.y+haut-r);
    ei_point_t c4 = ei_point(p1.x+larg-r, p1.y+r);
    ei_point_t c5 = ei_point(p1.x+h, p1.y+haut-h);
    ei_point_t c6 = ei_point(p1.x+larg-h, p1.y+h);
    ei_linked_point_t* nv1 = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* nv2 = malloc(sizeof(ei_linked_point_t));
    nv1->point = c6;
    nv2->point = c5;
    nv2->next = NULL;
    nv1->next = nv2;

    ei_linked_point_t* frame = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t* arc1 = arc(c2, rayon, -(float) (3*M_PI/4), -(float)M_PI/2);
    ei_linked_point_t* arc2 = arc(c3, rayon, -(float) M_PI/2, (float)0);
    ei_linked_point_t* arc3 = arc(c4, rayon, (float) 0, (float)M_PI/4);
    ei_point_t first_point = arc1->point;
    arc1 = inserer_queue(arc1, arc2);
    arc1 = inserer_queue(arc1, arc3);
    arc1 = inserer_queue(arc1, nv1);
    frame->point = first_point;
    frame->next = NULL;
    frame = inserer_queue(arc1, frame);
    return frame;
}


ei_linked_point_t* rounded_frame(ei_rect_t* rectangle, float rayon, int type)
{
    if( type == 0)
    {
        return rounded_frame_total(rectangle, rayon);
    }
    else if (type == 1)
    {
        return rounded_semi_frame_haut(rectangle, rayon);
    }
    else
    {
        return rounded_semi_frame_bas(rectangle, rayon);
    }
}

void draw_button_up(ei_surface_t surface, ei_rect_t* rectangle, ei_color_t color, float rayon, int epaisseur, ei_rect_t* clipper)
{

    if (rectangle->size.height == 0 && rectangle->size.width == 0){}
    else {
        ei_color_t fonce = {color.red + 20, color.green + 20, color.blue + 20, color.alpha};
        ei_color_t clair = {color.red - 20, color.green - 20, color.blue - 20, color.alpha};
        ei_linked_point_t *frame_haut = rounded_frame(rectangle, rayon, 1);
        ei_draw_polygon(surface, frame_haut, fonce, clipper);
        free(frame_haut);
        ei_linked_point_t *frame_bas = rounded_frame(rectangle, rayon, 2);
        ei_draw_polygon(surface, frame_bas, clair, clipper);
        free(frame_bas);
        ei_point_t p1 = rectangle->top_left;
        ei_point_t nv_point = ei_point(p1.x + epaisseur, p1.y + epaisseur);
        ei_size_t s1 = rectangle->size;
        ei_size_t nv_size = ei_size(s1.width - 2 * epaisseur, s1.height - 2 * epaisseur);
        //float prc = (float)nv_size.height/(float)s1.height;
        ei_rect_t nv_rect = ei_rect(nv_point, nv_size);
        ei_linked_point_t *frame_tot = rounded_frame(&nv_rect, rayon-(float)epaisseur, 0);
        ei_draw_polygon(surface, frame_tot, color, clipper);
    }
}

void draw_button_down(ei_surface_t surface, ei_rect_t* rectangle, ei_color_t color, float rayon, int epaisseur, ei_rect_t* clipper)
{
    if (rectangle->size.height == 0 && rectangle->size.width == 0){}
    else {
        ei_color_t fonce = {color.red + 20, color.green + 20, color.blue + 20, color.alpha};
        ei_color_t clair = {color.red - 20, color.green - 20, color.blue - 20, color.alpha};
        ei_linked_point_t *frame_haut = rounded_frame(rectangle, rayon, 1);
        ei_draw_polygon(surface, frame_haut, clair, clipper);
        free(frame_haut);
        ei_linked_point_t *frame_bas = rounded_frame(rectangle, rayon, 2);
        ei_draw_polygon(surface, frame_bas, fonce, clipper);
        free(frame_bas);
        ei_point_t p1 = rectangle->top_left;
        ei_point_t nv_point = ei_point(p1.x + epaisseur, p1.y + epaisseur);
        ei_size_t s1 = rectangle->size;
        ei_size_t nv_size = ei_size(s1.width - 2 * epaisseur, s1.height - 2 * epaisseur);
        float prc = (float) nv_size.height / (float) s1.height;
        ei_rect_t nv_rect = ei_rect(nv_point, nv_size);
        ei_linked_point_t *frame_tot = rounded_frame(&nv_rect, rayon- (float)epaisseur, 0);
        ei_draw_polygon(surface, frame_tot, color, clipper);
        free(frame_tot);
    }

}

void draw_frame_up(ei_surface_t surface, ei_rect_t* rectangle, ei_color_t color, int border_width, ei_rect_t* clipper)
{
    if (rectangle->size.height == 0 && rectangle->size.width == 0){}
    else {
        ei_color_t fonce = {color.red + 20, color.green + 20, color.blue + 20, color.alpha};
        ei_color_t clair = {color.red - 20, color.green - 20, color.blue - 20, color.alpha};
        ei_linked_point_t *frame_haut = rounded_frame(rectangle, 0, 1);
        ei_draw_polygon(surface, frame_haut, fonce, clipper);
        free(frame_haut);
        ei_linked_point_t *frame_bas = rounded_frame(rectangle, 0, 2);
        ei_draw_polygon(surface, frame_bas, clair, clipper);
        free(frame_bas);
        ei_point_t p1 = rectangle->top_left;
        ei_point_t nv_point = ei_point(p1.x + border_width, p1.y + border_width);
        ei_size_t s1 = rectangle->size;
        ei_size_t nv_size = ei_size(s1.width - 2 * border_width, s1.height - 2 * border_width);
        //float prc = (float)nv_size.height/(float)s1.height;
        ei_rect_t nv_rect = ei_rect(nv_point, nv_size);
        ei_linked_point_t *frame_tot = rounded_frame(&nv_rect, 0, 0);
        ei_draw_polygon(surface, frame_tot, color, clipper);
    }
}

void draw_frame_down(ei_surface_t surface, ei_rect_t* rectangle, ei_color_t color, int border_width, ei_rect_t* clipper)
{
    if (rectangle->size.height == 0 && rectangle->size.width == 0){}
    else {
        ei_color_t fonce = {color.red - 20, color.green - 20, color.blue - 20, color.alpha};
        ei_color_t clair = {color.red + 20, color.green + 20, color.blue + 20, color.alpha};
        ei_linked_point_t *frame_haut = rounded_frame(rectangle, 0, 1);
        ei_draw_polygon(surface, frame_haut, fonce, clipper);
        free(frame_haut);
        ei_linked_point_t *frame_bas = rounded_frame(rectangle, 0, 2);
        ei_draw_polygon(surface, frame_bas, clair, clipper);
        free(frame_bas);
        ei_point_t p1 = rectangle->top_left;
        ei_point_t nv_point = ei_point(p1.x + border_width, p1.y + border_width);
        ei_size_t s1 = rectangle->size;
        ei_size_t nv_size = ei_size(s1.width - 2 * border_width, s1.height - 2 * border_width);
        //float prc = (float)nv_size.height/(float)s1.height;
        ei_rect_t nv_rect = ei_rect(nv_point, nv_size);
        ei_linked_point_t *frame_tot = rounded_frame(&nv_rect, 0, 0);
        ei_draw_polygon(surface, frame_tot, color, clipper);
    }
}


void draw_toplevel(ei_surface_t surface, ei_rect_t rect, ei_color_t color, int border_width)
{

}