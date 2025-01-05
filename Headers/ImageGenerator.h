#ifndef IMAGE_GENERATOR_H
#define IMAGE_GENERATOR_H

#include <vector>
#include <iostream>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/StbImageWrite.h"
#include "Individual.h"
#include "State.h"

using namespace std;

using Population = vector<vector<Individual>>;

class ImageGenerator {

public:

    static void generate(const char* name, Population& population) {
        // Get the population matrix dimensions
        const int lines = static_cast<int>(population.size());
        const int columns = static_cast<int>(population[0].size());

        // Create an RGB buffer to store the image
        vector<unsigned char> imageBuffer(lines * columns * 3, 0);

        // Iterate over the population matrix and set pixel colors based on the individual's state
        for (int i = 0; i < lines; ++i) {
            for (int j = 0; j < columns; ++j) {
                int index = (i * columns + j) * 3; // Calculate the buffer index

                switch (population[i][j].state) {
                    case State::healthy:
                        imageBuffer[index + 0] = 0; // Red
                        imageBuffer[index + 1] = 255; // Green
                        imageBuffer[index + 2] = 0; // Blue
                        break;

                    case State::isolated:
                        imageBuffer[index + 0] = 0;
                        imageBuffer[index + 1] = 0;
                        imageBuffer[index + 2] = 0;
                        break;

                    case State::sick:
                        imageBuffer[index + 0] = 255;
                        imageBuffer[index + 1] = 255;
                        imageBuffer[index + 2] = 0;
                        break;

                    case State::dead:
                        imageBuffer[index + 0] = 255;
                        imageBuffer[index + 1] = 0;
                        imageBuffer[index + 2] = 0;
                        break;

                    case State::immune:
                        imageBuffer[index + 0] = 0;
                        imageBuffer[index + 1] = 0;
                        imageBuffer[index + 2] = 255;
                        break;

                    default:
                        cerr << "ERROR: INVALID STATE ON IMAGE GENERATION" << endl;
                        break;
                }
            }
        }

        if (stbi_write_png(name, columns, lines, 3, imageBuffer.data(), columns * 3)) {
            cout << "\nImage saved successfully as " << name << endl;
        } else {
            cerr << "\nERROR: Failed to save image as " << name << endl;
        }
    }

};

#endif
