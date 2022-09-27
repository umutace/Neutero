#pragma once
#include <string>
#include "hardware.h"

class NvidiaGPU : public Hardware
{
public:
    NvidiaGPU();
    std::string getModelName() const override;
    std::string toString() const override;
};
