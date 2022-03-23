#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include <memory>

class ExampleWindow : public Gtk::Window
{
public:
  ExampleWindow();
  virtual ~ExampleWindow();

protected:
  //Signal handlers:
  void on_button_info_clicked();
  void on_button_question_clicked();
  void on_question_dialog_response(int response_id);

  //Child widgets:
  Gtk::Box m_ButtonBox;
  Gtk::Button m_Button_Info, m_Button_Question;

  std::unique_ptr<Gtk::MessageDialog> m_pDialog;
};

#endif //GTKMM_EXAMPLEWINDOW_H