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
 * TestMany.h
 *
 *  Created on: Jan 30, 2010
 *      Author: sumeet
 */

#ifndef TESTMANY_H_
#define TESTMANY_H_
#include "Test.h"
#include "YObject.h"
#include "list"
#include "queue"
#include "vector"
class TestMany {
  std::vector<int> vpi;

public:
  Test t;
  int y;
  std::vector<int> vi;
  std::vector<std::string> vs;
  std::vector<double> vd;
  std::vector<long> vl;
  std::vector<bool> vb;
  std::vector<short> vsh;
  std::vector<YObject> vyo;
  std::list<int> li;
  std::queue<short> qsh;

public:
  void setVpi(std::vector<int> vpi) { this->vpi = vpi; }
  std::vector<int> getVpi() { return vpi; }
  TestMany();
  virtual ~TestMany();
};

#endif /* TESTMANY_H_ */
