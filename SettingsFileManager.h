#ifndef _SETTINGS_FILE_MANAGER_H_
#define _SETTINGS_FILE_MANAGER_H_

#include <fstream>
#include <vector>
#include <string>


class Settings_File_Manager
{
public:
    // Constructor
    Settings_File_Manager();
    // Destructor
    ~Settings_File_Manager();
    // load_settings responsible for getting the settings from external file
    std::vector<std::string> load_settings();
    // save_settings responsible for saving the settings to external file
    void save_settings(std::vector<std::string> settings);
private:
    // represents the external file where settings are located
    std::fstream settings_file;
};

#endif