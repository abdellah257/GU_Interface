#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
    ei_size_t	screen_size		= {600, 600};
    ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

    ei_widget_t*	frame;
    ei_size_t	frame_size		= {150,100};
    int		frame_x			= 150;
    int		frame_y			= 200;
    ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
    ei_relief_t	frame_relief		= ei_relief_raised;
    int		frame_border_width	= 6;
    char* text = "Hello button today it's ur day";
    ei_color_t      color_text  =  {0, 0, 0, 255};

    ei_widget_t*	button;
    ei_size_t	button_size		= {300,200};
    int		button_corner_radius	= 0;
    int		button_x		= 0;
    int		button_y		= 0;
    ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
    char*		button_title		= "Mon premier Bouton!";
    ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
    ei_relief_t	button_relief		= ei_relief_raised;
    int		button_border_width	= 6;
    //ei_callback_t	button_callback 	= button_press;

    ei_widget_t*	button1;
    ei_size_t	button_size1	= {300,200};
    int		button_corner_radius1	= 0;
    int		button_x1		= 250;
    int		button_y1		= 350;
    ei_color_t	button_color1		= {0x30, 0x30, 0x30, 0xff};
    char*		button_title1		= "Mon premier Bouton!";
    ei_color_t	button_text_color1	= {0x00, 0x00, 0x00, 0xff};
    ei_relief_t	button_relief1		= ei_relief_raised;
    int		button_border_width1	= 6;
    //ei_callback_t	button_callback 	= button_press;

    /* Create the application and change the color of the background. */
    ei_app_create(screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Create, configure and place the frame on screen. */
    frame = ei_widget_create("frame_class", ei_app_root_widget(), NULL, NULL);
    //ei_frame_configure(frame, NULL, &frame_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_frame_configure	(frame, &frame_size, &frame_color,
                           &frame_border_width, &frame_relief, &text, NULL, &color_text, NULL,
                           NULL, NULL, NULL);
    ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );


    /* Create, configure and place the button on screen. */
    button = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure	(button, &button_size, &button_color,
                            &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
                            NULL, NULL, NULL, NULL, NULL);
    ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    button1 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure	(button1, &button_size1, &button_color1,
                            &button_border_width1, &button_corner_radius1, &button_relief1, &button_title1, NULL, &button_text_color1, NULL,
                            NULL, NULL, NULL, NULL, NULL);
    ei_place(button1, NULL, &button_x1, &button_y1, NULL, NULL, NULL, NULL, NULL, NULL );

    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}
