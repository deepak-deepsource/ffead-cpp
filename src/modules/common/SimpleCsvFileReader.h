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
 * SimpleCsvReader.h
 *
 *  Created on: 29-Jan-2013
 *      Author: sumeetc
 */

#ifndef CSVFILEREADER_H_
#define CSVFILEREADER_H_
#include "AppDefines.h"
#include "StringUtil.h"
#include "csv.hpp"
#include "fstream"
#include "iostream"
#include "map"
#include "sstream"
#include "vector"

typedef std::vector<std::vector<std::string>> strVecVec;

class SimpleCsvReader {
  virtual ~SimpleCsvReader() {}

public:
  static strVecVec getRows(const std::string &);
};

class AdvancedCsvReader {
  virtual ~AdvancedCsvReader() {}

public:
  static strVecVec getRows(const std::string &filepath);
};

#endif /* CSVFILEREADER_H_ */
