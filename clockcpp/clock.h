#ifndef CLOCK_H
#define CLOCK_H

// Standard C++ Library 
#include <iostream>
#include <string>
#include <chrono>

// Standard C Library 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace std;

#define null '\0'
#define EMPTY 0
#define FAIL -1

int WAIT(int millisec)
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

#endif