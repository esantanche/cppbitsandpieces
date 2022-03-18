#include "mainwindow.h"
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>

#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"

#include "include/rapidjson/filereadstream.h"
#include <cstdio>
 

// FIXME what's this? auto builder = Gtk::Builder::create_from_file("test.glade");

MainWindow::MainWindow() : mVBox(Gtk::Orientation::VERTICAL), mButtonCancel("Cancel"),
      mButtonRun("Run")
{
 
  FILE* pJsonFile = fopen("big.json", "rb"); // non-Windows use "r"
  
  char ReadBuffer[65536];

  rapidjson::FileReadStream InputStream(pJsonFile, ReadBuffer, sizeof(ReadBuffer));
  
  rapidjson::Document JsonDocument;
  JsonDocument.ParseStream(InputStream);

  const rapidjson::Value& cnProjects = JsonDocument;

  assert(cnProjects.IsArray());
  // Uses SizeType instead of size_t
  for (rapidjson::SizeType i = 0; i < cnProjects.Size(); i++) {
    const rapidjson::Value& current_project = JsonDocument[i];
    //std::cout << i << "---" << current_project["project_name"].GetString()  << std::endl;
    mnProjectNames.push_back(current_project["project_name"].GetString());
  }
      
  
  fclose(pJsonFile);

  set_default_size(400, 200);
  set_title("Project starter");
  
  // Setting the margin of the vertical box that will contain all elements
  mVBox.set_margin(5);
  set_child(mVBox);
  
  // Putting the TreeView inside the scrolled window and setting its height
  mScrolledWindow.set_child(mTreeView);
  mScrolledWindow.set_min_content_height(200);
  
  // In the vertical box the scrolled window that contains the TreeView will be above.
  // The box that contains the buttons will be below
  mVBox.append(mScrolledWindow);
  mVBox.append(mButtonBox);

  // When the cancel button is clicked, the function on_button_cancel_clicked is called
  mButtonCancel.signal_clicked().connect( sigc::mem_fun(*this,
              &MainWindow::on_button_cancel_clicked) );
  
  mButtonBox.append(mButtonCancel);
  mButtonBox.set_margin(20);
  
  // This set_hexpand is needed for the buttons to align on the right
  mButtonCancel.set_hexpand(true);
  mButtonCancel.set_margin_end(5); // Space between the buttons
  mButtonCancel.set_halign(Gtk::Align::END);
  
  mButtonBox.append(mButtonRun);
   
  mButtonRun.set_halign(Gtk::Align::END);

  // When the run button is clicked, the function on_button_run_clicked is called
  mButtonRun.signal_clicked().connect( sigc::mem_fun(*this,
              &MainWindow::on_button_run_clicked) );
  
  // Now creating the list with the sinlge column
  // Getting a ref to a TreeModel
  mpTreeModel = Gtk::ListStore::create(mColumns);

  // Now setting the model above to be used in the TreeView
  mTreeView.set_model(mpTreeModel);

  // Now creating the rows in the list

  for (std::string iProjectName : mnProjectNames) {
    
    auto Row = *(mpTreeModel->append());
    Row[mColumns.mColProjectName] = iProjectName;
  
  }
  
  // Actually putting the column in the view
  mTreeView.append_column("Project Name", mColumns.mColProjectName);
    
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_button_cancel_clicked()
{
  //std::cout << "This is the main window reacting on button cancel clicked." << std::endl;
  close(); // Closing the window
}

// By default one row only can be selected

void MainWindow::on_button_run_clicked()
{
  //std::cout << "This is the main window reacti8ng on button RUN clicked." << std::endl;

  Glib::RefPtr<Gtk::TreeSelection> pTreeSelection = mTreeView.get_selection();
  Glib::RefPtr<Gtk::TreeModel> pModel = mTreeView.get_model();

  Gtk::TreeModel::iterator iter = pTreeSelection->get_selected(pModel);

  Gtk::TreeModel::Row Row = *iter;

  std::cout << Row[mColumns.mColProjectName] << std::endl; 
  
  close(); // Closing the window
}
