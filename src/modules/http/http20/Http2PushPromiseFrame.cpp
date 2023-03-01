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
 * Http2PushPromiseFrame.cpp
 *
 *  Created on: 07-Dec-2014
 *      Author: sumeetc
 */

#include "Http2PushPromiseFrame.h"

Http2PushPromiseFrame::Http2PushPromiseFrame() {
  header.type = 5;
  padLength = 0;
  reserved = false;
  promisedStreamId = -1;
}

const std::string &Http2PushPromiseFrame::getHeaderBlockFragment() const {
  return headerBlockFragment;
}

const std::string &Http2PushPromiseFrame::getPadding() const { return padding; }

unsigned char Http2PushPromiseFrame::getPadLength() const { return padLength; }

int Http2PushPromiseFrame::getPromisedStreamId() const {
  return promisedStreamId;
}

Http2PushPromiseFrame::Http2PushPromiseFrame(std::string data,
                                             Http2FrameHeader &aheader) {
  header = aheader;
  header.type = 5;
  padLength = data.at(0);
  reserved = ((data.at(1) >> 7) & 0x01);
  data[1] = data[1] & 0x7F;
  promisedStreamId = (int)CommonUtils::charArrayToULongLong(data.substr(1, 4));
  if (header.isPadded()) {
    headerBlockFragment = data.substr(5, data.length() - padLength - 5);
    padding = data.substr(data.length() - padLength);
  } else {
    headerBlockFragment = data.substr(5);
  }
}

bool Http2PushPromiseFrame::isReserved() const { return reserved; }

Http2PushPromiseFrame::~Http2PushPromiseFrame() {}

std::string Http2PushPromiseFrame::getFrameData() {
  std::string edata;
  edata.push_back(padLength);
  std::string data = CommonUtils::ulonglongTocharArray(promisedStreamId, 4);
  if (reserved)
    data[0] |= 0x01 << 0;
  else
    data[0] &= ~(0x01 << 0);
  edata.append(data);
  edata.append(headerBlockFragment);
  if (header.isPadded()) {
    edata.append(padding);
  }
  return edata;
}
