#include "Wrapper.h"

Wrapper::Wrapper()
{
    next = NULL;
}

Wrapper::~Wrapper()
{
    if(next)
        delete next;
}


void Wrapper::setNext(Wrapper* next) 
{
    this->next = next;
}
