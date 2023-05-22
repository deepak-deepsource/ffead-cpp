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
 * ExtHandler.h
 *
 *  Created on: Jun 17, 2012
 *      Author: Sumeet
 */

#ifndef EXTHANDLER_H_
#define EXTHANDLER_H_
#include "ConfigurationData.h"
#include "Constants.h"
#include "Document.h"
#include "DynamicView.h"
#include "FormHandler.h"
#include "FviewHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "LoggerFactory.h"
#include "Reflector.h"
#include "TemplateHandler.h"
#include "View.h"
#include "map"
#include <dlfcn.h>

typedef std::string (*DCPPtr)();
typedef ClassInfo (*FunPtr)();

class ExtHandler {
public:
  static bool handle(HttpRequest *req, HttpResponse *res, void *dlib,
                     void *ddlib, const std::string &ext, Reflector &reflector);
};

#endif /* EXTHANDLER_H_ */
