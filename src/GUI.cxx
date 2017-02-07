//
//
// C++ Implementation for module: GUI
//
// Description: 
//
//
// Author: exa
//
//

#include "GUI.hxx"
#include "Volume_Visualizer.hxx"

Drawing_Area::Drawing_Area ()
  //: pixmap (0), exas_gc(NULL)
{
  set_events (GDK_EXPOSURE_MASK
	      | GDK_LEAVE_NOTIFY_MASK
	      | GDK_BUTTON_PRESS_MASK
	      | GDK_POINTER_MOTION_MASK
	      | GDK_POINTER_MOTION_HINT_MASK);
    
}

//  Gtk_State *get_state()
//  {
//    return [GTK_WIDGET_STATE (GTK_WIDGET(gtkobj()))];
//  }

/* Create a new backing pixmap of the appropriate size */
int Drawing_Area::configure_event_impl (GdkEventConfigure * /* event */)
{
  win = get_window();
  visual = win.get_visual();
  // Only run under 16 bits true color display!!
  I(visual.gdkobj()->depth == 16);

  colormap = Gdk_Colormap::get_system();

//    if (pixmap)
//      pixmap.free();

  bg_color.set_rgb(0,40000,0);
  fg_color.set_rgb(60000,0,0);

  clip_rect.x = 0;
  clip_rect.y = 0;
  clip_rect.width = width();
  clip_rect.height = height();

  gc = get_style()->gtkobj()->black_gc;
  // gc2=gdk_gc_new(GTK_WIDGET(gtkobj())->window);

  // Gtk_Style has no access to its data members, so use gtk objekt.
  // Some access functions would be nice like GtkStyle::get_white_gc() etc.
  //pixmap.create(win, width(), height());
  image.create(GDK_IMAGE_FASTEST, visual, width(), height());
  gdk_image = image.gdkobj();

  clear();

  return TRUE;
}

void Drawing_Area::clear()
{
  //gc = get_style()->gtkobj()->white_gc;
//    pixmap.draw_rectangle (gc,
//  			 TRUE,
//  			 0, 0,
//  			 width(),
//  			 height());
  for (int x = 0; x <width(); x ++)
    for (int y = 0; y <height(); y ++)
      //image.put_pixel(x,y,0x0002f);
      image.put_pixel(x,y,0x00000);
}

void Drawing_Area::update_all()
{
  draw(&clip_rect);
}

/* Redraw the screen from the backing image */
int Drawing_Area::expose_event_impl (GdkEventExpose *event)
{
    
  //gc = get_style()->gtkobj()->fg_gc[GTK_WIDGET_STATE (GTK_WIDGET(gtkobj()))];
  // Same like above, + Gtk_Widget has set_state function but no get_state 
  // function. 

  //gc2 = get_GC();
//    win.draw_pixmap(gc,
//  		  pixmap,
//  		  event->area.x, event->area.y,
//  		  event->area.x, event->area.y,
//  		  event->area.width, event->area.height);
  win.draw_image(gc,
		 image,
		 event->area.x, event->area.y,
		 event->area.x, event->area.y,
		 event->area.width, event->area.height);

  return FALSE;
}


/* Draw a rectangle on the screen */
void Drawing_Area::draw_brush (gdouble x, gdouble y)
{
  GdkRectangle update_rect;
  update_rect.x = (int)x - 5;
  update_rect.y = (int)y - 5;
  update_rect.width = 10;
  update_rect.height = 10;
  gc = get_style()->gtkobj()->black_gc;
  
//    pixmap.draw_rectangle(gc,
//  			TRUE,
//  			update_rect.x, update_rect.y,
//  			update_rect.width, update_rect.height);
  draw (&update_rect);
}

//  void Drawing_Area::draw_point (gdouble x, gdouble y,
//  			       gdouble r, gdouble g, gdouble b)
//{
//    Gdk_Color color;
//    color.set_rgb_p(r, g, b);
//    colormap.alloc(color);
//    gc.set_foreground(color);
//    pixmap.draw_point(gc, x, y);
//    unsigned int pixel = (int(r * 63) << 10 ) |
//                         (int(g * 63) << 5 ) |
//                         (int(b * 63) );
//    image.put_pixel(x,y,pixel);
//  }

gint Drawing_Area::button_press_event_impl (GdkEventButton *event)
{
  if (event->button == 1 && image)
    //draw_brush (event->x, event->y);
    ;
  
  return TRUE;
}

gint Drawing_Area::motion_notify_event_impl (GdkEventMotion *event)
{
  int x, y;
  GdkModifierType state;
  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = (int)event->x;
      y = (int)event->y;
      state = (GdkModifierType) event->state;
    }
    
  if (state & GDK_BUTTON1_MASK && image)
    ;//draw_brush (x, y);
  
  return TRUE;
}

// Window class

Window::Window(Volume_Visualizer *v)
  :  Gtk_Window(GTK_WINDOW_TOPLEVEL),
     vbox (FALSE, 0),
     button ("quit"),
     visualizer(v)
{
  add (&vbox);

  /* Create the drawing area */
  //visualizer->drawing_area.size (400, 400);
  vbox.pack_start(v->drawing_area, TRUE, TRUE, 0);

  /* Add the button */
  vbox.pack_start(button, FALSE, FALSE, 0);

  connect_to_method(button.clicked, this, &Window::quit);
  connect_to_method(destroy, this, &Window::quit);

  visualizer->drawing_area.show();
  button.show();
  vbox.show();
}


void GUI_test(int argc, char *argv[])
{
  Window *window;
  Gtk_Main app(&argc, &argv);

//    window = new Window;
//    window->show();


  app.run();

}
