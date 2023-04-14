#include "SettingsFileManager.h"
#include <exception>

Settings_File_Manager::Settings_File_Manager() 
: settings_file("settings.csv")
{}

Settings_File_Manager::~Settings_File_Manager() {}

std::vector<std::string> Settings_File_Manager::load_settings()
{
    std::vector<std::string> settings;
    std::string s;
    if (settings_file.is_open())
    {
        while (getline(settings_file, s, ","))
        {
            settings.push_back(s);
        }
    }
    else
    {
        throw std::exception("Cannot find settings file.");
    }
    return settings;
}

void Settings_File_Manager::save_settings(std::vector<std::string> settings)
{
    if (settings_file.is_open())
    {
        settings_file << settings[0] << ",";
        settings_file << settings[1] << ",";
        settings_file << settings[2] << std::endl;
        settings_file.close();
    }
    else
    {
        throw std::exception("Cannot find settings file.");
    }
}