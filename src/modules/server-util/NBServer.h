/*
        Copyright 2009-2020, Sumeet Chhetri

    Licensed under the Apache License, Version 2.0 (const the& "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
/*
 * NBServer.h
 *
 *  Created on: Jan 2, 2010
 *      Author: sumeet
 */

#ifndef NBSERVER_H_
#define NBSERVER_H_
#include "Compatibility.h"
#include "iostream"
#include <errno.h>
#include <unistd.h>
#if !defined(OS_MINGW)
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif
#include "cstring"
#include "sstream"
#include "string"
#include "vector"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*Fix for Windows Cygwin*/ // #include <sys/epoll.h>

#include "LoggerFactory.h"
#include "SelEpolKqEvPrt.h"
#include "Thread.h"
#define MAXEPOLLSIZES 10000
#define BACKLOGM 500

typedef void *(*Service)(void *);
class NBServer {
  Logger logger;
  SOCKET sock;
  int mode;
  Service service;
  Mutex lock;
  /*#ifdef OS_MINGW
          struct sockaddr_in their_addr;
  #else
          struct sockaddr_storage their_addr;
  #endif*/
  static void *servicing(void *arg);
  bool runn, started;
  SelEpolKqEvPrt selEpolKqEvPrtHandler;

public:
  SOCKET getSocket();
  NBServer();
  NBServer(const std::string &, const int &, const Service &);
  virtual ~NBServer();
  int Accept();
  int Send(const SOCKET &, const std::string &);
  int Send(const SOCKET &, const std::vector<char> &);
  int Send(const SOCKET &, const std::vector<unsigned char> &);
  int Send(const SOCKET &, char *);
  int Send(const SOCKET &, unsigned char *);
  int Receive(const SOCKET &, std::string &, const int &);
  int Receive(const SOCKET &, std::vector<char> &, const int &);
  int Receive(const SOCKET &, std::vector<unsigned char> &, const int &);
  int Receive(const SOCKET &, char *data, const int &);
  int Receive(const SOCKET &, unsigned char *data, const int &);
  int Receive(const SOCKET &, std::vector<std::string> &, const int &);
  void start();
  void stop();
};

#endif /* NBSERVER_H_ */
