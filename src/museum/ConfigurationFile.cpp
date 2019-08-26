#include "ConfigurationFile.hpp"

#include "Map.hpp"

ConfigurationFile::ConfigurationFile() {
    /* Read configuration file */
    std::string file_name = "config.txt";

    std::ifstream infile(file_name);

    std::string line;
    while (std::getline(infile, line)) {
        std::vector<std::string> line_split = split(line, "=");
        if (line_split.size() < 1) continue;

        if (line_split[0] == "tcr_time") tcr_time_ = std::stoi(line_split[1]);
        if (line_split[0] == "TPS") tps_ = std::stoi(line_split[1]);
        if (line_split[0] == "use_dragon") use_dragon_ = static_cast<bool>(std::stoi(line_split[1]));
    }
}

size_t ConfigurationFile::GetTCRTime() {
    return tcr_time_;
}

size_t ConfigurationFile::GetTPS() {
    return tps_;
}

bool ConfigurationFile::UseDragon() {
    return use_dragon_;
}

