#ifndef IMAGE_GENERATOR_H
#define IMAGE_GENERATOR_H

#include <vector>
#include <iostream>
#include "lib/CImg.h"
#include "Individual.h"
#include "State.h"

#define cimg_use_magick

using namespace std;
using namespace cimg_library;

using Population = vector<vector<Individual>>;
using Image = CImg<unsigned char>;

class ImageGenerator {

    public:

        static void generate(const char* name, Population& population)
        {
            // Get the population matrix lines and columns count.
            const int lines = static_cast<int>(population.size());
            const int columns = static_cast<int>(population[0].size());

            // CImg RGB channels ids.
            const int CIMG_RED = 0;
            const int CIMG_GREEN = 1;
            const int CIMG_BLUE = 2;

            /**
             * Create a image object with:
             * 2 dimentions
             * 3 channels
             * black filled: 0
             */
            Image image(lines, columns, 1, 3, 0);
            // Fill the image with green.
            cimg_forXY(image, x, y) {
                image(x, y, CIMG_RED) = 0;
                image(x, y, CIMG_GREEN) = 255;
                image(x, y, CIMG_BLUE) = 0;
            };

            /**
             * Iterates over lines and columns and adding pixels corresponding with the individual state.
             */
            for(int i = 0; i < lines; ++i) {
                for(int j = 0; j < columns; ++j) {
                    switch(population[i][j].state) {
                        case State::healthy:
                            // Green.
                            image(i, j, CIMG_RED) = 0;
                            image(i, j, CIMG_GREEN) = 255;
                            image(i, j, CIMG_BLUE) = 0;
                        break;

                        case State::isolated:
                            // Black.
                            image(i, j, CIMG_RED) = 0;
                            image(i, j, CIMG_GREEN) = 0;
                            image(i, j, CIMG_BLUE) = 0;
                        break;

                        case State::sick:
                            // Yellow.
                            image(i, j, CIMG_RED) = 255;
                            image(i, j, CIMG_GREEN) = 255;
                            image(i, j, CIMG_BLUE) = 0;
                        break;

                        case State::dead:
                            // Red.
                            image(i, j, CIMG_RED) = 255;
                            image(i, j, CIMG_GREEN) = 0;
                            image(i, j, CIMG_BLUE) = 0;
                        break;

                        case State::immune:
                            // Blue.
                            image(i, j, CIMG_RED) = 0;
                            image(i, j, CIMG_GREEN) = 0;
                            image(i, j, CIMG_BLUE) = 255;
                        break;

                        default:
                            cerr << "ERROR: INVALID STATE ON IMAGE GENERATION" << endl;
                        break;
                    }
                }
            }

            image.save(name);
        }

};

#endif