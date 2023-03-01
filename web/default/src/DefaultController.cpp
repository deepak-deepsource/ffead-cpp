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
 * DefaultController.cpp
 *
 *  Created on: Aug 20, 2009
 *      Author: sumeet
 */

#include "DefaultController.h"
#include "DataSourceManager.h"
#include "Test.h"
#include "Test2.h"
#include "Test3.h"
#include "Test4.h"

DefaultController::DefaultController() {
  // TODO Auto-generated constructor stub
}

DefaultController::~DefaultController() {
  // TODO Auto-generated destructor stub
}

bool DefaultController::service(HttpRequest *req, HttpResponse *res) {
  DataSourceInterface *sqli = DataSourceManager::getImpl();
  std::vector<Test2> tec = sqli->getAll<Test2>();
  std::cout << "\n Sdorm Query fetched " << tec.size() << " rows\n"
            << std::flush;
  std::vector<Test3> tec1 = sqli->getAll<Test3>();
  std::cout << "\n Sdorm Query fetched " << tec1.size() << " rows\n"
            << std::flush;
  std::vector<Test4> tec2 = sqli->getAll<Test4>();
  std::cout << "\n Sprint Query fetched " << tec2.size() << " rows\n"
            << std::flush;
  DataSourceManager::cleanImpl(sqli);
  return true;
}
