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
 * Method.cpp
 *
 *  Created on: Oct 10, 2009
 *      Author: sumeet
 */

#include "Method.h"

Method::Method() {
  isstat = false;
  f = NULL;
}

GetMeth Method::getIns() const { return f; }

Method::~Method() {}

const std::string &Method::getAccessSpecifier() const {
  return accessSpecifier;
}

void Method::setAccessSpecifier(const std::string &accessSpecifier) {
  this->accessSpecifier = accessSpecifier;
}

const std::string &Method::getMethodName() const { return methodName; }

void Method::setMethodName(const std::string &methodName) {
  this->methodName = methodName;
}

const std::string &Method::getReturnType() const { return returnType; }

void Method::setReturnType(const std::string &returnType) {
  this->returnType = returnType;
}

args Method::getArgumentTypes() const { return argumentTypes; }

void Method::setArgumentTypes(const args &argumentTypes) {
  this->argumentTypes = argumentTypes;
}

int Method::getArgNum() const { return this->argumentTypes.size(); }

void Method::clear() {
  accessSpecifier = "";
  methodName = "";
  returnType = "";
  argumentTypes.clear();
}

bool Method::isStatic() { return isstat; }

bool Method::isVirtual() { return isV; }

bool Method::isPureVirtual() { return isPV; }

void Method::setFlags(const bool &isStatic, const bool &isV, const bool &isPV) {
  this->isstat = isStatic;
  this->isV = isV;
  this->isPV = isPV;
}

const std::string &Method::getRefName() const { return refName; }

void Method::setRefName(const std::string &refName) { this->refName = refName; }
