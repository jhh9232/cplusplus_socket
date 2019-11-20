#include "../confheader/common.h"
#include "../confheader/get_parse_data.h"
#include "../confheader/semaphoreTask.h"

string createString(unsigned int* randstate)
{
	int stringlen = rand_r(randstate) % 20 + 1;
	string res = "";
	for (int i = 0; i < stringlen; i++)
	{
		char numstr = null;
		int bigsmall = rand_r(randstate) % 2;
		if (bigsmall == 0)   //대문자
		{
			numstr = (char)(rand_r(randstate) % 26 + 65);
		}
		else    //소문자
		{
			numstr = (char)(rand_r(randstate) % 26 + 97);
		}
		string createstr(1, numstr);
		res.append(createstr);
	}
	return res;
}

string get_curtime()    //<string>, <ctime> 헤더파일 필요
{
	time_t curr_time;
	struct tm* curr_tm;

	curr_time = time(NULL);
	curr_tm = localtime(&curr_time);

	string curday = to_string(curr_tm->tm_year + 1900) + "-" + to_string(curr_tm->tm_mon + 1) + "-" + to_string(curr_tm->tm_mday) + " ";
	string curtime = to_string(curr_tm->tm_hour) + ":" + to_string(curr_tm->tm_min) + ":" + to_string(curr_tm->tm_sec);
	curday.append(curtime);
	//insert into test(testtime) values(to_timestamp('2009-01-03 12:33:33');

	return curday;
}

string toJSON(queue<JsonDatas>* sockDatas)
{
	string JsonStr = "[";
	const int append = 0;
	const int start = 1;
	const int end = 2;
	while (!sockDatas->empty())
	{
		string idname = "seqid";
		string tmp = strtoJson(idname, to_string(sockDatas->front().id), true, start);
		JsonStr.append(tmp);
		tmp = strtoJson("threadID", to_string(sockDatas->front().threadID), true, append);
		JsonStr.append(tmp);
		tmp = strtoJson("randomstr", sockDatas->front().randomstr, false, append);
		JsonStr.append(tmp);
		tmp = strtoJson("timestamp", sockDatas->front().timestamp, false, end);
		JsonStr.append(tmp);
		JsonStr.append(", ");
		sockDatas->pop();
	}
	JsonStr.erase(JsonStr.length() - 2, 2);
	JsonStr.append("]");
	return JsonStr;
}

//기존 JsonStr, key, value, 정수?문자?, 시작{, 추가, 끝}
string strtoJson(string key, string value, int isint, int status)
{
	string JsonStr = "";
	//append
	if (status == 0)
	{
		JsonStr.append(appJson(key, value, isint)); //..., "key": "value"
		JsonStr.append(", ");   //..., "key": "value", 
	}
	//start
	else if (status == 1)
	{
		JsonStr.append("{");  //{
		JsonStr.append(appJson(key, value, isint)); //{"key": "value"
		JsonStr.append(", ");   //{"key": "value", 
	}
	//end
	else if (status == 2)
	{
		JsonStr.append(appJson(key, value, isint)); //..., "key": "value"
		JsonStr.append("}");    //..., "key": "value"}
	}
	return JsonStr;
}
string appJson(string key, string value, int isint)
{
	string JsonStr = "\"";  //"
	JsonStr.append(key);    //"seqid
	if (isint == 1)
	{
		JsonStr.append("\": "); //"seqid": 
		JsonStr.append(value);  //"seqid": 0
	}
	else
	{
		JsonStr.append("\": \"");   //"seqid": "
		JsonStr.append(value);  //"seqid": "a
		JsonStr.append("\""); //"seqid": "a"
	}
	return JsonStr;
}