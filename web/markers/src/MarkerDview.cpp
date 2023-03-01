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
 * MarkerDview.cpp
 *
 *  Created on: Sep 12, 2009
 *      Author: sumeet
 */

#include "MarkerDview.h"
#include "SimpleXmlParser.h"

MarkerDview::MarkerDview() {
  // TODO Auto-generated constructor stub
}

MarkerDview::~MarkerDview() {
  // TODO Auto-generated destructor stub
}

void MarkerDview::getDocument(Document *doc) {
  std::string xml =
      "<html><head><script src=\"1.js\"></script></head><body><input "
      "type=\"text\"/><input type=\"submit\"/></body></html>";
  SimpleXmlParser parser("Parser");
  parser.parse(xml, *doc);
}
