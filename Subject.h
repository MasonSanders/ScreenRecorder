#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include "Observer.h"
// Subject class, defines subject interface for the observer pattern
class Subject 
{
public:
    virtual ~Subject() = 0;
    virtual void attach(Observer& o) = 0;
    virtual void detach(Observer& o) = 0;
    virtual void notify_observers(int s) = 0;
};

#endif