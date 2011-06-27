/*
 * sock.h
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
#ifndef _SOCK_H_
#define _SOCK_H_


/* input IP addr or Domain name AS text */
int fss_connect(const char *text, int *sockfd);
int fss_readline(int fd, char *buf, size_t size);
int read_msg_head(int fd, char *buf, size_t size);
int fss_write(int fd, size_t size, char *fmt, ...);

#endif
