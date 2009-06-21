/**
 * ----------------------------------------------------------------------
 *                         Pirat-libre Public License 1.0
 * ----------------------------------------------------------------------
 * Copyme (k)Akhemi Systems <http://kh.nu>. Some Rights Reserved.
 * 
 * Permission is hereby granted to any intelligent being obtaining a copy
 * of this work, to copy and distribute as-is or modified
 * copies of this work as one sees fit, subject to the following clauses.
 * ----------------------------------------------------------------------
 *      Terms and conditions for Copying, Distribution and Modification
 * ----------------------------------------------------------------------
 *   0. Derivatives may not be distributed as the original.
 *   1. You may not claim this work as your original
 *   2. All warranties, expressed or implied, are disclaimed.
 * ----------------------------------------------------------------------
**/

/*
 * ky_GetURL 2.2.1
 */

#include "ky_geturl.h"

int ky_errno=0;
int ky_h_errno=0;

/*
 * in nds mode tries to automatically connect to wifi
 * returns ky_SUCCESS (0) on success
 * -1 otherwise
 */
int ky_AutoConnectWifi (void)
{
	
	#ifdef DSWIFI9_H
		if (Wifi_AssocStatus() == ASSOCSTATUS_ASSOCIATED) {
			return ky_SUCCESS;
		} else {
			Wifi_AutoConnect();
			for (;;) {
				switch (Wifi_AssocStatus()) {
					case ASSOCSTATUS_ASSOCIATED: return ky_SUCCESS; break;
					case ASSOCSTATUS_CANNOTCONNECT: return -1; break;
					default: break;
				}
			}
		}
	#else
		return ky_SUCCESS; /* assume connected */
	#endif
}

/*
 * fills socket in @lnk
 * returns -1 if network connection fails, ky_AutoConnectWifi
 * returns -2 failed to get a socket (socket=-1); check errno
 * returns -3 if socket setup failed; check errno
 * returns -4 if hostname resolution fails(or malformed ip address); check ky_h_errno
 * returns -5 if failed to connect() to the socket
 * on failure, socket shall be closed and shutdown
 * on success 0 shall be returned and socket is assumed to be a connected
 *   socket, you must disconnect(shutdown+close, ky_Disconnect)
 */
