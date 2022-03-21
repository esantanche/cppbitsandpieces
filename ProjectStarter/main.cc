#include "mainwindow.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
  
  auto App = Gtk::Application::create("com.emanuelesantanche.ProjectStarter");

  // Shows the window and returns when it is closed.
  return App->make_window_and_run<MainWindow>(argc, argv);
}

