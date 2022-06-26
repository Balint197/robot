#define _USE_MATH_DEFINES

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

long double x;      // free coordinates in [m]
long double y;
long double rel_x;  // sensor distance from robot center [m]
long double rel_y;
int x_coord;        // free coordinates, transformed for display
int y_coord;
long double angle;
long double x_sensor;
long double y_sensor;


double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

vector<vector<long double>> read_csv() {
    vector<vector<long double>> content;
    vector<long double> row;
    string line, word;

    fstream file("robot.csv", ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(stold(word));
            content.push_back(row);
        }
    }
    else
        std::cout << "Could not open the file\n";

    return content;
}

void drawImage(vector<vector<int>> img) {
    FILE* imageFile;

    #pragma warning(suppress : 4996)
    imageFile = fopen("imgs/map_image.pgm", "wb");
    if (imageFile == NULL) {
        perror("ERROR: Cannot open output file");
        exit(EXIT_FAILURE);
    }

    fprintf(imageFile, "P2\n");               // filetype - PGM
    fprintf(imageFile, "%d %d\n", int(img[0].size()), int(img.size()));   // dimensions - w h
    fprintf(imageFile, "2\n");               
    // max value:
        //0 - (black) something there/unknown
        //1 - (gray)  empty 
        //2 - (white) travel path

    for (int i = 0; i < img.size(); i++)
    {    
        for (int j = 0; j < img[0].size(); j++)
        {
            fprintf(imageFile, "%d ", img[i][j]);
        }
        fprintf(imageFile, "\n");
    }
    fclose(imageFile);
}

vector<vector<int>> calc_vision(vector<vector<long double>> data) {
    // set speed of sound, size of step (bigger -> faster, but less dense)
    const long double c = 343.0; // speed of sound [m/s]
    const long double angle_step = 0.1; // iteration step for angle
    const long double dist_step = 0.00001; // iteration step for distance [m]

    // uncomment settings block (0/1) to determine forward direction rotation of bot 
    const int robot_x = 10; // robot x (US0 - US1 distance) / 2
    const int robot_y = 5; // robot y (US0 - US3 distance) / 2
    long double beta = atan(0.5); //atan(robot_x / robot_y)
/*  
    const int robot_x = 5; // robot x (US0 - US1 distance) / 2
    const int robot_y = 10; // robot y (US0 - US3 distance) / 2
    long double beta = deg2rad(90) - atan(0.5); //atan(robot_x / robot_y)
*/

    // make "map", initialize to zero
    vector<vector<int>> map;
    vector<int> submap;
    int mapSize = 1000;
    for (int i = 0; i < mapSize; ++i) {
      submap.push_back(0);
    }
    map.push_back(submap);

    // fill map with cells
    for (int row = 0; row < data.size(); row++)
    {
        cout << "\rCalculating row " << row << "...";
        // sensor distance from robot center [m]
        rel_x = sqrt(robot_x^2 + robot_y^2) * cos(beta + data[row][3]) * 0.01;
        rel_y = sqrt(robot_x^2 + robot_y^2) * sin(beta + data[row][3]) * 0.01;
        for (int sensor = 0; sensor < 4; sensor++)
        {
            for (long double angle_sensor_i = 0; angle_sensor_i < 30; angle_sensor_i+=angle_step)
            {
                // small angles iterated [rad]
                angle = data[row][3] + deg2rad(300 - (sensor * 90) + angle_sensor_i);
                for (long double dist = 0; dist < data[row][4 + static_cast<std::vector<long double, std::allocator<long double>>::size_type>(sensor)]; dist += dist_step)
                {
                    // calculate x and y coordinates of distance from sensor base (relative)
                    x_sensor = c * 0.5 * dist * cos(angle);
                    y_sensor = c * 0.5 * dist * sin(angle);
                    // calculate absolute coordinates (odometer + base-sensor + sensor relative)
                    switch (sensor)
                    {
                    case 3:
                        x = data[row][1] + x_sensor + rel_x;
                        y = data[row][2] + y_sensor + rel_y;
                        break;
                    case 2:
                        x = data[row][1] + x_sensor + rel_x;
                        y = data[row][2] + y_sensor - rel_y;
                        break;
                    case 1:
                        x = data[row][1] + x_sensor - rel_x;
                        y = data[row][2] + y_sensor - rel_y;
                        break;
                    case 0:
                        x = data[row][1] + x_sensor - rel_x;
                        y = data[row][2] + y_sensor + rel_y;
                        break;
                    default:
                        break;
                    }

                    x_coord = 300 + int(x * 100);   // offset negative values + increase coordinates for display
                    y_coord = 300 + int(y * 100);

                    // if out of range, add more rows to map
                    while (map.size() < y_coord + 1 || map.size() < x_coord + 1)
                    {
                        map.push_back(submap);
                    }
                    map[y_coord][x_coord] = 1;      // mark calculated cell as empty (y, x)
                }
            }
        }
    }
    for (int row = 0; row < data.size(); row++) {
        // mark cell as path (y, x)
        map[300 + int(data[row][2] * 100)][300 + int(data[row][1] * 100)] = 2;
    }
    return map;
}

int main()
{
    std::cout << "Reading file...\n";
    vector<vector<long double>> data = read_csv();

    std::cout << "Calculating cells...\n";
    vector<vector<int>> map_data = calc_vision(data);

    cout << "\rDrawing map image...\n";
    drawImage(map_data);
    cout << "Ready! You can view the map in the 'imgs' folder. \n";
}