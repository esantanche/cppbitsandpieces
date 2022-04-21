#ifndef GTKMM_PRJSTR_CFGPARSER_H
#define GTKMM_PRJSTR_CFGPARSER_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <string>

#include "include/rapidjson/document.h"

using namespace std;

class ConfigurationParser 
{

public:

  ConfigurationParser();
  virtual ~ConfigurationParser();

  vector<string> get_project_names();
  
  bool run_tasks_for_a_project(Glib::ustring ProjectName);

  bool json_configuration_is_correct();

protected:
  
  map<string, string> mnTaskType2Executable;
  vector<pair<string, int>> mnProjectNames;
  //vector<string> mnProjectNames;
  rapidjson::Document mnJsonConfiguration;
  bool mJsonConfigurationIsCorrect;

  void initialize_lookup_table_for_executables();
  void load_names_of_projects();
  string put_command_together(string Executable, string ExecutableParameters);
  bool needs_to_go_to_the_background(string Executable);

};

#endif //GTKMM_PRJSTR_CFGPARSER_H
