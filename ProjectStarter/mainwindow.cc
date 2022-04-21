/**
 * @file mainwindow.cc
 * @author Emanuele Santanche (http://emanuelesantanche.com/)
 * @brief This is the main (and only) window 
 */

#include "mainwindow.h"

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>
#include <cstdio>

#include <gtkmm/messagedialog.h>

#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/filereadstream.h"

/**
 * @brief Construct a new Main Window:: Main Window object
 */
MainWindow::MainWindow() : mVBox(Gtk::Orientation::VERTICAL), mButtonCancel("Cancel"),
      mButtonRun("Run")
{

  mJsonConfigurationOk = mConfigurationParser.json_configuration_is_correct();
 
  //cout << "json conf correct: " << JsonConfigurationOk  << endl;

  set_default_size(400, 400);
  set_title("Project starter");
  
  // Setting the margin of the vertical box that will contain all elements
  mVBox.set_margin(5);
  set_child(mVBox);

  if (mJsonConfigurationOk) {

    // Putting the TreeView inside the scrolled window and setting its height
    mScrolledWindow.set_child(mTreeView);
    mScrolledWindow.set_min_content_height(400);
    mVBox.append(mScrolledWindow);

  } else {

    mVBox.append(mLabelMessageJsonFileError);
    mLabelMessageJsonFileError.set_text("The JSON configuration file is wrong.\nProbably there is a comma missing or a similar problem.\n");
  
  }
  
 
  // In the vertical box the scrolled window that contains the TreeView will be above.
  // The box that contains the buttons will be below
 
  mVBox.append(mButtonBox);

  if (mJsonConfigurationOk) {

    // When the cancel button is clicked, the function on_button_cancel_clicked is called
    mButtonCancel.signal_clicked().connect( sigc::mem_fun(*this,
                &MainWindow::on_button_cancel_clicked) );
    
    mButtonBox.append(mButtonCancel);
    mButtonBox.set_margin(20);
    // This set_hexpand is needed for the buttons to align on the right
    mButtonCancel.set_hexpand(true);
    mButtonCancel.set_margin_end(5); // Space between the buttons
    mButtonCancel.set_halign(Gtk::Align::END);

    // Now creating the list with the single column
    // Getting a ptr to a TreeModel
    mpTreeModel = Gtk::ListStore::create(mColumns);

    // Now setting the model above to be used in the TreeView
    mTreeView.set_model(mpTreeModel);

    // Now creating the rows in the list

    const vector<string> nProjectNames = mConfigurationParser.get_project_names();

    for (string iProjectName : nProjectNames) {
      
      auto Row = *(mpTreeModel->append());
      Row[mColumns.mColProjectName] = iProjectName;
    
    }
    
    // Actually putting the column in the view
    mTreeView.append_column("Project Name", mColumns.mColProjectName);

    // Connecting double click on list to callback on_treeview_row_activated
    mTreeView.signal_row_activated().connect( sigc::mem_fun(*this,
                &MainWindow::on_treeview_row_activated) );

  } 

  // When the run button is clicked, the function on_button_run_clicked is called
  mButtonRun.signal_clicked().connect( sigc::mem_fun(*this,
              &MainWindow::on_button_run_clicked) );
  
  mButtonRun.set_halign(Gtk::Align::END);

  if (!mJsonConfigurationOk) {
    mButtonRun.set_hexpand(true);
    mButtonRun.set_label("OK");
  }

  mButtonBox.append(mButtonRun);

}

MainWindow::~MainWindow()
{
}

/**
 * @brief When the cancel button is clicked, just close the window
 */
void MainWindow::on_button_cancel_clicked()
{
  
  close(); // Closing the window
}

/**
 * @brief If there is a double click on a row, get the selected project and run its tasks
 * 
 * @param path Not used
 * @param column Not used
 */
void MainWindow::on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{

  get_selected_project_and_run_its_tasks();

}

/**
 * @brief  If che button run is clicked, get the selected project and run its tasks
 */
void MainWindow::on_button_run_clicked()
{

  if (mJsonConfigurationOk) 
    get_selected_project_and_run_its_tasks();
  else
    close();

}

/**
 * @brief Get the selected project and run its tasks
 */
void MainWindow::get_selected_project_and_run_its_tasks()
{

  Glib::RefPtr<Gtk::TreeSelection> pTreeSelection = mTreeView.get_selection();
  Glib::RefPtr<Gtk::TreeModel> pModel = mTreeView.get_model();

  Gtk::TreeModel::iterator iter = pTreeSelection->get_selected(pModel);

  // I can just do this to get the selected row because only one row can be selected
  Gtk::TreeModel::Row Row = *iter;

  const bool cSuccess = mConfigurationParser.run_tasks_for_a_project(Row[mColumns.mColProjectName]);

  // The given name of project (Row[mColumns.mColProjectName]) should always be found
  assert(cSuccess == true);
  
  close(); // Closing the window
}
