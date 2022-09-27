#ifndef INFERENCECREATOR_H
#define INFERENCECREATOR_H

#include "model/inference.h"

class IInferenceCreator
{
public:
    virtual ~IInferenceCreator() {}
    virtual std::vector<std::shared_ptr<Inference>> buildInferences() = 0;
    virtual bool isReady() = 0;

};

#endif // INFERENCECREATOR_H
