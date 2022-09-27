#pragma once
#include <string>
#include "hardware.h"

class IntelMovidius : public Hardware
{
public:
    IntelMovidius();
    std::string getModelName() const override;
    std::string toString() const override;
};
