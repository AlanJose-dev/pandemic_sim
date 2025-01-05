#ifndef RANDOM_WAL_MODEL_H
#define RANDOM_WAL_MODEL_H

#include <iostream>
#include <vector>
#include "Individual.h"
#include "State.h"
#include "RandomNumberGenerator.h"
#include "ImageGenerator.h"

using namespace std;

/**
 * The RandomWalkModel handle the simulation steps.
 */
class RandomWalkModel {

    protected:

        /**
         * Random number generator machine.
         */
        RandomNumberGenerator* randomNumberGenerator;

        /**
         * The population grid stores the individuals based on matrix size param.
         */
        vector<vector<Individual>> population;

        /**
         * Pre load population to next run.
         */
        vector<vector<Individual>> nextPopulation;

        /**
         * States change probabilities, ideally the sum of each line should result in 1.
         */
        vector<vector<double>> transitionProbabilities;

        /**
         * The disease contagion factor.
         * The default value is 0.5, corresponding to the value of the COVID 19 virus.
         */
        double contagionFactor;

        /**
         * The population grid size.
         */
        int populationMatrixSize;

        /**
         * Determines if the lockdown effect is active.
         */
        bool applySocialDistanceEffect;

        /**
         * Fill the population vectors.
         */
        void initializePopulation()
        {
            this->population.resize(
                this->populationMatrixSize,
                vector<Individual>(this->populationMatrixSize, Individual(State::healthy))
            );
            this->nextPopulation = this->population;
        }

        /**
         * Put a sick individual for each one population grid.
         */
        void initializeSickIndividuals()
        {
            int startIndex = populationMatrixSize / 2;
            this->population[startIndex][startIndex].state = State::sick;
            this->nextPopulation[startIndex][startIndex].state = State::sick;
        }

        /**
         * Uses a randomic rate to calculate the social interactions.
         */
        void computeSocialInteractions(int line, int column)
        {
            int initialLine = max(0, line - 1);
            int finalLine = min(line + 2, this->populationMatrixSize);

            int isolatedCount = 0;

            for (int i = initialLine; i < finalLine; ++i) {
                int initialColumn = max(0, column - 1);
                int finalColumn = min(column + 2, this->populationMatrixSize);

                for (int j = initialColumn; j < finalColumn; ++j) {
                    Individual& neighbour = population[i][j];

                    if (neighbour.state == State::isolated && this->applySocialDistanceEffect) {
                        isolatedCount++;
                    }

                    if (neighbour.state == State::sick) {
                        computeSickContact(nextPopulation[line][column], neighbour);
                    }
                }
            }

            if (isolatedCount > 0 && this->applySocialDistanceEffect) {
                double reduction = 0.05 * isolatedCount;
                this->contagionFactor = max(this->contagionFactor * (1.0 - reduction), 0.1);
            }
        }

        /**
         * Handle the probability of an individual turns sick.
         */
        void computeSickContact(Individual& individual, Individual& neighbour)
        {
            if (individual.state == State::dead) return;

            double number = this->randomNumberGenerator->getRandomNumber();

            if (number < this->contagionFactor) {
                individual.state = State::sick;
            }
        }

        /**
         * Handle the individual state transition.
         */
        void individualTransition(int line, int column)
        {
            Individual& individual = population[line][column];

            if (individual.state == State::dead) {
                return;
            }

            if (individual.state == State::healthy) {
                this->computeSocialInteractions(line, column);
            } else {
                const vector<double>& probabilities = this->transitionProbabilities[static_cast<int>(individual.state)];
                double number = this->randomNumberGenerator->getRandomNumber();

                double cumulativeProbability = 0.0;
                for (size_t i = 0; i < probabilities.size(); ++i) {
                    cumulativeProbability += probabilities[i];
                    if (number <= cumulativeProbability) {
                        this->nextPopulation[line][column].state = static_cast<State>(i);
                        break;
                    }
                }
            }
        }

        /**
         * Make the individual transition for each individual and replace the population grids.
         */
        void nextGeneration()
        {
            for (int i = 0; i < this->populationMatrixSize; ++i) {
                for (int j = 0; j < this->populationMatrixSize; ++j) {
                    this->individualTransition(i, j);
                }
            }
            this->population = this->nextPopulation;
        }

    public:

        /**
         * Constructor.
         */
        RandomWalkModel(int size, double contagionFactor, bool socialDistanceEffect)
            : populationMatrixSize(size), contagionFactor(contagionFactor), applySocialDistanceEffect(socialDistanceEffect)
        {
            this->randomNumberGenerator = new RandomNumberGenerator();    
            this->initializePopulation();
            this->initializeSickIndividuals();
        }

        /**
         * Set the model states transition probabilities via main file.
         */
        void setTransitionProbabilities(vector<vector<double>> transitionProbabilities)
        {
            this->transitionProbabilities = transitionProbabilities;
        }

        /**
         * Get the individuals count based on given state.
         */
        int getStateCount(State state)
        {
            int cumulated = 0;
            for (const auto& row : this->population) {
                for (const auto& individual : row) {
                    if (individual.state == state) {
                        cumulated++;
                    }
                }
            }
            return cumulated;
        }

        void generateImage()
        {
            const char* imageFilename = "Visual_Example_";
            time_t currentTimestamp;
            time(&currentTimestamp);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", localtime(&currentTimestamp));
            string fullImageFilename = string(imageFilename) + buffer + ".png";
            ImageGenerator::generate(fullImageFilename.c_str(), this->population);
        }

        /**
         * Run the generations during the generations count.
         */
        void simulation(int generations)
        {
            for (int i = 0; i < generations; ++i) {
                this->nextGeneration();
            }
        }

};

#endif