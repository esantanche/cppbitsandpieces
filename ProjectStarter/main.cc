#include "mainwindow.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
  // FIXME name org.gtkmm.example to fix
  auto App = Gtk::Application::create("org.gtkmm.example");

  // Shows the window and returns when it is closed.
  return App->make_window_and_run<MainWindow>(argc, argv);
}


// you use these prefixes in alphabetic order


//         c for constants/readonlys
//         e for events
//         i for indexes and iterators
//         m for members
//         n for collections (arrays and vectors)
//         p for pointer (and pp for pointer to pointer)
//         s for static
//         v for volatile

//         for variables, functions and classes use CamelCase so that you differentiate from standard variables (but GTKMM does the same)
        
//         So
        
//         mTreeView 
//         mpTreeView