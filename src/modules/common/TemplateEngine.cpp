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
 * TemplateEngine.cpp
 *
 *  Created on: Sep 5, 2009
 *      Author: sumeet
 */

#include "TemplateEngine.h"

TemplateEngine::TemplateEngine() {}

TemplateEngine::~TemplateEngine() {}

std::string TemplateEngine::evaluate(const std::string &fileName,
                                     const StringContext &cntxt) {
  std::string ret, data;
  std::ifstream infile;
  infile.open(fileName.c_str(), std::ios::binary);
  if (infile.is_open()) {
    std::string file;
    int s, en;
    s = fileName.find_last_of("/") + 1;
    en = fileName.find_last_of(".");
    file = fileName.substr(s, en - s);
    while (getline(infile, data)) {
      if (data.find("<import>") != std::string::npos &&
          data.find("</import>") != std::string::npos) {
        int s = data.find("<import>") + 8;
        int e = data.find("</import>");
        data = data.substr(s, e - s);
        std::ifstream inf(data.c_str(), std::ios::binary);
        if (inf) {
          while (getline(inf, data)) {
            ret.append(data + "\n");
          }
        }
      }
      ret.append(data + "\n");
    }
  } else
    ret = fileName;
  StringContext::const_iterator itr;
  for (itr = cntxt.begin(); itr != cntxt.end(); itr++) {
    std::string rep;
    rep = "${" + itr->first + "}";
    StringUtil::replaceAll(ret, rep, itr->second);
  }
  return ret;
}
