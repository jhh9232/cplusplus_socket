#include "common.h"
int WAITTIME(int millisec)
{
    chrono::system_clock::time_point start = std::chrono::system_clock::now();
    millisec--;
    chrono::milliseconds mill;
    while(true)
    {
        if(chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - start).count() > millisec)
            break;
    }
    return true;
}