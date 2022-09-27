#ifndef SUBJECT_H
#define SUBJECT_H
#include "view/observer.h"

class ISubject
{
public:
    virtual ~ISubject() {}
    virtual void registerObserver(IObserver* observer) = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notify() = 0;

};

#endif // SUBJECT_H
