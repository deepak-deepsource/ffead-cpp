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
 * RemoteComponent.cpp
 *
 *  Created on: Mar 27, 2010
 *      Author: sumeet
 */

#include "RemoteComponent.h"

RemoteComponent::RemoteComponent() {}

RemoteComponent::~RemoteComponent() {}

void *RemoteComponent::invoke(const std::string &name,
                              const std::vector<GenericObject> &args,
                              const std::string &rettyp) {
  return getContext().invoke(name, args, this->name, rettyp);
}
