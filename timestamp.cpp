#include <iostream>
#include <string>
#include <ctime>

using namespace std;

string get_curtime()    //<string>, <ctime> 헤더파일 필요
{
    time_t curr_time;
    struct tm* curr_tm;

    curr_time = time(NULL);
    curr_tm = localtime(&curr_time);\

    string curday = to_string(curr_tm->tm_year + 1900) + "-" + to_string(curr_tm->tm_mon + 1) + "-" + to_string(curr_tm->tm_mday) + " ";
    string curtime = to_string(curr_tm->tm_hour) + ":" + to_string(curr_tm->tm_min) + ":" + to_string(curr_tm->tm_sec);
    curday.append(curtime);
    //insert into test(testtime) values(to_timestamp('2009-01-03 12:33:33');
    
    return curday;
}

int main()
{
    string curtime = get_curtime();
    cout << curtime << endl;

    return 0;
}