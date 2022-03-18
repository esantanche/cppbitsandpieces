#ifndef GTKMM_EXAMPLE_BUTTONS_H
#define GTKMM_EXAMPLE_BUTTONS_H

class Controller;

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include "controller.h"

class Buttons : public Gtk::Window
{
public:
  Buttons(Controller * controller);
  //Buttons();
  virtual ~Buttons();

protected:
  //Signal handlers:
  void on_button_clicked();

  //Child widgets:
  Gtk::Button m_button;
};

#endif //GTKMM_EXAMPLE_BUTTONS_H
