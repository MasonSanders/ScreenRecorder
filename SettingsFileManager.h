#ifndef _SETTINGS_FILE_MANAGER_H_
#define _SETTINGS_FILE_MANAGER_H_

#include <fstream>
#include <vector>
#include <string>

class Settings_File_Manager
{
public:
    Settings_File_Manager();
    ~Settings_File_Manager();
    std::vector<std::string> load_settings();
    void save_settings(std::vector<std::string> settings);
private:
    std::fstream settings_file;
};

#endif