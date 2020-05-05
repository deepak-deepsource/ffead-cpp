/*
	Copyright 2009-2020, Sumeet Chhetri

    Licensed under the Apache License, Version 2.0 (the "License");
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
 * RequestReaderHandler.h
 *
 *  Created on: 02-Jan-2015
 *      Author: sumeetc
 */

#ifndef REQUESTREADERHANDLER_H_
#define REQUESTREADERHANDLER_H_
#include "queue"
#include "string"
#include "SelEpolKqEvPrt.h"
#include "SocketInterface.h"
#include "Thread.h"
#include "Mutex.h"
#include "map"
#include "ServiceHandler.h"
#include "LoggerFactory.h"
#include "SSLClient.h"
#include "Client.h"
#include "concurrentqueue.h"
#include <libcuckoo/cuckoohash_map.hh>
#include "atomic"

typedef SocketInterface* (*SocketInterfaceFactory) (SOCKET);

class RequestReaderHandler {
	SelEpolKqEvPrt selector;
	std::atomic<bool> run;
	std::atomic<int> complete;
	bool isMain;
	bool isNotRegisteredListener;
	SOCKET listenerSock;
	ServiceHandler* shi;
	long siIdentifierSeries;
	SocketInterfaceFactory sf;
	std::vector<SocketInterface*> clsdConns;
	bool isActive();
	void addSf(SocketInterface* sf);
	static void* handle(void* inp);
public:
	void start(unsigned int cid);
	void stop(std::string, int, bool);
	RequestReaderHandler(ServiceHandler* shi, const bool& isMain, const SOCKET& listenerSock = INVALID_SOCKET);
	void registerSocketInterfaceFactory(const SocketInterfaceFactory& f);
	virtual ~RequestReaderHandler();
};

#endif /* REQUESTREADERHANDLER_H_ */
