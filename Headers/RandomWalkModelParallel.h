#ifndef RANDOM_WALK_MODEL_PARALLEL
#define RANDOM_WALK_MODEL_PARALLEL

#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include "RandomWalkModel.h"
#include "MultithreadingController.h"

using namespace std;

class RandomWalkModelParallel : public RandomWalkModel {
    
    private:
    
        int threadCount;

        int currentProcessorAvailableThreads;

        void processChunk(int startRow, int endRow) {
            for (int i = startRow; i < endRow; ++i) {
                for (int j = 0; j < this->populationMatrixSize; ++j) {
                    this->individualTransition(i, j);
                }
            }
        }

        void throwIfMaximumThreadsIsExceeded()
        {
            if(this->threadCount > this->currentProcessorAvailableThreads) {
                throw out_of_range("ERROR: THE REQUESTED THREADS COUNT EXCEEDS THE CURRENT PROCESSOR AVAILABLE THREADS.");
            }
        }

        void throwIfMultithreadingIsNotSupported()
        {
            if(this->threadCount == 0) {
                throw out_of_range("ERROR: THE CURRENT PROCESSOR DOES NOT SUPPORTS MULTITHREADING.");
            }
        }

    public:

        using RandomWalkModel::RandomWalkModel; // Inherit constructor.

        RandomWalkModelParallel(int populationMatrixSize, double contagionFactor, bool applySocialDistanceEffect, int threadCount):
         RandomWalkModel(populationMatrixSize, contagionFactor, applySocialDistanceEffect), threadCount(threadCount)
        {
            this->currentProcessorAvailableThreads = MultithreadingController::getCurrentProcessorAvailableThreads();
            this->throwIfMultithreadingIsNotSupported();
            this->throwIfMaximumThreadsIsExceeded();
        }

        void parallelSimulation(int generations) {
            for (int g = 0; g < generations; ++g) {
                // Create threads to process chunks of the population grid.
                vector<thread> threads;
                int rowsPerThread = this->populationMatrixSize / this->threadCount;
                int remainingRows = this->populationMatrixSize % this->threadCount;

                for (int t = 0; t < this->threadCount; ++t) {
                    int startRow = t * rowsPerThread;
                    int endRow = startRow + rowsPerThread + (t < remainingRows ? 1 : 0);

                    threads.emplace_back([this, startRow, endRow]() {
                        processChunk(startRow, endRow);
                    });
                }

                // Wait for all threads to finish.
                for (auto& t : threads) {
                    t.join();
                }

                // Swap population data.
                this->population = this->nextPopulation;
            }
        }
};


#endif