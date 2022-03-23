/**
 * @file configurationparser.cc
 * @author your name (you@domain.com)
 * @brief 
 *
 */

#include "configurationparser.h"

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <regex>
#include <unistd.h>

#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/filereadstream.h"

ConfigurationParser::ConfigurationParser() 
{
 
  load_names_of_projects();
  
  initialize_lookup_table_for_executables();
    
}

ConfigurationParser::~ConfigurationParser()
{
}

void ConfigurationParser::load_names_of_projects()
{

  FILE* pJsonFile = fopen("projectstarterconfiguration.json", "rb"); // non-Windows use "r"
  
  char ReadBuffer[65536];

  // Reading json configuration from file
  rapidjson::FileReadStream InputStream(pJsonFile, ReadBuffer, sizeof(ReadBuffer));
  
  //rapidjson::Document JsonConfiguration;
  mnJsonConfiguration.ParseStream(InputStream);

  assert(mnJsonConfiguration.IsArray());

  // The index i could be declared as size_t being an unsigned integer iterating on the
  // items of an array. We need to use rapidjson::SizeType instead because rapidjson uses
  // 32 bit sizes even on a 64 bits platform
  // See http://rapidjson.org/namespacerapidjson.html#a44eb33eaa523e36d466b1ced64b85c84
  for (rapidjson::SizeType i = 0; i < mnJsonConfiguration.Size(); i++) {
    const rapidjson::Value& iProject = mnJsonConfiguration[i];
    mnProjectNames.push_back(iProject["project_name"].GetString());
  }

  fclose(pJsonFile);
  
}

std::vector<std::string> ConfigurationParser::get_project_names()
{

  return mnProjectNames;
}

/**
 * @brief 
 * 
 */
void ConfigurationParser::initialize_lookup_table_for_executables()
{

  mnTaskType2Executable["BROWSER"] = "firefox";
  mnTaskType2Executable["VIDEOPLAYER"] = "vlc";
  mnTaskType2Executable["FILEMANAGER"] = "nautilus";
  mnTaskType2Executable["PDFVIEWER"] = "evince";
  mnTaskType2Executable["TEXTEDITOR"] = "geany";
  mnTaskType2Executable["SHELL"] = "bash -c";

  return;
}

/**
 * @brief 
 * 
 * @param Executable 
 * @param ExecutableParameters 
 * @return std::string 
 */
std::string ConfigurationParser::get_extra_option_for_given_executable(std::string Executable, std::string ExecutableParameters) 
{
  
  // Needs to return for example --new-window for firefox in some cases

  std::string ExtraOptions = "";

  if (std::regex_match(Executable, std::regex(".*firefox.*") )) {

    // It adds the option --new-window if there is one only URL
    // It works with 2, 3 or more URLs
    if (!std::regex_match(ExecutableParameters, std::regex(".*\\s+.*"))) 
      ExtraOptions = "--new-window";

  }

  return ExtraOptions;
}

// FIXME  add comments Doxygen style

/**
 * @brief 
 * 
 * @param Executable 
 * @return true 
 * @return false 
 */
bool ConfigurationParser::needs_to_go_to_the_background(std::string Executable)
{
  bool NeedsToGoToTheBackground = false;

  if (std::regex_match(Executable, std::regex(".*(vlc|evince|nautilus|geany).*") )) 
    NeedsToGoToTheBackground = true;

  return NeedsToGoToTheBackground;
}

bool ConfigurationParser::run_tasks_for_a_project(Glib::ustring ProjectName)
{

  bool ProjectFound = false;

  for (rapidjson::SizeType i = 0; i < mnJsonConfiguration.Size(); i++)
  {
    const rapidjson::Value &iProject = mnJsonConfiguration[i];

    if (iProject["project_name"].GetString() == ProjectName ) {

      ProjectFound = true;

      const rapidjson::Value &iProjectTasks = iProject["tasks"];

      for (rapidjson::SizeType j = 0; j < iProjectTasks.Size(); j++)
      {
        const rapidjson::Value &iTask = iProjectTasks[j];

        const std::string cTaskType = (std::string) iTask["task_type"].GetString();
        std::string TaskURI = (std::string) iTask["task_uri"].GetString();

        // If the task type is "BROWSER", TaskURI contains one or many URLs (or URIs).
        // They don't have spaces.
        // But if the task is different, TaskURI contains a single path with spaces.
        // In this case, I add quotations otherwise the path would be interprested as many.

        if (cTaskType != "BROWSER" && !TaskURI.empty()) 
          TaskURI = "\"" + TaskURI + "\"";

        const std::string cExecutable = mnTaskType2Executable[cTaskType];

        const std::string cExtraOptions = get_extra_option_for_given_executable(cExecutable, TaskURI);

        std::string Command = cExecutable + " " + cExtraOptions + " " + TaskURI;

        if (needs_to_go_to_the_background(cExecutable)) 
            Command = "nohup " + Command + ">/dev/null 2>&1 &";
        
        const char *cpCommand = Command.c_str();

        system(cpCommand);

        sleep(3);

      }

    }

  }

  // true on success, false on failure
  return ProjectFound;
}
