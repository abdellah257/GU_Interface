#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "button.h"










void	ei_draw_polyline	(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 ei_color_t			color,
						 const ei_rect_t*		clipper)
{
    int X, Y, XX, YY;
    if (clipper != NULL) {
        Y = clipper->top_left.y;
        X = clipper->top_left.x;
        XX = X + clipper->size.width;
        YY = Y + clipper->size.height;
    }else{
        X = 0;
        Y = 0;
        XX = hw_surface_get_size(surface).width;
        YY = hw_surface_get_size(surface).height;
    }
    if (first_point == NULL){}
    else {
        uint32_t* first_pixel;
        first_pixel = (uint32_t*)hw_surface_get_buffer(surface);
        int x = first_point->point.x;
        int y = first_point->point.y;
        uint32_t* f_pixel = first_pixel + hw_surface_get_size(surface).width * (y - 1) + x;
        if (first_point->next == NULL)
        {
            if (x>= X && x <= XX && y >= Y && y <= YY) {
                *f_pixel = (color.red << 24) + (color.green << 16) + (color.blue << 8) + color.alpha;
            }
        }
        while (first_point->next != NULL)
        {
            int dx = first_point->next->point.x - first_point->point.x;
            int dy = first_point->next->point.y - first_point->point.y;
            if (dx == 0 && dy == 0){
                if (x>= X && x <= XX && y >= Y && y <= YY) {
                    *f_pixel = (color.red << 24) + (color.green << 16) + (color.blue << 8) + color.alpha;
                }
            }
            int e = 0;
            if (abs(dy)< abs(dx)){
                int x0 = first_point->point.x;
                int y0 = first_point->point.y;
                int x1 = first_point->next->point.x;
                int y1 = first_point->next->point.y;
                if (x0 > x1){
                    int sgn = 1;
                    if (dy > 0) {
                        sgn = -1;
                        dy = -dy;
                    }
                    while (x1 < x0) {
                        f_pixel = first_pixel + hw_surface_get_size(surface).width * (y1 - 1) + x1;
                        if (x1>= X && x1 <= XX && y1 >= Y && y1 <= YY) {
                            *f_pixel = (color.red << 24) + (color.green << 16) + (color.blue << 8) + color.alpha;
                        }
                        x1 += 1;
                        e -= dy;
                        if (2 * e > -dx) {
                            y1 += sgn;
                            e += dx;
                        }
                    }
                }else {
                    int sgn = 1;
                    if (dy < 0) {
                        sgn = -1;
                        dy = -dy;
                    }
                    while (x0 < x1) {
                        f_pixel = first_pixel + hw_surface_get_size(surface).width * (y0 - 1) + x0;
                        if (x0 >= X && x0 <= XX && y0 >= Y && y0 <= YY) {
                            *f_pixel = (color.red << 24) + (color.green << 16) + (color.blue << 8) + color.alpha;
                        }
                        x0 += 1;
                        e += dy;
                        if (2 * e > dx) {
                            y0 += sgn;
                            e -= dx;
                        }
                    }
                }
            }else {
                int x0 = first_point->point.x;
                int y0 = first_point->point.y;
                int x1 = first_point->next->point.x;
                int y1 = first_point->next->point.y;
                if (y0 > y1){
                    int sgn = 1;
                    if (dx > 0) {
                        sgn = -1;
                        dx = -dx;
                    }
                    while (y1 < y0) {
                        f_pixel = first_pixel + hw_surface_get_size(surface).width * (y1 - 1) + x1;
                        if (x1 >= X && x1 <= XX && y1 >= Y && y1 <= YY) {
                            *f_pixel = (color.red << 24) + (color.green << 16) + (color.blue << 8) + color.alpha;
                        }
                        y1 += 1;
                        e -= dx;
                        if (2 * e > -dy) {
                            x1 += sgn;
                            e += dy;
                        }
                    }
                }else {
                    int sgn = 1;
                    if (dx < 0) {
                        sgn = -1;
                        dx = -dx;
                    }
                    while (y0 < y1) {
                        f_pixel = first_pixel + hw_surface_get_size(surface).width * (y0 - 1) + x0;
                        if (x0 >= X && x0 <= XX && y0 >= Y && y0 <= YY) {
                            *f_pixel = (color.red << 24) + (color.green << 16) + (color.blue << 8) + color.alpha;
                        }
                        y0 += 1;
                        e += dx;
                        if (2 * e > dy) {
                            x0 += sgn;
                            e -= dy;
                        }
                    }
                }
            }
            first_point = first_point->next;
        }
    }
}


void			ei_fill			(ei_surface_t		surface,
                                    const ei_color_t*	color,
                                    const ei_rect_t*	clipper)
{
    uint32_t*			pixel_ptr;
    int				i;
    pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
    for (i = 0; i < (hw_surface_get_size(surface).width * hw_surface_get_size(surface).height); i++)
        *pixel_ptr++ = (color->red<< 24) + (color->green<<16) + (color->blue<<8) + color->alpha;
}


