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
 * HttpClient.h
 *
 *  Created on: 03-Nov-2018
 *      Author: sumeet
 */

// https://github.com/libcpr/cpr
// https://github.com/seznam/elasticlient
// https://github.com/sewenew/redis-plus-plus#install-redis-plus-plus

#ifndef SRC_MODULES_CLIENT_UTIL_HTTP_HTTPCLIENT_H_
#define SRC_MODULES_CLIENT_UTIL_HTTP_HTTPCLIENT_H_

#include "AppDefines.h"
#ifdef HAVE_CURLLIB
#include <curl/curl.h>
#endif
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <cstdlib>
#include <map>
#include <string>

class HttpClient {
  std::string baseUrl;
#ifdef HAVE_CURLLIB
  CURL *_h;
#endif
  static size_t onContent(void *res, size_t len, size_t mb, void *data);
  static size_t onHeaders(void *res, size_t len, size_t mb, void *data);
  static size_t readContent(char *ptr, size_t size, size_t nmemb,
                            void *userdata);

public:
  static void init();
  static void cleanup();
  HttpClient();
  HttpClient &withBaseUrl(std::string baseUrl);
  HttpClient(std::string baseUrl);
  virtual ~HttpClient();

  void execute(HttpRequest *request, HttpResponse *response, propMap &props);
};

#endif /* SRC_MODULES_CLIENT_UTIL_HTTP_HTTPCLIENT_H_ */
