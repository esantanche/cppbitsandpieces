#include "examplewindow.h"
#include <gtkmm/messagedialog.h>
#include <iostream>

ExampleWindow::ExampleWindow()
: m_ButtonBox(Gtk::Orientation::VERTICAL),
  m_Button_Info("Show Info MessageDialog"),
  m_Button_Question("Show Question MessageDialog")
{
  set_title("Gtk::MessageDialog example");

  set_child(m_ButtonBox);

  m_ButtonBox.append(m_Button_Info);
  m_Button_Info.set_expand(true);
  m_Button_Info.signal_clicked().connect(sigc::mem_fun(*this,
              &ExampleWindow::on_button_info_clicked) );

  m_ButtonBox.append(m_Button_Question);
  m_Button_Question.set_expand(true);
  m_Button_Question.signal_clicked().connect(sigc::mem_fun(*this,
              &ExampleWindow::on_button_question_clicked) );
}

ExampleWindow::~ExampleWindow()
{
}

void ExampleWindow::on_button_info_clicked()
{
  m_pDialog.reset(new Gtk::MessageDialog(*this, "This is an INFO MessageDialog"));
  m_pDialog->set_secondary_text(
          "And this is the secondary text that explains things.");
  m_pDialog->set_modal(true);
  m_pDialog->set_hide_on_close(true);
  m_pDialog->signal_response().connect(
    sigc::hide(sigc::mem_fun(*m_pDialog, &Gtk::Widget::hide)));

  m_pDialog->show();
}

void ExampleWindow::on_button_question_clicked()
{
  m_pDialog.reset(new Gtk::MessageDialog(*this, "This is a QUESTION MessageDialog",
          false /* use_markup */, Gtk::MessageType::QUESTION,
          Gtk::ButtonsType::OK_CANCEL, true /* modal */));
  m_pDialog->set_secondary_text(
          "And this is the secondary text that explains things.");
  m_pDialog->set_hide_on_close(true);
  m_pDialog->signal_response().connect(
    sigc::mem_fun(*this, &ExampleWindow::on_question_dialog_response));

  m_pDialog->show();
}

void ExampleWindow::on_question_dialog_response(int response_id)
{
  m_pDialog->hide();

  //Handle the response:
  switch (response_id)
  {
    case Gtk::ResponseType::OK:
    {
      std::cout << "OK clicked." << std::endl;
      break;
    }
    case Gtk::ResponseType::CANCEL:
    {
      std::cout << "Cancel clicked." << std::endl;
      break;
    }
    default:
    {
      std::cout << "Unexpected button clicked." << std::endl;
      break;
    }
  }
}

