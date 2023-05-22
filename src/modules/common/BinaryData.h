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
 * BinaryData.h
 *
 *  Created on: Jun 14, 2010
 *      Author: sumeet
 */

#ifndef BINARYDATA_H_
#define BINARYDATA_H_
#include "cstring"
#include "string"
#include "vector"
#include <stdio.h>

typedef std::vector<unsigned char> binaryData;

class BinaryData {
  binaryData data;

public:
  BinaryData();
  virtual ~BinaryData();
  void append(const unsigned char &dat);
  void append(unsigned char *dat);
  void append(unsigned char *dat, const int &);
  void append(const std::string &dat);
  void append(const std::vector<unsigned char> &data);
  binaryData getData();
  static std::string serilaize(const BinaryData &data);
  static BinaryData *unSerilaize(const std::string &temp);
};

#endif /* BINARYDATA_H_ */
