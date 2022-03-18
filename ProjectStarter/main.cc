#include "mainwindow.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
  
  auto App = Gtk::Application::create("com.emanuelesantanche.ProjectStarter");

  // Shows the window and returns when it is closed.
  return App->make_window_and_run<MainWindow>(argc, argv);
}


// you use these prefixes in alphabetic order


//         c for constants/readonlys
//         e for events
//         i for indexes and iterators (it can be used as well for variables that represent the current item in a loop)
//         m for members
//         n for collections (arrays and vectors)
//         p for pointer (and pp for pointer to pointer)
//         s for static
//         u for unsafe
//         v for volatile

//         for variables, functions and classes use CamelCase so that you differentiate from standard variables (but GTKMM does the same)
        
//         So
        
//         mTreeView 
//         mpTreeView