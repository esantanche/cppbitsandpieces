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
 
  // FIXME  put this is its own method
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

  fclose(pJsonFile);
  
  initialize_lookup_table_for_executables();
    
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
  mnTaskType2Executable["PDFVIEWER"] = "evince";
  mnTaskType2Executable["TEXTEDITOR"] = "geany";


  // FIXME  what about         workspace-rename.sh LNK
  // FIXME  need to be able to open txt files and shell scripts

  return;
}

std::string ConfigurationParser::get_extra_option_for_given_executable(std::string Executable, std::string ExecutableParameters) 
{
  // FIXME  to be completed
  // Needs to return for example --new-window for firefox in some cases
  std::string ExtraOptions = "";

  if (std::regex_match(Executable, std::regex(".*firefox.*") )) {

    // It adds the option -new-window if there is one only URL
    // It works with 2, 3 or more URLs
    if (!std::regex_match(ExecutableParameters, std::regex(".*\\s+.*"))) 
      ExtraOptions = "--new-window";

  }

  return ExtraOptions;
}

bool ConfigurationParser::needs_to_go_to_the_background(std::string Executable)
{
  bool NeedsToGoToTheBackground = false;

  // FIXME  nautilus needs to go to the background if there is no other instance running of it

  // FIXME  you have to test all executables when there is already an instance of them running and when there isn't
  // there may be more executables that need to be sent to the background

  if (std::regex_match(Executable, std::regex(".*(vlc|evince|nautilus).*") )) 
    NeedsToGoToTheBackground = true;

  return NeedsToGoToTheBackground;
}

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

      //std::cout << "got it" << ProjectName << std::endl;

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

        // FIXME  attention! folder paths may still have spaces!!
        // maybe i can just not support opening many folders in one single task

        if (cTaskType != "BROWSER" && !TaskURI.empty()) {
          TaskURI = "\"" + TaskURI + "\"";
        }

        const std::string cExecutable = mnTaskType2Executable[cTaskType];

        const std::string cExtraOptions = get_extra_option_for_given_executable(cExecutable, TaskURI);

        std::string Command = cExecutable + " " + cExtraOptions + " " + TaskURI;

        // FIXME maybe add this so that you don't get the nohup.out: >/dev/null 2>&1 
        if (needs_to_go_to_the_background(cExecutable)) 
            Command = "nohup " + Command + " &";
        
        std::cout << Command << std::endl;

        const char *cpCommand = Command.c_str();

        system(cpCommand);

        sleep(3);

      }

    }

  }

  // true on success, false on failure
  return ProjectFound;
}
