//
//
// C++ Interface for module: GUI
//
// Description: 
//
//
// Author: exa
//
//

#ifndef GUI_Interface
#define GUI_Interface

#include "General.hxx"
#include <gtk--.h>
#include <gdk--.h>
#include <gtk/gtk.h>
#include <iostream>

class Drawing_Area  : public Gtk_DrawingArea
{
public:

  Drawing_Area ();

  void clear();
  
  void update_all();

  // Assume 16 bit true color display throughout

  void draw_point(int x, int y, double r,
		  double g, double b) {
    unsigned int pixel = (int(r * 63) << 10 ) |
                         (int(g * 63) << 5 ) |
                         (int(b * 63) );
    image.put_pixel(x,y,pixel);
  }

  void draw_gray_point(int x, int y, float _i) {
//      int i = int(_i * 0x3F );
//      unsigned int pixel = (i << 10 ) |
//                           (i << 5 ) |
//                           (i );
    char i = int(_i * 0x3F );
    char i_2 = i >> 1;
    //unsigned int pixel = i << 5;
      unsigned int pixel =
	(i_2 << 11 ) |
	(i << 5 ) |
	(i_2 );
    gdk_image_put_pixel(gdk_image, x,y,pixel);
    //image.put_pixel(x,y,pixel);
  }

private:

  /* Backing pixmap for drawing area */

  //Gdk_Pixmap pixmap;
  Gdk_Image image;
  GdkImage *gdk_image;
  Gdk_GC gc;
  //Gdk_GC gc2;
  Gdk_Window win;
  Gdk_Visual visual;
  Gdk_Colormap colormap;
  Gdk_Color bg_color, fg_color;
  GdkRectangle clip_rect;

  /* Create a new backing pixmap of the appropriate size */
  int configure_event_impl (GdkEventConfigure * /* event */);

  /* Redraw the screen from the backing pixmap */
  int expose_event_impl (GdkEventExpose *event);

//    Gdk_GC get_GC() {
//      if(exas_gc==0)
//        exas_gc=gdk_gc_new(GTK_WIDGET(gtkobj())->window);
//      return Gdk_GC(exas_gc);
//    }


  /* Draw a rectangle on the screen */
  void draw_brush (gdouble x, gdouble y);

  gint button_press_event_impl (GdkEventButton *event);

  gint motion_notify_event_impl (GdkEventMotion *event);


};


class Volume_Visualizer;

class Window : public Gtk_Window
{
public:  
  
  Window(Volume_Visualizer *v);

  Volume_Visualizer *visualizer;
  //Drawing_Area drawing_area;

private:
  Gtk_VBox vbox;
  Gtk_Button button;

  void quit() { gtk_exit (0); }
};


void GUI_test(int argc, char *argv[]);

#endif
