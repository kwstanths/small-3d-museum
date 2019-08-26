#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <limits>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Map.hpp"
#include "MersenneTwister.hpp"

static MersenneTwisterGenerator rng(MersenneTwisterGenerator::SEVEN);

/**
    (0,1)
*/
float get_rand() {
    return rng.genrand_real3();
}

/**
    [0,1]
*/
float get_rand2() {
    return rng.genrand_real1();
}

/**
    [0,1)
*/
float get_rand3() {
    return rng.genrand_real2();
}

unsigned long get_rand_int() {
    return rng.genrand_int32();
}

int get_rand_int_between(int min, int max) {
    return min + (max - min) * rng.genrand_real1();
}

template<typename T> static bool Equal(const T& a, const T& b, T e = std::numeric_limits<T>::epsilon()) {
    return std::abs(a - b) <= e;
}

double GetRadians(double degrees) {
    return static_cast<double>(M_PI / 180.0 * degrees);
}

double GetDegrees(double radians) {
    return static_cast<double>(radians * 180.0 / M_PI);
}


std::vector<std::pair<int, int>> Bresenham(int x1, int y1, int x2, int y2, int ** map) {

    /**
        Original author of this code can be found here: https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.2B.2B
    */

    std::vector<std::pair<int, int>> temp;

    bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if (steep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = y1;

    int maxX = x2;

    for (int x = x1; x < maxX; x++) {
        if (steep){
            temp.push_back(std::make_pair(y, x));
        } else {
            temp.push_back(std::make_pair(x, y));
        }

        if (map[x][y] == 1) break;

        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }

    return temp;
}

std::vector<std::pair<int, int>> GridTraversal(double px, double py, double angle, int ** map) {

    /*
        Grid traversal algorithm is adopted from here: http://www.cse.yorku.ca/~amana/research/grid.pdf
    */

    std::vector<std::pair<int, int>> temp;
    int x = std::floor(px / map_scale);
    int y = std::floor(py / map_scale);

    double dir_x = std::cos(angle);
    double dir_y = std::sin(angle);

    int step_x = (dir_x >= 0) ? 1 : -1;
    int step_y = (dir_y >= 0) ? 1 : -1;

    double near_x = (step_x >= 0) ? (x + 1) * map_scale - px : px - (x * map_scale);
    double near_y = (step_y >= 0) ? (y + 1) * map_scale - py : py - (y * map_scale);

    double tMaxX = (!Equal(dir_x, 0.0)) ? near_x / dir_x : std::numeric_limits<double>::max();
    double tMaxY = (!Equal(dir_y, 0.0)) ? near_y / dir_y : std::numeric_limits<double>::max();

    double tDeltaX = (!Equal(dir_x, 0.0)) ? map_scale / dir_x : std::numeric_limits<double>::max();
    double tDeltaY = (!Equal(dir_y, 0.0)) ? map_scale / dir_y : std::numeric_limits<double>::max();

    while (x >= 0 && x < MAP_SIZE_X && y >= 0 && y < MAP_SIZE_Y) {
        
        temp.push_back(std::make_pair(x, y));
        if (map[static_cast<int>(x)][static_cast<int>(y)] == 1) break;

        if (std::abs(tMaxX) < std::abs(tMaxY)) {
            tMaxX = tMaxX + tDeltaX;
            x = x + step_x;
        } else {
            tMaxY = tMaxY + tDeltaY;
            y = y + step_y;
        }
    }

    return temp;
}

int main(int argc, char ** argv) {
    
    int **  museum_map = ReadMapFromFile("map.txt");

    /* small test */
    {
        float point_x = float(16.5);
        float point_y = float(10.5);
        float angle = GetRadians(315);

        std::vector<std::pair<int, int>> cells = GridTraversal(point_x, point_y, angle, museum_map);
        std::cout << "test" << std::endl;
    }

    size_t RAYS_PER_CELL = 1000000;
    
    std::cout << "Total number of lines: " << MAP_SIZE_X << std::endl;
    std::vector<std::unordered_map<int, int>> visibility;
    for (size_t i = 0; i < MAP_SIZE_X; i++) {
        std::cout << "Calculating line: " << i << std::endl;
        for (size_t j = 0; j < MAP_SIZE_Y; j++) {
        
            /* Compute PVS for cell (i,j) */
            std::unordered_map<int, int> PVS;
            
            /* Random positiond and rays */
            for (size_t ray = 0; ray < RAYS_PER_CELL; ray++) {
                float point_x = map_scale * (float(i) + get_rand2());
                float point_y = map_scale * (float(j) + get_rand2());
                float angle = get_rand2() * 2.0f * M_PI;

                std::vector<std::pair<int, int>> cells = GridTraversal(point_x, point_y, angle, museum_map);
                for (size_t cell = 0; cell < cells.size(); cell++)
                    PVS[cells[cell].first*MAP_SIZE_Y + cells[cell].second] = 1;
            }

            visibility.push_back(PVS);
        }
    }

    /* Write to file */
    std::ofstream out("visibility.txt");

    for (size_t i = 0; i < visibility.size(); i++) {
        for (auto it = visibility[i].begin(); it != visibility[i].end(); ++it)
            out << it->first << " ";
        out << "\n";
    }
    out.close();

#ifdef _WIN32
    system("pause");
#endif // _WIN32


}