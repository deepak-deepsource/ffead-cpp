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
 * TemplateGenerator.h
 *
 *  Created on: 15-Feb-2013
 *      Author: sumeetc
 */

#ifndef TEMPLATEGENERATOR_H_
#define TEMPLATEGENERATOR_H_
#include "ConfigurationData.h"
#include "StringUtil.h"
#include "fstream"
#include "map"
#include "string"
#include "vector"

class TemplateGenerator {
public:
  TemplateGenerator();
  virtual ~TemplateGenerator();
  static std::string generateTempCd(const std::string &, std::string &,
                                    std::string &, const std::string &app);
  static std::string generateTempCdFast(const std::string &, std::string &,
                                        std::string &, const std::string &app);
  static std::string generateTempCdAll(const std::string &serverRootDirectory);
};

#endif /* TEMPLATEGENERATOR_H_ */
