/**
 * @file main.cc
 * @author Emanuele Santanche (http://emanuelesantanche.com/)
 * @brief Just the main of the app. It runs the main window
 */

#include "mainwindow.h"
#include <gtkmm/application.h>

/**
 * @brief The main
 * 
 * @param argc Not used
 * @param argv Not used
 * @return int Exit code
 */
int main(int argc, char *argv[])
{
  
  auto App = Gtk::Application::create("com.emanuelesantanche.ProjectStarter");

  // Shows the window and returns when it is closed.
  return App->make_window_and_run<MainWindow>(argc, argv);
}
