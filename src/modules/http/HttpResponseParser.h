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
 * HttpResponseParser.h
 *
 *  Created on: Nov 27, 2010
 *      Author: sumeet
 */

#ifndef HTTPRESPONSEPARSER_H_
#define HTTPRESPONSEPARSER_H_
#include "CastUtil.h"
#include "HttpResponse.h"
#include "LoggerFactory.h"
#include "StringUtil.h"
#include "fstream"
#include "iostream"
#include "map"
#include "sstream"
#include "stdio.h"
#include "vector"

class HttpResponseParser {
  // TODO - Need to move this content to a MultipartContent List object in the
  // HttpResponse class itself
  std::string content;
  Logger logger;

public:
  std::string getContent();
  HttpResponseParser();
  HttpResponseParser(const std::string &vecstr, HttpResponse &response);
  virtual ~HttpResponseParser();
};

#endif /* HTTPRESPONSEPARSER_H_ */
