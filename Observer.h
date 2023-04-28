#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Observer 
{
public:
    // destructor
    virtual ~Observer() = 0;
    // update responsible for updating subjects based on a notification from one subject
    virtual void update(int s) = 0;
};

#endif