#include "../confheader/common.h"
#include "../confheader/httppost.h"


//프로토콜 체계를 설정하는 부분은 PF로 시작하는 상수를 사용하고, 주소 체계를 설정하는 부분은 AF로 시작하는 상수를  사용하는 것이 좋습니다.
ssize_t process_http(int sockfd, const char *host, const char *page, const char *poststr)
{
	char sendline[MAXLINE + 1], recvline[MAXLINE + 1];
	ssize_t n;
	snprintf(sendline, MAXLINE,
		 "POST %s HTTP/1.0\r\n"
		 "Host: %s\r\n"
		 "Content-type: application/json\r\n"
		 "Content-length: %d\r\n\r\n"
		 "%s", page, host, (int)strlen(poststr), poststr);

	write(sockfd, sendline, strlen(sendline)+1);
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = '\0';
		//printf("%s", recvline);
	}
	return n;
}

int PostData(const char *hname, const char *page, const char *poststr)
{
	int sockfd;
	struct sockaddr_in servaddr;

	char **pptr;
	//*******************************************************

	struct hostent *hptr;
	if ((hptr = gethostbyname(hname)) == NULL) {
		fprintf(stderr, " gethostbyname error for host: %s",
			hname);
		return false;
	}
	//printf("hostname: %s\n", hptr->h_name);
	if (hptr->h_addrtype == AF_INET
	    && (pptr = hptr->h_addr_list) != NULL) {
		// printf("address: %s\n",
		//        inet_ntop(hptr->h_addrtype, *pptr, str,
		// 		 sizeof(str)));
	} else {
		fprintf(stderr, "Error call inet_ntop \n");
		return false;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80);
	inet_pton(AF_INET, hname, &servaddr.sin_addr);

	connect(sockfd, (SA *) & servaddr, sizeof(servaddr));
	process_http(sockfd, hname, page, poststr);
	close(sockfd);
	return true;
}