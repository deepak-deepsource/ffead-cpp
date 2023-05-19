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
 * CppServerServer.h
 *
 *  Created on: 1-Nov-2020
 *      Author: sumeetc
 */

#ifndef SRC_SERVER_CppServerServer_H_
#define SRC_SERVER_CppServerServer_H_
#include "ServerInitUtil.h"
#include "fmt/ostream.h"
#include "server/asio/service.h"
#include "server/http/http_server.h"
#include <memory>

class CppServerServer {
public:
  static void runServer(std::string ipaddr, int port,
                        std::vector<std::string> servedAppNames,
                        std::string serverRootDirectory, bool isSSLEnabled);
};

#endif /* SRC_SERVER_CppServerServer_H_ */
