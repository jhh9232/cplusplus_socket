#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>

#define SA      struct sockaddr
#define MAXLINE 8192
#define MAXSUB  200

extern int h_errno;

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
		printf("%s", recvline);
	}
	return n;

}
int main(void)
{
	int sockfd;
	struct sockaddr_in servaddr;

	char **pptr;
	//********** You can change. Puy any values here *******
	const char *hname = "10.0.0.95";
	const char *page = "/postdata.php";
	const char *poststr = "{\"seqid\": 15, \"threadID\": 102343533, \"randomstr\": \"stringkkkkk\", \"timestamp\": \"2019-10-25 17:50:33\"}\r\n";
	//*******************************************************

	char str[50];
	struct hostent *hptr;
	if ((hptr = gethostbyname(hname)) == NULL) {
		fprintf(stderr, " gethostbyname error for host: %s: %s",
			hname, hstrerror(h_errno));
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
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80);
	inet_pton(AF_INET, str, &servaddr.sin_addr);

	connect(sockfd, (SA *) & servaddr, sizeof(servaddr));
	process_http(sockfd, hname, page, poststr);
	close(sockfd);
	return true;

}

//g++ -g -o socketpost socketpost.cpp