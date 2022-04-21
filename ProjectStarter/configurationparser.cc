/**
 * @file configurationparser.cc
 * @author Emanuele Santanche (http://emanuelesantanche.com/)
 * @brief This class loads and parses the configuration file.
 * It also puts the commands to run together and actually runs the tasks 
 * for a project when the user chooses the project.
 * 
 * @ref  https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/
 * @ref  https://stackoverflow.com/questions/56143678/making-a-vector-of-pairs-sorting-it-and-then-extract-the-vectors-from-it
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

using namespace std;

/**
 * @brief Construct a new Configuration Parser:: Configuration Parser object
 * It loads the names of the projects from the json file.
 * Doing this, it actually loads the entire json file. 
 * It will use it later to run the tasks.
 */
ConfigurationParser::ConfigurationParser() 
{
 
  load_names_of_projects();
  
  initialize_lookup_table_for_executables();
    
}

ConfigurationParser::~ConfigurationParser()
{
}

bool cmp(pair<string, int>& a,
         pair<string, int>& b)
{
    return a.second < b.second;
}

/**
 * @brief Loads the entire json file and creates the vector that contains the names of the projects.
 */
void ConfigurationParser::load_names_of_projects()
{

  mJsonConfigurationIsCorrect = true;

  FILE* pJsonFile = fopen("projectstarterconfiguration.json", "rb"); // non-Windows use "r"
  
  char ReadBuffer[65536];

  // Reading json configuration from file
  rapidjson::FileReadStream InputStream(pJsonFile, ReadBuffer, sizeof(ReadBuffer));
  
  mnJsonConfiguration.ParseStream(InputStream);

  if (mnJsonConfiguration.HasParseError()) {

    mJsonConfigurationIsCorrect = false;

    cout << "There is an error in the json configuration file." << endl;
    cout << "Probably it's a missing comma or parentesis." << endl;

  } else {

    assert(mnJsonConfiguration.IsArray());

    // FIXME  there may be names to fix everywhere to follow naming conventions

    // The index i could be declared as size_t being an unsigned integer iterating on the
    // items of an array. We need to use rapidjson::SizeType instead because rapidjson uses
    // 32 bit sizes even on a 64 bits platform
    // See http://rapidjson.org/namespacerapidjson.html#a44eb33eaa523e36d466b1ced64b85c84
    for (rapidjson::SizeType i = 0; i < mnJsonConfiguration.Size(); i++) {
      const rapidjson::Value& iProject = mnJsonConfiguration[i];
      const string cProjectName = iProject["project_name"].GetString();
      const int cProjectPriority = iProject["priority"].GetInt();
      const pair <string, int> fixmemethisname = make_pair(cProjectName, cProjectPriority);
      if (cProjectPriority > 0)
        mnProjectNames.push_back(fixmemethisname);
    }

    fclose(pJsonFile);

    sort(mnProjectNames.begin(), mnProjectNames.end(), cmp);

  }

  // FIXME  a lot of cleaning up

}

bool ConfigurationParser::json_configuration_is_correct()
{

  return mJsonConfigurationIsCorrect;
}


/**
 * @brief Returns the names of the projects
 * 
 * @return vector<string> Names of the projects
 */
vector<string> ConfigurationParser::get_project_names()
{
  vector<string> ListOfOrderedProjectsNames;

  for ( auto iProject = mnProjectNames.begin(); iProject != mnProjectNames.end(); iProject++ )
  {
      ListOfOrderedProjectsNames.push_back(iProject->first);
  }

  return ListOfOrderedProjectsNames;
}

/**
 * @brief Initializes the lookup table that associates the type of task to the
 * actual executable to run 
 */
void ConfigurationParser::initialize_lookup_table_for_executables()
{

  mnTaskType2Executable["BROWSER"] = "firefox";
  mnTaskType2Executable["VIDEOPLAYER"] = "vlc";
  mnTaskType2Executable["FILEMANAGER"] = "nautilus";
  mnTaskType2Executable["PDFVIEWER"] = "evince";
  mnTaskType2Executable["TEXTEDITOR"] = "geany";
  mnTaskType2Executable["SHELL"] = "bash -c";
  mnTaskType2Executable["DOC"] = "libreoffice";

  // The TaskURI will be a folder where the terminal should start in
  // The executable should contain the substring "terminal" for the command to execute
  // to be assembled correctly. For example, this app may call the terminal app tilda incorrectly
  // I tested gnome-terminal and xfce4-terminal
  mnTaskType2Executable["TERMINAL"] = "gnome-terminal"; 

  return;
}

