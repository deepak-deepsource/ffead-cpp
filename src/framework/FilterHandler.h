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
 * FilterHandler.h
 *
 *  Created on: Jun 17, 2012
 *      Author: Sumeet
 */

#ifndef FILTERHANDLER_H_
#define FILTERHANDLER_H_
#include "ConfigurationData.h"
#include "Filter.h"
#include "LoggerFactory.h"
#include "Reflector.h"
#include "string"
#include "vector"
#include <dlfcn.h>

typedef ClassInfo (*FunPtr)();

class FilterHandler {
  static bool getFilterForPath(std::string_view cntxtName,
                               const std::string &actUrl,
                               std::vector<std::string> &filters,
                               const std::string &type);

public:
  static void handleIn(HttpRequest *req, const std::string &ext,
                       Reflector &reflector);
  static bool handle(HttpRequest *req, HttpResponse *res,
                     const std::string &ext, Reflector &reflector);
  static void handleOut(HttpRequest *req, HttpResponse *res,
                        const std::string &ext, Reflector &reflector);
  static bool hasFilters(std::string_view cntxtName);
};

#endif /* FILTERHANDLER_H_ */