struct cote {
    int ymax;
    float xymin;
    int err;
    float deltax;
    int deltay;
    struct cote* suivant;
};

static void remplir(const ei_linked_point_t* first_point, struct cote** TeC)
{
    ei_point_t p1 = first_point->point;
    int y = p1.y;
    while (first_point->next->point.x != p1.x || first_point->next->point.y != y){
        if (first_point->next->point.y == first_point->point.y){}
        else{
            if (first_point->next->point.y < first_point->point.y){
                struct cote *nv = malloc(sizeof(struct cote));
                nv->ymax = first_point->point.y;
                nv->xymin = (float) first_point->next->point.x;
                nv->deltax = (float) first_point->point.x - (float) first_point->next->point.x;
                nv->deltay = first_point->point.y - first_point->next->point.y;
                nv->err = 0;
                nv->suivant = TeC[first_point->next->point.y];
                TeC[first_point->next->point.y] = nv;
            }else{
                struct cote* nv = malloc(sizeof(struct cote));
                nv->ymax = first_point->next->point.y;
                nv->xymin = (float)first_point->point.x;
                nv->deltax = (float)first_point->next->point.x - (float)first_point->point.x;
                nv->deltay = first_point->next->point.y-first_point->point.y;
                nv->err = 0;
                nv->suivant = TeC[first_point->point.y];
                TeC[first_point->point.y] = nv;
            }
        }
        first_point = first_point->next;
    }
    if (first_point->point.y < y){
        struct cote* nv = malloc(sizeof(struct cote));
        nv->ymax = y;
        nv->xymin = (float)first_point->point.x;
        nv->deltax = -(float)first_point->point.x + (float)p1.x;
        nv->deltay = -first_point->point.y + y;
        nv->err = 0;
        nv->suivant = TeC[first_point->point.y];
        TeC[first_point->point.y] = nv;

    }else {
        struct cote *nv = malloc(sizeof(struct cote));
        nv->ymax = first_point->point.y;
        nv->xymin = (float) p1.x;
        nv->deltax = (float) first_point->point.x - (float) p1.x;
        nv->deltay = first_point->point.y - y;
        nv->err = 0;
        nv->suivant = TeC[y];
        TeC[y] = nv;
    }
}

static struct cote* inserer_queue(struct cote *p, struct cote* q)
{
    struct cote* Q1 = NULL;
    struct cote* Q2 = NULL;
    struct cote* tmp = NULL;
    tmp = p;
    while (tmp->suivant != NULL) {
        tmp = tmp->suivant;
    }
    Q1 = tmp;
    if ( q == NULL)
    {
        return p;
    }else{
        tmp = q;
        while (tmp->suivant != NULL) {
            tmp = tmp->suivant;
        }
        Q2 = tmp;
        Q1->suivant = q;
        Q1 = Q2;
        return p;
    }

}

static struct cote* supprime (struct cote* TeCA, int y)
{
    struct cote *tmp;
    struct cote *preced = TeCA;
    if (preced->ymax == y) {
        TeCA = preced->suivant;
    }
    tmp = preced->suivant;
    while (tmp != NULL) {
        if (tmp->ymax == y) {
            preced->suivant = tmp->suivant;
        }
        preced = tmp;
        tmp = tmp->suivant;
    }
    return(TeCA);
}


static struct cote* trie(struct cote* TeCA) {
    if (TeCA == NULL) {
        return TeCA;
    } else {
        struct cote *premier = TeCA;
        int recommencer;
        do {
            // commence au début de la liste
            struct cote *precedent = NULL;
            struct cote *element = premier;
            struct cote *suivant = element->suivant;
            recommencer = 0;

            while (suivant != NULL) {
                if (suivant->xymin < element->xymin) {
                    // si le classement de l'élément et de son suivant est incorrect :

                    // la liste devra être re-parcourue
                    recommencer = 1;

                    // inverse l'élément courant et son suivant
                    if (precedent == NULL)
                        premier = suivant;
                    else
                        precedent->suivant = suivant;
                    element->suivant = suivant->suivant;
                    suivant->suivant = element;

                    // avance dans la liste
                    precedent = suivant;        // nouveau précédent = ancien suivant
                    // nouvel élément = ancien élément (inchangé)
                    suivant = element->suivant; // nouveau suivant = suivant du nouvel élément

                } else {
                    // si le classement de l'élément et de son suivant est correct :

                    // avance dans la liste
                    precedent = element;        // nouveau précédent = ancien élément
                    element = element->suivant; // nouvel élément = ancien suivant
                    suivant = element->suivant; // nouveau suivant = suivant du nouvel élément
                }
            }
        } while (recommencer);
        return premier;
    }
}

