#include "SettingsFileManager.h"
#include <stdexcept>
#include <iostream>

// Constructor
Settings_File_Manager::Settings_File_Manager()
{}

// Destructor
Settings_File_Manager::~Settings_File_Manager() {}

// load_settings
std::vector<std::string> Settings_File_Manager::load_settings()
{   
    // get the settings from a csv file and return them
    std::vector<std::string> settings;
    std::string s;
    settings_file.open("settings.csv", std::fstream::in);
    if (settings_file.is_open())
    {
        while (std::getline(settings_file, s, ','))
        {
            settings.push_back(s);
        }
        settings_file.close();
    }
    else
    {
        throw std::runtime_error("Cannot find settings file.");
    }
    return settings;
}

// save_settings
void Settings_File_Manager::save_settings(std::vector<std::string> settings)
{   
    // store settings from the application back into the csv file
    settings_file.open("settings.csv", std::fstream::out);
    if (settings_file.is_open())
    {
        settings_file << settings[0] << ",";
        settings_file << settings[1] << ",";
        settings_file << settings[2];
        settings_file.close();
    }
    else
    {
        throw std::runtime_error("Cannot find settings file.");
    }
}