#pragma once
#include <string>
#include "hardware.h"

class IntelCPU : public Hardware
{
public:
    IntelCPU();
    std::string getModelName() const override;
    std::string toString() const override;
};
