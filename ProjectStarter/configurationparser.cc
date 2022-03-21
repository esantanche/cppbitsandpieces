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
 
  FILE* pJsonFile = fopen("big.json", "rb"); // non-Windows use "r"
  
  char ReadBuffer[65536];

  // Reading json configuration from file
  rapidjson::FileReadStream InputStream(pJsonFile, ReadBuffer, sizeof(ReadBuffer));
  
  //rapidjson::Document JsonConfiguration;
  mJsonConfiguration.ParseStream(InputStream);

  assert(mJsonConfiguration.IsArray());

  // FIXME why rapidjson::SizeType instead of just int?
  // Uses SizeType instead of size_t
  for (rapidjson::SizeType i = 0; i < mJsonConfiguration.Size(); i++) {
    const rapidjson::Value& iProject = mJsonConfiguration[i];
    mnProjectNames.push_back(iProject["project_name"].GetString());

    // FIXME  iProject["tasks"]  
    const rapidjson::Value& iProjectTasks = iProject["tasks"];
    assert(iProjectTasks.IsArray());
    for (rapidjson::SizeType j = 0; j < iProjectTasks.Size(); j++) {
      const rapidjson::Value& iTask = iProjectTasks[j];
    }
  }

  // FIXME  this goes in its own method
  
  initialize_lookup_table_for_executables();

    
  fclose(pJsonFile);
    
}

ConfigurationParser::~ConfigurationParser()
{
}

std::vector<std::string> ConfigurationParser::get_project_names()
{

  return mnProjectNames;
}

void ConfigurationParser::initialize_lookup_table_for_executables()
{

  mnTaskType2Executable["BROWSER"] = "firefox";
  mnTaskType2Executable["VIDEOPLAYER"] = "vlc";
  mnTaskType2Executable["FILEMANAGER"] = "nautilus";

  // FIXME  need to add PDFs
  // FIXME  what about         workspace-rename.sh LNK


  return;
}

std::string ConfigurationParser::get_extra_option_for_given_executable(std::string Executable, std::string ExecutableParameters) 
{
  // FIXME  to be completed
  // Needs to return for example --new-window for firefox in some cases
  std::string ExtraOptions = "";

  if (std::regex_match(Executable, std::regex(".*firefox.*") )) {
    //std::cout << "matched" << std::endl;
    // FIXME  if the ExecutableParameters is two or more uris, no need for --new-window

    // FIXME  does it work with 3 or more urls?
    if (!std::regex_match(ExecutableParameters, std::regex(".*\\s+.*"))) {
      //std::cout << "found space " << ExecutableParameters << '\n';
      ExtraOptions = "--new-window";
    }

  }

  return ExtraOptions;
}

bool ConfigurationParser::needs_to_go_to_the_background(std::string Executable)
{
  bool NeedsToGoToTheBackground = false;

  if (std::regex_match(Executable, std::regex(".*vlc.*") )) 
    NeedsToGoToTheBackground = true;


  return NeedsToGoToTheBackground;
}


  //std::vector<std::string> get_tasks_for_a_project(std::string ProjectName);

bool ConfigurationParser::run_tasks_for_a_project(Glib::ustring ProjectName)
{
  // FIXME exception handling if the give project name is not found

  bool ProjectFound = false;

  for (rapidjson::SizeType i = 0; i < mJsonConfiguration.Size(); i++)
  {
    const rapidjson::Value &iProject = mJsonConfiguration[i];
    // std::cout << i << "---" << iProject["project_name"].GetString()  << std::endl;
    //mnProjectNames.push_back(iProject["project_name"].GetString());

    if (iProject["project_name"].GetString() == ProjectName ) {

      std::cout << "got it" << ProjectName << std::endl;

      ProjectFound = true;

      const rapidjson::Value &iProjectTasks = iProject["tasks"];

      // FIXME  what's this rapidjson::SizeType about?
      for (rapidjson::SizeType j = 0; j < iProjectTasks.Size(); j++)
      {
        const rapidjson::Value &iTask = iProjectTasks[j];
      
        // I have to find a way to specify --new-window to use with firefox 
        // maybe I add an attribute "task_options"

        const std::string cTaskType = (std::string) iTask["task_type"].GetString();
        std::string TaskURI = (std::string) iTask["task_uri"].GetString();

        if (!TaskURI.empty())
          TaskURI = "\"" + TaskURI + "\"";

        const std::string cExecutable = mnTaskType2Executable[cTaskType];

        const std::string cExtraOptions = get_extra_option_for_given_executable(cExecutable, TaskURI);

        std::string Command = cExecutable + " " + cExtraOptions + " " + TaskURI;

        if (needs_to_go_to_the_background(cExecutable)) 
            Command = "nohup " + Command + " &";
        
        std::cout << Command << std::endl;

        const char *cpCommand = Command.c_str();

        system(cpCommand);

        sleep(5);

      }

    }

  }

  // true on success, false on failure
  return ProjectFound;
}
