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
 * ComponentGen.h
 *
 *  Created on: Mar 24, 2010
 *      Author: sumeet
 */

#ifndef COMPONENTGEN_H_
#define COMPONENTGEN_H_
#include "CastUtil.h"
#include "Component.h"
#include "LoggerFactory.h"
#include "PropFileReader.h"
#include "exception"
#include "fstream"
#include "iostream"
#include "string"
#include "vector"

typedef std::vector<std::string> Cont1;
class ComponentGen {
  Logger logger;

public:
  ComponentGen();
  virtual ~ComponentGen();
  std::string generateComponentCU(const std::string &, std::string &,
                                  std::string &, std::string &, std::string &,
                                  const std::string &);
};

#endif /* COMPONENTGEN_H_ */
