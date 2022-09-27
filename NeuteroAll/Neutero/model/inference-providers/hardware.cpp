#include "hardware.h"

// Return true if the hardware component is recognized by one or more inference providers
bool Hardware::isConnected()
{
    BackendCommunicator* b = BackendCommunicator::getInstance();
    return !(b->getAllAvailableHardware()[this->toString()].empty());
}
