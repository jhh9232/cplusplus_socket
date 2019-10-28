#include <stdio.h>
#include <curl/curl.h>

//https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response

long get_post(const char *Jdata)
{
	CURL *curl;
	CURLcode res;
	long statLong = 0;
	//In windows, this will init the winsock stuff
	//curl_global_init(CURL_GLOBAL_ALL); // 이 옵션은 thread 메모리 공유에 안전하지 않다.
	
	//get a curl handle
	curl = curl_easy_init();

	struct curl_slist *list = NULL;
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "http://10.0.0.95/postdata.php");           //webserver ip 주소와 포트번호

		list = curl_slist_append(list, "Content-Type: application/json");    // content-type 정의 내용 list에 저장 
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);                    // content-type 설정

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // 값을 false 하면 에러가 떠서 공식 문서 참고함 
    	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L); // 값을 false 하면 에러가 떠서 공식 문서 참고함

		curl_easy_setopt(curl, CURLOPT_POST, 1L);          //POST option
    	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Jdata); //Json형식으로 Jdata를 보냄
		
		//curl_easy_setopt(curl, CURLOPT_NOBODY, 1);	//출력되는 HTTP내용을 숨김
		res = curl_easy_perform(curl);

		// Check for errors
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		else
			curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &statLong);
	}
	// always cleanup
	curl_easy_cleanup(curl); // curl_easy_init 과 세트
	curl_slist_free_all(list);
	//curl_global_cleanup(); // curl_global_init 과 세트

	return statLong;
}

int main()
{
	//{\"seqid\": 1, \"threadID\": 102343533, \"randomstr\": \"string\", \"timestamp\": \"2019-10-25 17:50:33\"}
	long resCode = get_post("{\"seqid\": 1, \"threadID\": 102343533, \"randomstr\": \"string\", \"timestamp\": \"2019-10-25 17:50:33\"}");
	printf("response code : %ld\n", resCode);
	return 0;
}

//g++ -g -o httppost httppost.cpp -lcurl
//makefile에서는 cflags, LIBS 옵션 추가
//CFLAGS=-02 -Wall -Wextra -std=c99 -Iinc -I. -lcurl
//LIBS := -lloragw -lrt -lm -lcurl