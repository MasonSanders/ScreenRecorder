#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include "Observer.h"
// Subject class, defines subject interface for the observer pattern
class Subject 
{
public:
    // Destructor
    virtual ~Subject() = 0;
    // attach responsible for attaching an observer to a subject
    virtual void attach(Observer& o) = 0;
    // detach removes an observer from a subject
    virtual void detach(Observer& o) = 0;
    // notify, tells all observers that the state of a subject has changed
    virtual void notify_observers(int s) = 0;
};

#endif