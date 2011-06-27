/*
 * sock.c
 *
 * network manipulate functions
 *
 * Copyright (c) 2010, 2011 lxd <i@lxd.me>
 * 
 * This file is part of File Synchronization System(fss).
 *
 * fss is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, 
 * (at your option) any later version.
 *
 * fss is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fss.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fss.h"
#include "sock.h"
#include <stdarg.h>

extern int errno;

static int socket_fd;

int fss_connect(const char *text, int *sockfd)
{
  struct addrinfo hints, *result;
  int rv;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(text, PORT_STR, &hints, &result)) != 0) {
    fprintf(stderr,
	    "@fss_connect(): getaddrinfo() %s, %s fails: %s\n",
	    text, PORT_STR, gai_strerror(rv));
    return 1;
  }

  if ((*sockfd = socket(result->ai_family, result->ai_socktype,
			result->ai_protocol)) < 0) {
    perror("@fss_connect(): socket() fails");
    return 1;
  }

  socket_fd = *sockfd;
  
  /* only deal with the first addr */
  if ((rv = connect(*sockfd, result->ai_addr, result->ai_addrlen)) < 0) {
    perror("@fss_connect(): connect() fails");
    return 1;
  }

  freeaddrinfo(result);
  return 0;
}


/* This function comes from UNP v3 with a little modification
 * without appending LF to buf */
int fss_readline(int fd, char *buf, size_t size)
{
  char c;
  char *ptr;
  size_t i;
  int rv;

  ptr = buf;
  for(i = 1; i < size; i++) {
    if ((rv = read(fd, &c, 1)) == 1) {
      if (c == '\n')
	break;
      *ptr++ = c;
      
    } else if (rv == 0) {
      *ptr = 0;
      return 0;
      
    } else {
      if (errno == EINTR) {
	i--;
	continue;
      }

      perror("@fss_readline(): read failed");
      return 1;
    }
    
  }// end for(..

  *ptr = 0;
  return 0;
}


/* There are 2 LF between msg head and body, this function strip second
 * LF and so only append one LF to buf */
int read_msg_head(int fd, char *buf, size_t size)
{
  char c, d;
  char *ptr;
  size_t i;
  int rv;

  ptr = buf;
  c = d = 0;
  for(i = 1; i < size; i++) {
    c = d;
    if ((rv = read(fd, &d, 1)) == 1) {
      if (c == '\n' && d == '\n')
	break;
      *ptr++ = d;
      
    } else if (rv == 0) {
      *ptr = 0;
      return 0;
      
    } else {
      if (errno == EINTR) {
	d = c;
	i--;
	continue;
      }

      perror("@fss_readline(): read failed");
      return 1;
    }
    
  }// end for(..

  *ptr = 0;
  return 0;
}


int fss_write(int fd, size_t size, char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  char msg[size];
  vsnprintf(msg, size, fmt, ap);

  if(write(fd, msg, strlen(msg)) < 0) {
    perror("@fss_send(): write() failed");
    return 1;
  }

  va_end(ap);

  return 0;
}
