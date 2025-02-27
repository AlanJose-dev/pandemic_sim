#ifndef PROGRAM_INFO_VIWER_H
#define PROGRAM_INFO_VIWER_H

#include <iostream>
#include "State.h"
#include "MultithreadingController.h"

using namespace std;

const char VERSION[] = "0.4.7";

inline const char * const boolToString(bool boolean)
{
  return boolean ? "Yes" : "No";
}

void printASCIIArt()
{
    cout << "__________                    .___             .__              _________.__         " << endl;
    cout << "\\______   \\_____    ____    __| _/____   _____ |__| ____       /   _____/|__| _____  " << endl;
    cout << " |     ___/\\__  \\  /    \\  / __ |/ __ \\ /     \\|  |/ ___\\      \\_____  \\ |  |/     \\ " << endl;
    cout << " |    |     / __ \\|   |  \\/ /_/ \\  ___/|  Y Y  \\  \\  \\___      /        \\|  |  Y Y  \\" << endl;
    cout << " |____|    (____  /___|  /\\____ |\\___  >__|_|  /__|\\___  >____/_______  /|__|__|_|  /" << endl;
    cout << "                \\/     \\/      \\/    \\/      \\/        \\/_____/       \\/          \\/ " << endl;
}

/**
 * ACII art via: https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Pandemic_Sim
 */
void printHeaders(int intParams[4], bool boolParams[2], double doubleParams[1])
{
    cout << "-------------------------------------------------------------------------------------------" << endl;
    printASCIIArt();
    cout << "-- Number of runs: " << intParams[0] << endl;
    cout << "-- Population matrix size: " << intParams[1] << " (" << intParams[1] * intParams[1] << " individuals)" << endl;
    cout << "-- Number of generations: " << intParams[2] << endl;
    cout << "-- Disease contagion factor: " << doubleParams[0] << endl;
    cout << "-- Social distance effect applyied: " << boolToString(boolParams[0]) << endl;
    cout << "-- Threads: " << intParams[3] << endl;
    cout << "-- Generate visual example image on finish: " << boolToString(boolParams[1]) << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    delete[] intParams;
    delete[] boolParams;
    delete[] doubleParams;
}

void printHelp()
{
    printASCIIArt();
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "Usage: simulator [-v | --version] [-h | --help] [-r | --runs <value>] [-p | --population <value>]" << endl;
    cout << "                 [-g | --generations <value>] [-s | --social-distance-effect] [-t | --threads <value>]" << endl;
    cout << "                 [-c | --contagion-factor <value>] [-o | --output-state <value>] [-i | --image]" << endl;
    cout << "\n" << endl;
    cout << "Multithreading is available : " << boolToString(MultithreadingController::currentProcessorSupportsMultithreading()) << "." << endl;
    cout << "CPU Threads available       : " << MultithreadingController::getCurrentProcessorAvailableThreads() << "." << endl;
    cout << "\n" << endl;
    cout << "Individual states           : " << endl;
    cout << "Healthy                     : " << "0" << endl;
    cout << "Isolated                    : " << "1" << endl;
    cout << "Sick                        : " << "2" << endl;
    cout << "Dead                        : " << "3" << endl;
    cout << "Immune                      : " << "4" << endl;
    cout << "\n" << endl;
    cout << "Parameter descriptions        :" << endl;
    cout << "-v | --version                :       Show the program version." << endl;
    cout << "-h | --help                   :       Show this message." << endl;
    cout << "-r | --runs                   :       Define how many times the model will be executed, determining the number of results (integer)." << endl;
    cout << "-p | --population             :       Define the population matrix side length. Use the square root, e.g., 100 corresponds to 10,000 (integer)." << endl;
    cout << "-g | --generations            :       Specify the number of generations in weeks (integer)." << endl;
    cout << "-s | --social-distance-effect :       Run the simulations with the social distancing/lockdown effect applied, reducing the disease contagion factor." << endl;
    cout << "-t | --threads                :       Run the simulations with a multi-threaded profile. Specifies the number of threads the program may use. The maximum value is the number of threads available on the current processor (integer)." << endl;
    cout << "-c | --contagion-factor       :       Defines the disease contagion factor, minimum 0.1, maximum 1 (double)." << endl;
    cout << "-o | --output-state           :       Defines the state for which you want to obtain the number of affected individuals (integer)." << endl;
    cout << "-i | --image                  :       Generate a visual disease spread example as a .png image." << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;
    cout << "Default params: r(100), p(100), p(10), c(0.5), o(3), s(false), t(1), i(false)" << endl;
}

void printVersion()
{
    printASCIIArt();
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "Pandemic Sim version " << VERSION << endl;
    cout << "Author: Alan Jose <alanjsdelima@gmail.com>"<< endl;
    cout << "Public Domain Code, feel free to use, modify and redistribute it."<< endl;
}

#endif