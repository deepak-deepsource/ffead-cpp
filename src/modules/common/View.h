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

#ifndef VIEW_H_
#define VIEW_H_
#include "Document.h"
#include <iostream>
#include <sstream>

class View {
  void traverseElement(std::string *ss, Element *element);
  std::string generateAttributes(AttributeList attributes);
  std::string generateCloseTag(std::string tagName);
  std::string generateEndOpenTag();
  std::string generateStartOpenTag(std::string tagName);

public:
  View();
  ~View();
  std::string generateDocument(Document &document);
};
#endif
