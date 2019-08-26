#ifndef __Map_hpp__
#define __Map_hpp__

#define MAP_SIZE_HORIZONTAL 30
#define MAP_SIZE_VERTICAL 30
#define MAP_SIZE_X 30
#define MAP_SIZE_Y 30

#include <vector>
#include <string>
#include <fstream>

static size_t map_scale = 3;
static float camera_speed = 4;

static std::vector<std::string> split(const std::string & str, const std::string & delim) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

static int** ReadMapFromFile(std::string file) {
    std::ifstream infile(file);

    int** map = new int*[MAP_SIZE_HORIZONTAL];
    for (size_t i = 0; i < MAP_SIZE_HORIZONTAL; i++)
        map[i] = new int[MAP_SIZE_VERTICAL];

    std::string line;
    size_t i = 0, j = 0;
    while (std::getline(infile, line)) {
        std::vector<std::string> line_split = split(line, " ");
        for (size_t str = 0; str < line_split.size(); str++) {
            map[i][j++] = std::stoi(line_split[str]);
        }
        i++;
        j = 0;
    }

    return map;
}


#endif