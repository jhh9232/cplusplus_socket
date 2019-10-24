#include "clock.h"
int main(void)
{
    chrono::system_clock::time_point start = chrono::system_clock::now();
    WAIT(1000);
    chrono::milliseconds mill = chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - start);
    cout << "sleep delay : " << mill.count() << endl;

    return 0;
}