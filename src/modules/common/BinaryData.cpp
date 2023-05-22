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
 * BinaryData.cpp
 *
 *  Created on: Jun 14, 2010
 *      Author: sumeet
 */

#include "BinaryData.h"

BinaryData::BinaryData() {}

BinaryData::~BinaryData() {}

void BinaryData::append(const unsigned char &dat) { this->data.push_back(dat); }
void BinaryData::append(unsigned char *dat) {
  // for(int i=0;i<(signed int)strlen(dat);i++)
  //	this->data.push_back(dat[i]);
}
void BinaryData::append(unsigned char *dat, const int &length) {
  for (int i = 0; i < length; i++)
    this->data.push_back(dat[i]);
}
void BinaryData::append(const std::string &dat) {
  for (int i = 0; i < (signed int)dat.length(); i++)
    this->data.push_back(dat[i]);
}
void BinaryData::append(const binaryData &data) {
  for (int i = 0; i < (signed int)data.size(); i++)
    this->data.push_back(data.at(i));
}
binaryData BinaryData::getData() { return this->data; }

std::string BinaryData::serilaize(const BinaryData &data) {
  std::string temp;
  for (int i = 0; i < (signed int)data.data.size(); i++)
    temp += data.data.at(i);
  return temp;
}

BinaryData *BinaryData::unSerilaize(const std::string &temp) {
  BinaryData *binr = new BinaryData;
  binr->data.clear();
  binr->append(temp);
  return binr;
}