static int get_taille(struct cote* liste)
{
    int taille = 0;
    struct cote* cel = liste;
    while (cel != NULL)
    {
        cel = cel->suivant;
        taille += 1;
    }
    return taille;
}

static int get_min(const ei_linked_point_t* point)
{
    ei_linked_point_t* cel = point;
    int min = point->point.y;
    while (cel != NULL){
        if ( cel->point.y < min){
            min = cel->point.y;
        }
        cel = cel->next;
    }
    return min;
}

void	ei_draw_polygon	(ei_surface_t			surface,
                             const ei_linked_point_t*	first_point,
                             ei_color_t			color,
                             const ei_rect_t*		clipper)
{
    int X, Y, XX, YY;
    if (clipper != NULL) {
        Y = clipper->top_left.y;
        X = clipper->top_left.x;
        XX = X + clipper->size.width;
        YY = Y + clipper->size.height;
    }else{
        X = 0;
        Y = 0;
        XX = hw_surface_get_size(surface).width;
        YY = hw_surface_get_size(surface).height;
    }
    int hauteur = hw_surface_get_size(surface).height;
    int largeur = hw_surface_get_size(surface).width;
    //first_point = premier_point(first_point);
    struct cote** TeC = malloc(hauteur*sizeof(struct cote*));
    for (uint32_t i = 0; i < hauteur; i++){
        TeC[i] = NULL;
    }
    ei_point_t p1 = first_point->point;
    int y = get_min(first_point);
    uint32_t* first_pixel;
    first_pixel = (uint32_t *) hw_surface_get_buffer(surface) + largeur * (y);
    remplir(first_point, TeC);
    struct cote* TeCA = NULL;
    while( y < hauteur){
        if (TeCA == NULL){
            TeCA = TeC[y];
        }else{
            TeCA = inserer_queue(TeCA, TeC[y]);
        }
        TeC[y] = NULL;
        int taille = get_taille(TeCA);
        for (uint32_t i=0; i<taille; i++)
        {
            TeCA = supprime(TeCA, y);
        }
        TeCA = trie(TeCA);
        struct cote *prem = TeCA;
        if ( y >= Y && y <= YY) {
            while (prem != NULL) {
                float x = prem->xymin;
                float xmax = prem->suivant->xymin;
                first_pixel += (int) x;
                while (x <= xmax) {
                    if ( x >= (float)X && x <= (float)XX ) {
                        *first_pixel = (color.red << 24) + (color.green << 16) + (color.blue << 8) + color.alpha;
                    }
                    first_pixel++;
                    x += 1;
                }
                prem = prem->suivant->suivant;
            }
        }
        y += 1;
        first_pixel = (uint32_t*)hw_surface_get_buffer(surface) + largeur*(y);
        struct cote *maj = TeCA;
        while (maj != NULL) {
            float dx = maj->deltax;
            int dy = maj->deltay;
            maj->xymin += dx/(float)dy;
            maj = maj->suivant;
        }
    }
    free(TeCA);
    for (uint32_t i = 0; i < hauteur; i++){
        free(TeC[i]);
    }
    free(TeC);
}



