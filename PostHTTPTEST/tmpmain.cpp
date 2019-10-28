#include "common.h"
#include "httppost.h"

int main()
{
	if (PostData("10.0.0.95", "/postdata.php", "[{\"seqid\": 53, \"threadID\": 3234212343, \"randomstr\": \"stringmmmm\", \"timestamp\": \"2019-10-25 17:50:33\"}, {\"seqid\": 54, \"threadID\": 3234212343, \"randomstr\": \"stringnnnnn\", \"timestamp\": \"2019-10-25 17:50:33\"}]\r\n"))
	{
		printf("Post Transfer Success\n"); 
	}
	else
	{
		printf("Post Transfer Failed\n");
	}
	return 0;
}
