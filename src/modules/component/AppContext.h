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
 * AppContext.h
 *
 *  Created on: Mar 30, 2010
 *      Author: sumeet
 */

#ifndef APPCONTEXT_H_
#define APPCONTEXT_H_
#include "Exception.h"
#include "map"
#include "string"

class BeanException : public Exception {
public:
  BeanException(const std::string &);
  virtual ~BeanException() throw();
};

class AppContext {
  std::map<std::string, std::string> components;
  static AppContext *_instance;
  AppContext();
  virtual ~AppContext();
  static void init();
  static void clear();

public:
  static bool registerComponent(const std::string &);
  static bool unregisterComponent(const std::string &);
  static void lookup(const std::string &);
};

#endif /* APPCONTEXT_H_ */
