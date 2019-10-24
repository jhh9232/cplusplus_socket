#include<iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

void showfunc(int i) {  // function:
    cout << i << ", ";
}

int main()
{
    srand(time(NULL));
    int num = rand() % 8 + 1;

    cout << "random : " << num << endl;

    vector<int> vec;
    for(int i=0; i < num; i++)
    {
        vec.push_back(i);
    }
    for_each(vec.begin(), vec.end(), showfunc);
    cout << "\b\b  " << endl;
    cout << "vec size : " << vec.size() << endl;
    
    return 0;
}