#include "buttons.h"
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>

Buttons::Buttons(Controller * controller)
{
  // This corresponds to Gtk::Bin::add_pixlabel("info.xpm", "cool button") in gtkmm3.
  //Create Image and Label widgets:
  auto pmap = Gtk::make_managed<Gtk::Image>("info.xpm");
  auto label = Gtk::make_managed<Gtk::Label>("cool juuuu77 button");
  label->set_expand(true);

  //Put them in a Box:
  auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 5);
  hbox->append(*pmap);
  hbox->append(*label);

  std::cout << "I am in Buttons constructor" << std::endl;

  //And put that Box in the button:
  m_button.set_child(*hbox);

  set_title("This is the title!!!");

  m_button.signal_clicked().connect( sigc::mem_fun(*this,
              &Buttons::on_button_clicked) );

  m_button.set_margin(10);
  set_child(m_button);
}

Buttons::~Buttons()
{
}

void Buttons::on_button_clicked()
{
  std::cout << "The Button was clicked 78hytt6766." << std::endl;
  
  // FIXME here I should call the controller
  
}
