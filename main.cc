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
    const char* shortOptions = "r:p:g:st:c:o:ihv";
    const option longOptions[] = {
        {"runs", optional_argument, nullptr, 'r'},
        {"population", optional_argument, nullptr, 'p'},
        {"generations", optional_argument, nullptr, 'g'},
        {"social-distance-effect", no_argument, nullptr, 's'},
        {"threads", optional_argument, nullptr, 't'},
        {"contagion-factor", optional_argument, nullptr, 'c'},
        {"output-state", optional_argument, nullptr, 'o'},
        {"image", no_argument, nullptr, 'i'},
        {"version", no_argument, nullptr, 'v'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
    };
    int cliOption;
    while ((cliOption = getopt_long(argc, argv, shortOptions, longOptions, nullptr)) != -1) {
    switch (cliOption) {
        case 'r': {
            if (optarg == nullptr && optind < argc && argv[optind][0] != '-') {
                optarg = argv[optind++];
            }
            numberOfRuns = stoi(optarg);
        } break;
        case 'p': {
            if (optarg == nullptr && optind < argc && argv[optind][0] != '-') {
                optarg = argv[optind++];
            }
            populationMatrixSize = stoi(optarg);
        } break;
        case 'g': {
            if (optarg == nullptr && optind < argc && argv[optind][0] != '-') {
                optarg = argv[optind++];
            }
            numberOfGenerations = stoi(optarg);
        } break;
        case 's': {
            applySocialDistanceEffect = true;
        } break;
        case 't': {
            if (optarg == nullptr && optind < argc && argv[optind][0] != '-') {
                optarg = argv[optind++];
            }
            try {
                int requestedThreadCount = stoi(optarg);
                if (requestedThreadCount < 1) {
                    throw out_of_range("ERROR: THE REQUESTED THREADS COUNT IS LESS THAN 1.");
                }
                threadCount = requestedThreadCount;
            } catch (const out_of_range& exception) {
                cerr << exception.what() << endl;
                exit(EXIT_FAILURE);
            }
        } break;
        case 'c': {
            if (optarg == nullptr && optind < argc && argv[optind][0] != '-') {
                optarg = argv[optind++];
            }
            try {
                const double MIN_CONTAGION_FACTOR = 0.0;
                const double MAX_CONTAGION_FACTOR = 1.0;
                double givenContagionFactor = stod(optarg);
                if (givenContagionFactor < MIN_CONTAGION_FACTOR || givenContagionFactor > MAX_CONTAGION_FACTOR) {
                    throw out_of_range("ERROR: The contagion factor must be between " +
                                       to_string(MIN_CONTAGION_FACTOR) + " and " +
                                       to_string(MAX_CONTAGION_FACTOR) + ".");
                }
                contagionFactor = givenContagionFactor;
            } catch (const invalid_argument&) {
                cerr << "Error: Invalid argument for -c. Expected a double.\n" << endl;
            } catch (const out_of_range& exception) {
                cerr << exception.what() << endl;
                exit(EXIT_FAILURE);
            }
        } break;
        case 'o': {
            if (optarg == nullptr && optind < argc && argv[optind][0] != '-') {
                optarg = argv[optind++];
            }
            try {
                int requestedStateValue = stoi(optarg);
                int stateEnumValuesCount = 5;
                if (requestedStateValue < 0 || requestedStateValue > stateEnumValuesCount) {
                    throw out_of_range("ERROR: Invalid state: " + to_string(requestedStateValue) + ".");
                }
                requestedStateCount = requestedStateValue;
            } catch (const out_of_range& exception) {
                cerr << exception.what() << endl;
                exit(EXIT_FAILURE);
            }
        } break;
        case 'i': {
            generateImage = true;
        } break;
        case 'h':
            printHelp();
            exit(EXIT_SUCCESS);
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
            }
        }
        else {
            unique_ptr<RandomWalkModel> model;
            for(int i = 0; i < numberOfRuns; ++i) {
                model = make_unique<RandomWalkModel>(populationMatrixSize, contagionFactor, applySocialDistanceEffect);
                model->setTransitionProbabilities(transitionProbabilities);
                model->simulation(numberOfGenerations);
                //Print the individuals count based on current state.
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