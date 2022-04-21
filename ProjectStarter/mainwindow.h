#ifndef GTKMM_PRJSTR_MAINWINDOW_H
#define GTKMM_PRJSTR_MAINWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/textview.h>
#include <gtkmm/label.h>

// #include <gdk/gdk.h>

#include <string>

#include "configurationparser.h"

class MainWindow : public Gtk::Window
{
public:
  MainWindow();
  virtual ~MainWindow();

protected:
  // Boxes to contain elements:
  Gtk::Box mVBox; // Contains all elements in a vertical arrangement
  Gtk::Box mButtonBox; // Contains the buttons
  
  Gtk::Button mButtonRun;  // Button 'RUN' that starts the project
  Gtk::Button mButtonCancel;  // Button cancel to exit without running anything
  Gtk::TextView mTextView;
  Gtk::Label mLabelMessageJsonFileError; 
  
  bool mJsonConfigurationOk;


  // Tree model columns. These are the columns (1) that you find in the list
  // It's the column with the project names
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(mColProjectName); }

    Gtk::TreeModelColumn<Glib::ustring> mColProjectName;

    // bool onKeyPress(GdkEventKey*);
    
  };


  Gtk::ScrolledWindow mScrolledWindow;
  Gtk::TreeView mTreeView;
  Glib::RefPtr<Gtk::ListStore> mpTreeModel;

  ModelColumns mColumns;

  ConfigurationParser mConfigurationParser;


  void get_selected_project_and_run_its_tasks();
  void on_button_run_clicked();
  void on_button_cancel_clicked();
  void on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);

};

#endif //GTKMM_PRJSTR_MAINWINDOW_H
