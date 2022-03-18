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

protected:
  
  std::vector<std::string> mnProjectNames;
  rapidjson::Document mJsonConfiguration;

};

#endif //GTKMM_PRJSTR_CFGPARSER_H
