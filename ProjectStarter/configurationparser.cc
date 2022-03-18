#include "configurationparser.h"

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>
#include <cstdio>
#include <string>
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
    //std::cout << i << "---" << iProject["project_name"].GetString()  << std::endl;
    mnProjectNames.push_back(iProject["project_name"].GetString());

    // iProject["tasks"]
    const rapidjson::Value& iProjectTasks = iProject["tasks"];
    assert(iProjectTasks.IsArray());
    for (rapidjson::SizeType j = 0; j < iProjectTasks.Size(); j++) {
      const rapidjson::Value& iTask = iProjectTasks[j];
      //std::cout << i << "---" << j << "---" << iTask["task_name"].GetString()  << std::endl;
    }
  }
    
  fclose(pJsonFile);
    
}

ConfigurationParser::~ConfigurationParser()
{
}

std::vector<std::string> ConfigurationParser::get_project_names()
{

  return mnProjectNames;
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
        // std::cout << i << "---" << j << "---" << iTask["task_name"].GetString()  << std::endl;
        // std::cout << i << "---" << j << "---" << iTask["task_type"].GetString()  << std::endl;  
        // std::cout << i << "---" << j << "---" << iTask["task_uri"].GetString()  << std::endl;  

        // I have to find a way to specify --new-window to use with firefox 
        // maybe I add an attribute "task_options"

        const std::string cTaskURI = (std::string) iTask["task_uri"].GetString();
        const std::string cExecutable = "firefox ";
        const std::string cCommand = cExecutable + cTaskURI;

        std::cout << cCommand << std::endl;

        const char *cpCommand = cCommand.c_str();

        system(cpCommand);

        sleep(5);

      }

    }
    // // iProject["tasks"]
    // const rapidjson::Value &iProjectTasks = iProject["tasks"];
    // assert(iProjectTasks.IsArray());
    // for (rapidjson::SizeType j = 0; j < iProjectTasks.Size(); j++)
    // {
    //   const rapidjson::Value &iTask = iProjectTasks[j];
    //   // std::cout << i << "---" << j << "---" << iTask["task_name"].GetString()  << std::endl;
    // }
  }

  // true on success, false on failure
  return ProjectFound;
}
