#include "Unwrapper.h"


Unwrapper::Unwrapper()
{
    next = NULL;
}

Unwrapper::~Unwrapper()
{
    if(next)
        delete next;

}