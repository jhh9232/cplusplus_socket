#include "common.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>

#define SA      struct sockaddr
#define MAXLINE 8192
#define MAXSUB  200

extern int h_errno;

ssize_t process_http(int , const char *, const char *, const char *);

int PostData(const char *, const char *, const char *);