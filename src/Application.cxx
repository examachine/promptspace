//
//
// C++ Implementation file for application
//
// Description: includes the main(...) function
//
// exa
//
//

#include "General.hxx"
#include "Numerical_Vector.hxx"
#include "Matrix.hxx"
#include "TMatrix.hxx"
#include "GUI.hxx"
#include "Volume_Visualizer.hxx"
#include "Slice_Visualizer.hxx"

Window *window;

gint animation_callback( gpointer data )
{
  try {
    if (window->visualizer->fast) {
      window->visualizer->cam.parent.rotate(Angles(0,M_PI/128, 0));
      if (window->visualizer->cam.parent.sel_orn().pitch >= M_PI/8)
	return FALSE;
    }
    else {
      window->visualizer->cam.parent.rotate(Angles(0,-M_PI/32, 0));
    }
    window->visualizer->update_view();
    //return FALSE; // don't animate
    return TRUE;
  }
  catch(exception e) {
    cerr << e.what() << endl;
    return FALSE;
  }
}

int main(int argc, char *argv[])
{
  // tests

  //Numerical_Vector_test();
  //Matrix_test();
  //GUI_test(argc, argv);

  // real application, give control to GUI
  
  try {

    Gtk_Main app(&argc, &argv);
    
    bool fast;
    if (argc == 4)
      fast = false;
    else
      fast = true;

    nlog << "Rendering Shear/Warp? " << fast << endl;

    int image_ext;
    if (argc >= 3)
      image_ext = atoi(argv[2]);
    else
      image_ext = 120;

    Screen screen(1, 1, image_ext);

    string filename;
    if (argc >= 2)
      filename = argv[1];
    else
      filename = "data/brainsmall.rv";

    nlog << "Filename " << filename << endl;

    Model model(screen, filename);
    //Model model(screen, 64);

    //Volume_Visualizer visualizer(model, fast);
    Slice_Visualizer visualizer(model, fast);
    
    //visualizer.make_test_volume();

    window = new Window(&visualizer);
    window->show();

    //visualizer.cam.parent.rotate(Angles(0,-M_PI/7, 0));

    if (fast)
      gtk_idle_add(animation_callback, NULL);
    else {
      gtk_idle_add(animation_callback, NULL);
      window->visualizer->cam.parent.rotate(Angles(0, 0, M_PI*31/16));
      //window->visualizer->cam.parent.rotate(Angles(0,M_PI*32/32, 0));
    }
    //window->visualizer->update_view();
    // Timer

    app.run();
  }
  catch(exception e) {
    cerr << e.what() << endl;
  }

  return 0;
}
