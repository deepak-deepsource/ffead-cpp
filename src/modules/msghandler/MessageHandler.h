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
 * MessageHandler.h
 *
 *  Created on: Sep 27, 2009
 *      Author: sumeet
 */

#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_
#include "AMEFResources.h"
#include "Constants.h"
#include "Exception.h"
#include "LoggerFactory.h"
#include "MQueue.h"
#include "Message.h"
#include "Server.h"
#include "StringUtil.h"
#include "Thread.h"
#include "algorithm"
#include "string"
#include "vector"
#include <errno.h>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXBUFLEN 1024
#define BACKLOG 10
typedef std::vector<unsigned char> Cont;

class MessageHandler {
  std::string path;
  Logger logger;

public:
  MessageHandler(const std::string &);
  ~MessageHandler() {}
  static void trigger(const std::string &, const std::string &);
  static void stop();
  static MessageHandler *instance;

private:
  Server server;
  bool running;
  static void *service(void *arg);
  static void init(const std::string &);
  Message readMessageFromQ(const std::string &fileName, const bool &erase);
  void writeMessageToQ(const Message &msg, const std::string &fileName);
  bool tempUnSubscribe(const std::string &subs, const std::string &fileName);
  Message readMessageFromT(const std::string &fileName,
                           const std::string &subs);
  void writeMessageToT(const Message &msg, const std::string &fileName);
  void subscribe(const std::string &subs, std::string fileName);
  void unSubscribe(const std::string &subs, std::string fileName);
};

#endif /* MESSAGEHANDLER_H_ */
