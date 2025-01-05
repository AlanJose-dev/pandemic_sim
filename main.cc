#include <getopt.h>
#include <memory>
#include <string>
#include "Headers/RandomWalkModel.h"
#include "Headers/RandomWalkModelParallel.h"
#include "Headers/State.h"
#include "Headers/ProgramInfoViewer.h"

using namespace std;

int main(int argc, char* argv[])
{
    //Default params.
    int numberOfRuns = 1000;
    int populationMatrixSize = 100;
    int numberOfGenerations = 10;
    double contagionFactor = 0.5;
    int requestedStateCount = static_cast<int>(State::dead);
    bool applySocialDistanceEffect = false;
    int threadCount = 1;
    bool generateImage = false;
    
    //Parse CLI options.
    //Don't move.

    //TODO: Add support to long params.
    const char* shortOptions = "r:p:g:st:c:o:ihv";
    int cliOption;
    while ((cliOption = getopt(argc, argv, shortOptions)) != -1) {
        switch (cliOption) {
            case 'r': {
                numberOfRuns = stoi(optarg);
            } break;
            case 'p': {
                populationMatrixSize = stoi(optarg);
            } break;
            case 'g': {
                numberOfGenerations = stoi(optarg);
            } break;
            case 's': {
                applySocialDistanceEffect = true;
            } break;
            case 't': {
                try
                {
                    int requestedThreadCount = stoi(optarg);
                    if(requestedThreadCount < 1) {
                        throw out_of_range("ERROR: THE REQUESTED THREADS COUNT IS LESS THAN 1.");
                    }
                    threadCount = requestedThreadCount;
                }
                catch(const out_of_range& exception)
                {
                    cerr << exception.what() << endl;
                    exit(EXIT_FAILURE);
                }
                
            } break;
            case 'c': {
                try
                {
                    const int MIN_CONTAGION_FACTOR = 0;
                    const int MAX_CONTAGION_FACTOR = 1;
                    double givenContagionFactor = stod(optarg);
                    if(givenContagionFactor < MIN_CONTAGION_FACTOR || givenContagionFactor > MAX_CONTAGION_FACTOR) {
                        throw out_of_range("ERROR: The contagion factor must be between " + to_string(MIN_CONTAGION_FACTOR) + " and " + to_string(MAX_CONTAGION_FACTOR) + ".");
                    }
                    contagionFactor = givenContagionFactor;
                }
                catch(const invalid_argument&)
                {
                    cerr << "Error: Invalid argument for -c. Expected a double.\n" << endl;
                }
                catch(const out_of_range& exception)
                {
                    cerr << exception.what() << endl;
                    exit(EXIT_FAILURE);
                }
            } break;
            case 'o': {
                try
                {
                    int requestedStateValue = stoi(optarg);
                    int stateEnumValuesCount = 5;
                    if(requestedStateValue < 0 || requestedStateValue > stateEnumValuesCount) {
                        throw out_of_range("ERROR: Invalid state: " + to_string(requestedStateValue) + ".");
                    }
                    requestedStateCount = requestedStateValue;
                }
                catch(const out_of_range& exception)
                {
                    cerr << exception.what() << endl;
                    exit(EXIT_FAILURE);
                }
            } break;
            case 'i': {
                generateImage = true;
            } break;
            case 'h':
                printHelp();
            exit(EXIT_SUCCESS); //Terminate the program.
            case 'v':
                printVersion();
            exit(EXIT_SUCCESS);
            default:
                cerr << "Unknown option. Use -h for usage information.\n";
            exit(EXIT_FAILURE);
        }
    }

    //Switch the probabilities as you need.

    /**
     * --------------------------------------------------------------
     *           |  Healthy |  Isolated |   Sick  |  Dead |  Immune
     *-----------|----------|-----------|---------|-------|----------
     * Healthy   |   0.62   |    0.3    |   0.05  |  0.0  |  0.03
     *-----------|----------|-----------|---------|-------|----------
     * Isolated  |   0.05   |   0.64    |    0.1  | 0.01  |   0.2
     *-----------|----------|-----------|---------|-------|----------
     * Sick      |    0.0   |    0.1    |   0.65  |  0.1  |  0.15
     *-----------|----------|-----------|---------|-------|----------
     * Dead      |    0.0   |    0.0    |    0.0  |  1.0  |   0.0
     *-----------|----------|-----------|---------|-------|----------
     * Immune    |    0.0   |   0.05    |   0.02  |  0.0  |  0.93
     *-----------|----------|-----------|---------|-------|----------
     */
    vector<vector<double>> transitionProbabilities = {
        {0.62, 0.3, 0.05, 0.0, 0.03}, // healthy
        {0.05, 0.64, 0.1, 0.01, 0.2}, // isolated
        {0.0,  0.1,  0.65, 0.1,  0.15}, // sick
        {0.0,  0.0,  0.0,  1.0,  0.0},  // dead
        {0.0,  0.05, 0.02, 0.0,  0.93}  // immune
    };
    bool isMultiThreading = threadCount > 1;

    printHeaders(
        new int[4]{numberOfRuns, populationMatrixSize, numberOfGenerations, threadCount},
        new bool[2]{applySocialDistanceEffect, generateImage},
        new double[1]{contagionFactor}
    );

    /**
     * Executes the model.
     */
    try
    {
        if(isMultiThreading) {
            unique_ptr<RandomWalkModelParallel> model;
            for(int i = 0; i < numberOfRuns; ++i) {
                model = make_unique<RandomWalkModelParallel>(populationMatrixSize, contagionFactor, applySocialDistanceEffect, threadCount);
                model->setTransitionProbabilities(transitionProbabilities);
                model->parallelSimulation(numberOfGenerations);
                //Print the individuals count based on current state.
                cout << model->getStateCount(State(requestedStateCount)) << endl;
            }
            if(generateImage) {
                model->generateImage();
                cout << "\nImage generated." << endl;
            }
        }
        else {
            unique_ptr<RandomWalkModel> model;
            for(int i = 0; i < numberOfRuns; ++i) {
                model = make_unique<RandomWalkModel>(populationMatrixSize, contagionFactor, applySocialDistanceEffect);
                model->setTransitionProbabilities(transitionProbabilities);
                model->simulation(numberOfGenerations);
                cout << model->getStateCount(State(requestedStateCount)) << endl;
            }
            if(generateImage) {
                model->generateImage();
            }
        }

        return EXIT_SUCCESS;
    }
    catch(invalid_argument& exception)
    {
        cerr << exception.what() << endl;
        abort();
    }
    catch(out_of_range& exception)
    {
        cerr << exception.what() << endl;
        abort();
    }
    catch(exception& exception)
    {
        cerr << exception.what() << endl;
        abort();
    }
}