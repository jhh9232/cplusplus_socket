#include "common.h"
#include "httppost.h"

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