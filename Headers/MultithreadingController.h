#ifndef MULTITHREADING_CONTROLLER_H
#define MULTITHREADING_CONTROLLER_H

#include <thread>

using namespace std;

class MultithreadingController {

    public:

        static int getCurrentProcessorAvailableThreads()
        {
            return thread::hardware_concurrency();
        }

        static bool currentProcessorSupportsMultithreading()
        {
            return MultithreadingController::getCurrentProcessorAvailableThreads() > 1;
        }

};

#endif