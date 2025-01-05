#ifndef HARDWARE_INFO_CONTROLLER_H
#define HARDWARE_INFO_CONTROLLER_H

#include <thread>

using namespace std;

class HardwareInfoController {

    public:

        static int getCurrentProcessorAvailableThreads()
        {
            return thread::hardware_concurrency();
        }

        static bool currentProcessorSupportsMultithreading()
        {
            return HardwareInfoController::getCurrentProcessorAvailableThreads() > 1;
        }

};

#endif