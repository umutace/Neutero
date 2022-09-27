#ifndef IPARAMETERCONFIGURATOR_H
#define IPARAMETERCONFIGURATOR_H

#include "controller/inferenceRunner/inferencemanager.h"


class IParameterConfigurator
{
protected:
    bool ready = false;
public:
    virtual ~IParameterConfigurator() {}
    template<class object>
    object getParameter() {}
    template<class object>
    bool setParameter(object param) {}
    bool isReady() {return ready;}
};

#endif // IPARAMETERCONFIGURATOR_H
