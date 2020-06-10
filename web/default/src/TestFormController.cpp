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
 * TestFormController.cpp
 *
 *  Created on: Jul 14, 2011
 *      Author: sumeet
 */

#include "TestFormController.h"


TestFormController::TestFormController() {
	// TODO Auto-generated constructor stub

}

TestFormController::~TestFormController() {
	// TODO Auto-generated destructor stub
}

void TestFormController::onSubmit(void* vform,HttpResponse* res)
{
	TestForm* form = (TestForm*)vform;
	res->setHTTPResponseStatus(HTTPResponseStatus::Ok);
	res->addHeaderValue(HttpResponse::ContentType, ContentTypes::CONTENT_TYPE_TEXT_PLAIN);
	res->setContent(form->getTxt()+form->getChe());
	std::cout << form->getTxt()+form->getChe() << "inside TestFormController" << std::endl;
}