/**
 * @brief Generates the command to run that will start the executable (firefox, gnome-terminal, vlc, etc.)
 * 
 * @param Executable This is the executable (firefox, gnome-terminal, vlc, etc.)
 * @param ExecutableParameters This is the property task_uri in the json file
 * @return string Command to run
 */
string ConfigurationParser::put_command_together(string Executable, string ExecutableParameters) 
{
  
  string ExtraOptions = "";
  string Command;

  if (regex_match(Executable, regex(".*firefox.*") )) {

    // It adds the option --new-window if there is one only URL
    // It works with 2, 3 or more URLs
    if (!regex_match(ExecutableParameters, regex(".*\\s+.*"))) 
      ExtraOptions = "--new-window";

    Command = Executable + " " + ExtraOptions + " " + ExecutableParameters;

  } else if (regex_match(Executable, regex(".*terminal.*"))) {

    // For terminal apps, ExecutableParameters contains the working folder
    // I specify it using the option --working-directory.
    // Both gnome-terminal and xfce4-terminal support this option

    Command = Executable + " --working-directory=" + ExecutableParameters;

  } else {

    Command = Executable + " " + ExecutableParameters;

  }

  return Command;
}

/**
 * @brief Determines if the executable has to run in the background or it will
 * block the app
 * 
 * @param Executable This is the executable (firefox, gnome-terminal, vlc, etc.)
 * @return true The executable has to run in the background
 * @return false It hasn't
 */
bool ConfigurationParser::needs_to_go_to_the_background(string Executable)
{
  bool NeedsToGoToTheBackground = false;

  if (regex_match(Executable, regex(".*(vlc|evince|nautilus|geany|terminal|libreoffice).*") )) 
    NeedsToGoToTheBackground = true;

  return NeedsToGoToTheBackground;
}

/**
 * @brief The user has chosen the project and this method runs the tasks
 * 
 * @param ProjectName This is the name of the project that has been chosen 
 * @return true The given name of project exists
 * @return false It doesn't (this is a critical error and aborts the app, the given project name should always exist)
 */
bool ConfigurationParser::run_tasks_for_a_project(Glib::ustring ProjectName)
{

  bool ProjectFound = false;

  // The index i could be declared as size_t being an unsigned integer iterating on the
  // items of an array. We need to use rapidjson::SizeType instead because rapidjson uses
  // 32 bit sizes even on a 64 bits platform
  // See http://rapidjson.org/namespacerapidjson.html#a44eb33eaa523e36d466b1ced64b85c84
  for (rapidjson::SizeType i = 0; i < mnJsonConfiguration.Size(); i++)
  {
    const rapidjson::Value &iProject = mnJsonConfiguration[i];

    if (iProject["project_name"].GetString() == ProjectName ) {

      // I found the project the user chose

      ProjectFound = true;

      const rapidjson::Value &iProjectTasks = iProject["tasks"];

      // Now I run all the tasks

      for (rapidjson::SizeType j = 0; j < iProjectTasks.Size(); j++)
      {
        const rapidjson::Value &iTask = iProjectTasks[j];

        const string cTaskType = (string) iTask["task_type"].GetString();
        string TaskURI = (string) iTask["task_uri"].GetString();

        // If the task type is "BROWSER", TaskURI contains one or many URLs (or URIs).
        // They don't have spaces.
        // But if the task is different, TaskURI contains a single path with spaces.
        // In this case, I add quotations otherwise the path would be interpreted as many.

        if (cTaskType != "BROWSER" && !TaskURI.empty()) 
          TaskURI = "\"" + TaskURI + "\"";

        const string cExecutable = mnTaskType2Executable[cTaskType];

        string Command = put_command_together(cExecutable, TaskURI);

        if (needs_to_go_to_the_background(cExecutable)) 
            Command = "nohup " + Command + ">/dev/null 2>&1 &";
        
        const char *cpCommand = Command.c_str();

        system(cpCommand);

        sleep(3);

      }

    }

  }

  return ProjectFound;
}
