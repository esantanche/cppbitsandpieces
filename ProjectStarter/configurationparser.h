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

class ConfigurationParser 
{
public:
  ConfigurationParser();
  virtual ~ConfigurationParser();

  std::vector<std::string> get_project_names();
  // FIXME is there any problem using Glib::ustring instead of std::string?
  bool run_tasks_for_a_project(Glib::ustring ProjectName);

// For me it's ok to declare these members as private, isn't it? FIXME 
protected:
  
  std::map<std::string, std::string> mnTaskType2Executable;
  std::vector<std::string> mnProjectNames;
  rapidjson::Document mJsonConfiguration;
  void initialize_lookup_table_for_executables();
  std::string get_extra_option_for_given_executable(std::string Executable, std::string ExecutableParameters);
  bool needs_to_go_to_the_background(std::string Executable);

};

#endif //GTKMM_PRJSTR_CFGPARSER_H
