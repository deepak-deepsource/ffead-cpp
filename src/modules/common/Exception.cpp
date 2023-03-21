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
 * Exception.cpp
 *
 *  Created on: Sep 27, 2009
 *      Author: sumeet
 */

#include "Exception.h"

Exception::Exception() {}

Exception::Exception(const std::string &message) { this->message = message; }

Exception::~Exception() throw() {}

const std::string &Exception::getMessage() const { return message; }

void Exception::setMessage(const std::string &message) {
  this->message = message;
}

const char *Exception::what() const throw() {
  return this->getMessage().c_str();
}