int ky_ConnectEx (sLink *lnk, eSocketMode mode, unsigned int timeout)
{
	int i;
	struct hostent *he;
	struct sockaddr_in sa;
	struct timeval tv;
	
	ky_Disconnect(lnk);
	
	if (ky_AutoConnectWifi() != ky_SUCCESS) return -1;
	
	errno = 0;
	lnk->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (lnk->socket == -1) return -2;
	
	tv.tv_usec = 0;
	tv.tv_sec = timeout;
	
	errno = 0;
	if (setsockopt(lnk->socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
		ky_Disconnect(lnk);
		return -3;
	}
	
	/* the docs say the behaviour of gethostbyname() when passed an ip
	 * is unspecified, it seems to work on linux, but not tested anywhere else
	 * so use, gethostbyaddr first
	 * ----
	 * dswifi, checks for an ip therefore there is no gethostbyaddr
	 */
	#ifdef DSWIFI9_H
		he = gethostbyname(lnk->host);
	#else
		if (inet_aton(lnk->host, &sa.sin_addr) != 0) {
			he = gethostbyaddr(&sa.sin_addr, sizeof(struct in_addr), AF_INET);
		} else {
			he = gethostbyname(lnk->host);
		}
	#endif
	ky_h_errno = h_errno;
	if (he == NULL) {
		ky_Disconnect(lnk);
		return -4;
	}
	memcpy(&sa.sin_addr, (struct in_addr**)*he->h_addr_list, sizeof(struct in_addr));
	
	sa.sin_family = AF_INET;
	sa.sin_port = htons(lnk->port);
	
	errno = 0;
	i = connect(lnk->socket, (struct sockaddr*)&sa, sizeof(sa));
	if (i != 0) {
		ky_Disconnect(lnk);
		return -5;
	}
	
	if (mode == SOCK_NON_BLOCKING) {
		i = 1;
		(void)ioctl(lnk->socket, FIONBIO, &i);
	}
	
	return ky_SUCCESS;
}

/*
 * see ky_ConnectEx
 * calls it with default: SOCK_BLOCKING and ky_TIMEOUT
 */
int ky_Connect (sLink *lnk)
{
	return ky_ConnectEx(lnk, SOCK_BLOCKING, ky_TIMEOUT);
}

/*
 * shutdown: SHUT_RDWR socket, then close it
 * and set socket to un-sane: -1
 * if socket is not sane, there is no effect
 */
void ky_Disconnect (sLink *lnk)
{
	if (lnk->socket >= 0) {
		(void)shutdown(lnk->socket, SHUT_RDWR);
		(void)closesocket(lnk->socket);
		lnk->socket = -1;
	}
}

/*
 * a version of strndup .. 'copy' means malloc()'d, so YOU are responsible for free()ing
 * returns a NUL(\0) terminated copy of the first n chars of str
 */
char* ky_Strndup (const char *buf, size_t n)
{
	char *s=NULL;
	
	if (buf) {
		s = (char*)malloc(n + 1);
		if (s) {
			memcpy(s, buf, n);
			s[n] = '\0';
		}
	}
	return s;
}

/*
 * splits @url and fills @lnk with host, port and query
 * socket is set to -1
 * returns 0 on success
 *   if :port is missing, port is set to ky_DEFAULT_PORT
 *   host is newly allocated memory
 * returns -1 if url is NULL or empty
 * otherwise
 * query is a pointer to the start of the query field of the url scheme
 *   if there was no query [host:port]query, query is "/"
 * returns -2 if port is detected (: is present) but invalid
 *   a valid port is base a base 10 number >= 1
 *   no error checking is done, url :80 will not fail
 *   likewise, url*:80.+/, where port does not immediately precede query
 *   will not fail, the url is assumed to be well formed (port=0)
 * returns -3 if memory allocation of host fails (host=NULL)
 * on failure != -1, the contents of lnk are indeterminate
 */
int ky_InitLink (sLink *lnk, const char *url)
{
	char *port, *query, *host;
	
	lnk->socket = -1;
	lnk->port = 0;
	lnk->host = NULL;
	lnk->query = NULL;
	
	if (url == NULL || *url == '\0') return -1;
	
	host = strstr(url, "://"); /* only support http currently */
	if (host) host += 3;
	else host = (char*)url;
	host = ky_Strndup(host, strlen(host));
	if (host == NULL) return -2;
	lnk->host = host;
	
	query = strchr(host, '/');
	if (query) {
		*query = '\0';
		lnk->query = query + 1;
	} else {
		lnk->query = NULL;
	}
	
	port = strchr(host, ':');
	if (port) { /* port must come before query */
		*port = '\0';
		lnk->port = strtoul((port + 1), NULL, 10);
		if (lnk->port == 0) lnk->port = ky_PORT;
	} else {
		lnk->port = ky_PORT;
	}
	
	return ky_SUCCESS;
}

void ky_FreeLink (sLink *lnk)
{
	ky_Disconnect(lnk);
	lnk->port = 0;
	lnk->query = NULL;
	ky_Free(lnk->host);
}

void ky_FreeBuffer (sBuffer *bfr)
{
	ky_Free(bfr->buf);
	ky_InitBuffer(bfr);
}

void ky_InitBuffer (sBuffer *bfr)
{
	bfr->buf = NULL;
	bfr->length = 0;
	bfr->buffer = NULL;
}

/*
 * returns 0/false 1/true depending on
 * socket < 0, port < 1, host is NULL or \0 ..
 *   query is not checked
 */
int ky_SaneLink (const sLink *lnk)
{
	return (lnk->socket >= 0 && lnk->port > 0 && lnk->host && *lnk->host);
}

/*
 * already connected @socket, not checked for sanity
 * @buf buffer to store up to @len bchars
 * @recvd, to store the amount of data recieved
 * returns 0 on completion,
 * return 1 if a timeout occurred; ky_RETRY
 * returns -1 otherwise
 * !the @buf(0..len) is NOT zeroed
 */
int ky_Get (int socket, bchar *buf, size_t len, ssize_t *recvd)
{
	ssize_t size;
	signed char retry=0;
	
	*recvd = 0;
	for (;;) {
		errno = 0;
		size = recv(socket, buf + *recvd, len, 0);
		
		if (size == 0) {
			return ky_SUCCESS;
		} else if (size == -1) {
			if (((errno != EAGAIN) && (errno != EWOULDBLOCK)) || (++retry > ky_RETRY)) {
				return -1;
			}
		} else {
			len -= size;
			*recvd += size;
		}
	}
}

/*
 * send query to host via socket
 * if @query is null, @lnk->query is used instead ..
 * returns 0 on success
 * returns -1 if lnk is not sane, see ky_SaneLinkQuery
 * (lnk->query is not changed, @query is internally guided by that specification)
 * returns -2 on memory allocation failure for request(maybe host/query is too big)
 * returns -3 if send()ing fails
 */
int ky_Query (sLink *lnk, const char *query)
{
	/* HTTP/1.1 connect appears noticeably slower than HTTP/1.0,
	 * and we don't use any special features of HTTP/1.1
	 */
	#define _req "GET /%s HTTP/1.0\r\nHost: %s\r\n"ky_USERAGENT"Accept: */*\r\n\r\n"
	size_t size;
	char *request, _request[ky_CHUNK_SIZE+1];
	if (!ky_SaneLink(lnk)) return -1;
	
	size = strlen(lnk->host) + (sizeof(_req) - 4);
	
	if (query == NULL) query = lnk->query;
	if (query) size += strlen(query);
	else query = "";
	
	if (size > ky_CHUNK_SIZE) {
		request = (char*)malloc(size);
		if (request == NULL) return -2;
	} else {
		request = _request;
	}
	(void)snprintf(request, size, _req, query, lnk->host);
	
	errno = 0;
	if ((send(lnk->socket, request, size, 0) == -1)
	  || (ky_Connect(lnk) != ky_SUCCESS) /* reconnect */
	  || (send(lnk->socket, request, size, 0) == -1) /* retry request */
	) {
		if (size > ky_CHUNK_SIZE) ky_Free(request);
		ky_Disconnect(lnk);
		return -3;
	} else {
		if (size > ky_CHUNK_SIZE) ky_Free(request);
		return ky_SUCCESS;
	}
}

/*
 * makes send lnk->query, see ky_Query
 */
int ky_QueryLink (sLink *lnk)
{
	return ky_Query(lnk, NULL);
}


/*
 * tries to guess the required buffer length, reducing reallocs
 * uses Content-Length and \r\n\r\n
 * note: this function never fails, it always returns >= 0
 */
size_t ky_PredictBufferLength (char *buf)
{
	char *sp, *ep=buf;
	size_t length=0;
	
	if (!buf) return 0;
	sp = strstr(buf, "Content-Length:");
	if (sp) {
		sp += 15;
		length = strtoul(sp, &ep, 10);
	}
	sp = strstr(ep, "\r\n\r\n");
	if (sp)length += sp - buf;
	return length;
}

/*
 * @buf is newly allocated when return is >= 0, shoudl be NULL otherwise
 * check ky_errno
 * returns -1 if query fails
 * returns -2 if initial get fails
 * returns -3 if memory allocation fails
 * returns 1 if content-length is detected, and get fails
 * returns 2 if *recvd is not as expected
 * returns 3 if fails to Get any remaining data
 * returns 4 if any required buffer resize fails(realloc)
 * 
 * in any event errno is set to the last error status
 * it's not cleared so if you experience weirdness
 * even if the return is > 0 check errno
 * errors are propagated so check ky_errno and don't forget *buf must
 * be freed only if >= 0 is returned
 */
int ky_GetUrlEx (sLink *lnk, sBuffer *bfr)
{
	ssize_t recvd=0, allocd=0, len=0;
	bchar *tmp, chunk[ky_CHUNK_SIZE+1];
	int retry=0;
	
	ky_InitBuffer(bfr);
	
	ky_errno = ky_Query(lnk, NULL);
	if (ky_errno != 0) return -1;
	
	memset(chunk, '\0', sizeof(chunk));
	ky_errno = ky_Get(lnk->socket, chunk, ky_CHUNK_SIZE, &recvd);
	if (ky_errno < 0) return -2;
	
	allocd = ky_PredictBufferLength((char*)chunk);
	if (recvd < 0) {
		recvd = 0;
		if (allocd == 0) allocd = ky_CHUNK_SIZE;
	} else if (allocd < recvd) {
		allocd = recvd;
	}
	
	bfr->buf = (bchar*)malloc(allocd + 1);
	if (bfr->buf == NULL) return -3;
	bfr->buf[0] = '\0'; /* recvd might be 0 */
	bfr->buf[allocd] = '\0';
	
	memcpy(bfr->buf, chunk, recvd);
	bfr->length = recvd;
	
	for (;;) {
		ky_errno = ky_Get(lnk->socket, chunk, ky_CHUNK_SIZE, &recvd);
		
		if (recvd <= 0) {
			bfr->buf[bfr->length] = '\0';
			tmp = (bchar*)strstr((char*)bfr->buf, "\r\n\r\n");
			if (tmp) {
				tmp += 4;
				bfr->length -= (tmp - bfr->buf);
				bfr->buffer = tmp;
			} else {
				bfr->buffer = bfr->buf;
			}
			
			if (recvd == 0) return ky_SUCCESS;
			else if (++retry > ky_RETRY) return 3;
		} else {
			len = bfr->length + recvd;
			if (len > allocd) {
				tmp = (bchar*)realloc(bfr->buf, len + 1);
				if (tmp == NULL) return 4;
				bfr->buf = tmp;
				allocd = len;
			}
			memcpy((bfr->buf + bfr->length), chunk, (size_t)recvd);
			bfr->length = len;
		}
	}
}

/*
 * helper for ky_GetUrlEx
 * returns a value <= -31 if failed to init lnk, -30 + ky_InitLink() error
 * returns a value <= -21 if failed to init lnk, -20 + ky_Connect() error
 * otherwise see ky_GetUrlEx()
 * ky_errno may be set
 */
int ky_GetUrl (const char *url, sBuffer *bfr)
{
	sLink lnk;
	int err;
	
	ky_InitBuffer(bfr);
	
	err = ky_InitLink(&lnk, url);
	if (err != ky_SUCCESS) {
		ky_FreeLink(&lnk);
		return err - 30;
	}
	
	err = ky_Connect(&lnk);
	if (err != ky_SUCCESS) {
		ky_FreeLink(&lnk);
		return err - 20;
	}
	err = ky_GetUrlEx(&lnk, bfr);
	ky_FreeLink(&lnk); /* FreeLink calls Disconnect */
	return err;
}
