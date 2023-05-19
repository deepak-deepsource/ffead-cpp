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
 * RemoteServiceInt.h
 *
 *  Created on: Mar 27, 2010
 *      Author: sumeet
 */

#ifndef REMOTECOMPONENT_H_
#define REMOTECOMPONENT_H_
#include "BeanContext.h"
#include "GenericObject.h"
#include "vector"

class RemoteComponent : public RemoteComponentInt {
  std::string name;

public:
  RemoteComponent();
  virtual ~RemoteComponent();
  void *invoke(const std::string &name, const std::vector<GenericObject> &args,
               const std::string &rettyp);
  void setBeanName(const std::string &name) { this->name = name; }
};

#endif /* REMOTECOMPONENT_H_ */
