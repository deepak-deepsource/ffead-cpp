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
 * Http2DataFrame.cpp
 *
 *  Created on: 07-Dec-2014
 *      Author: sumeetc
 */

#include "Http2DataFrame.h"

Http2DataFrame::Http2DataFrame() {
  header.type = 0;
  padLength = 0;
}

const std::string &Http2DataFrame::getData() const { return data; }

const std::string &Http2DataFrame::getPadding() const { return padding; }

Http2DataFrame::Http2DataFrame(const std::string &data,
                               Http2FrameHeader &aheader) {
  this->header = aheader;
  header.type = 0;
  if (header.isPadded()) {
    padLength = data.at(0);
    this->data = data.substr(1, data.length() - padLength - 1);
    padding = data.substr(data.length() - padLength);
  } else {
    this->data = data;
  }
}

unsigned char Http2DataFrame::getPadLength() const { return padLength; }

Http2DataFrame::~Http2DataFrame() {}

std::string Http2DataFrame::getFrameData() {
  std::string edata;
  if (header.isPadded()) {
    edata.push_back(padLength);
  }
  edata.append(data);
  if (header.isPadded()) {
    edata.append(padding);
  }
  return edata;
}