int			ei_copy_surface		(ei_surface_t		destination,
                                       const ei_rect_t*	dst_rect,
                                       ei_surface_t		source,
                                       const ei_rect_t*	src_rect,
                                       ei_bool_t		alpha)
{
    int largeur = hw_surface_get_size(source).width;
    int hauteur = hw_surface_get_size(source).height;
    uint32_t *first_pix_src = (uint32_t*) hw_surface_get_buffer(source);
    uint32_t *first_pix_dest = (uint32_t*) hw_surface_get_buffer(destination);
    if (dst_rect != NULL) {
        first_pix_dest += (dst_rect->top_left.y - 1) * (hw_surface_get_size(destination).width)+ dst_rect->top_left.x;
        if (src_rect != NULL) {
            if (dst_rect->size.width != src_rect->size.width && dst_rect->size.height != src_rect->size.height) {
                return 1;
            }
            first_pix_src += (src_rect->top_left.y - 1) * (hw_surface_get_size(source).width) + src_rect->top_left.x;
            largeur = src_rect->size.width;
            hauteur = src_rect->size.height;
        } else {
            if (largeur != dst_rect->size.width && hauteur != dst_rect->size.width) {
                return 1;
            }
        }
    }else{
        if(src_rect!= NULL){
            if(hw_surface_get_size(destination).width != src_rect->size.width &&
               hw_surface_get_size(destination).height != src_rect->size.height){
                return 1;
            }
            first_pix_src += (src_rect->top_left.y-1)*(hw_surface_get_size(source).width)+(src_rect->top_left.x);
            largeur = src_rect->size.width;
            hauteur = src_rect->size.height;
        }else{
            if(largeur != hw_surface_get_size(destination).width || hauteur != hw_surface_get_size(destination).height){
                return 1;
            }
        }
    }

    // On recopie le plus petit rectangle autorisé après clipping de taille largeur*hauteur
    int ir_src, ig_src, ib_src, ia_src;
    int ir_dst, ig_dst, ib_dst, ia_dst;
    hw_surface_get_channel_indices(source,&ir_src,&ig_src,&ib_src,&ia_src);
    hw_surface_get_channel_indices(destination,&ir_dst,&ig_dst,&ib_dst,&ia_dst);
    uint32_t i;
    uint32_t j;
    for(i=0; i < hauteur; i++){
        for (j=0; j< largeur; j++){
            if( !alpha ){
                //if (*first_pix_src != (color.red<<8)+(color.green<<8)+(color.blue<<8)){
                *first_pix_dest = *first_pix_src;
                //}
            }else{
                uint32_t *pix_src = first_pix_src;
                uint8_t *ptr_src = pix_src;
                uint8_t P_R = ptr_src[ir_src];
                uint8_t P_G = ptr_src[ig_src];
                uint8_t P_B = ptr_src[ib_src];
                uint8_t P_A = ptr_src[ia_src];

                uint32_t *pix_dst = first_pix_dest;
                uint8_t *ptr_dst = pix_dst;
                uint8_t S_R = ptr_dst[ir_dst];
                uint8_t S_G = ptr_dst[ig_dst];
                uint8_t S_B = ptr_dst[ib_dst];
                //uint8_t S_A = ptr_dst[ia_dst];

                ptr_dst[ir_dst] = (P_A+P_R+(255-P_A)*S_R)/255;
                ptr_dst[ig_dst] = (P_A+P_G+(255-P_A)*S_G)/255;
                ptr_dst[ib_dst] = (P_A+P_B+(255-P_A)*S_B)/255;
                *first_pix_dest = *(pix_dst);

            }
            first_pix_src ++;
            first_pix_dest ++;
        }
        first_pix_dest += hw_surface_get_size(destination).width - largeur;
        first_pix_src += hw_surface_get_size(source).width - largeur;
    }
    return 0;
}


void			ei_draw_text		(ei_surface_t		surface,
                                     const ei_point_t*	where,
                                     const char*		text,
                                     ei_font_t		font,
                                     ei_color_t		color,
                                     const ei_rect_t*	clipper) {
    ei_surface_t source = hw_text_create_surface(text, font, color);
    hw_surface_lock(source);
    int largeur = hw_surface_get_size(source).width;
    int hauteur = hw_surface_get_size(source).height;
    ei_rect_t rect;
    if (clipper == NULL){
        rect = ei_rect(*where, ei_size(largeur, hauteur));
        ei_copy_surface(surface, &rect, source, NULL, EI_FALSE);
    }else{
        ei_point_t B = ei_point(clipper->top_left.x+clipper->size.width, clipper->top_left.y+clipper->size.height);
        ei_point_t V = ei_point(where->x+largeur, where->y +hauteur);
        int l;
        int h;
        if(where->x > clipper->top_left.x && where->y > clipper->top_left.y){
            if (B.x>V.x){l = largeur;} else{l = B.x - where->x;}
            if (B.y>V.y){h = hauteur;} else{h = B.y - where->y;}
            rect = ei_rect(*where, ei_size(l,h));
        }
        else if(where->x < clipper->top_left.x && where->y < clipper->top_left.y){
            if (B.x>V.x){l = largeur;} else{l = V.x - clipper->top_left.x;}
            if (B.y>V.y){h = hauteur;} else{h = V.y - clipper->top_left.y;}
            rect = ei_rect(clipper->top_left, ei_size(l,h));
        }
        else if(where->x >= clipper->top_left.x && where->y <= clipper->top_left.y){
            ei_point_t E = ei_point(where->x, clipper->top_left.y);
            if(B.x>V.x){l = largeur;} else{l = B.x - E.x;}
            if (B.y>V.y){h = V.y-E.y;} else{h = B.y-E.y;}
            rect = ei_rect(E, ei_size(l,h));
        }
        else{
            ei_point_t F = ei_point(clipper->top_left.x ,where->y);
            if(B.x>V.x){l = V.x-F.x;} else{l = B.x - F.x;}
            if (B.y>V.y){h = hauteur;} else{h = B.y-F.y;}
            rect = ei_rect(F, ei_size(l,h));
        }
        int x = rect.top_left.x - where->x;
        int y = rect.top_left.y - where->y;
        ei_rect_t src_rct = ei_rect(ei_point(x,y), ei_size(l,h));
        ei_copy_surface(surface, &rect, source, &src_rct, EI_TRUE);
    }
    //hw_surface_free(source);
}
