#ifndef GET_PARSE_DATA_H
#define GET_PARSE_DATA_H

#include "../confheader/semaphoreTask.h"
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <queue>

string createString(unsigned int*);
string get_curtime();
string toJSON(queue<JsonDatas>*);
string strtoJson(string, string, int, int);
string appJson(string, string, int);

#endif