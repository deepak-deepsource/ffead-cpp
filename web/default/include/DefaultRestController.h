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
 * DefaultIOFilter.h
 *
 *  Created on: Apr 7, 2011
 *      Author: sumeet
 */

#ifndef DEFAULTRESTCONTROLLER_H_
#define DEFAULTRESTCONTROLLER_H_

#include "CastUtil.h"
#include "ConfigurationData.h"
#include "TestMany.h"
#include "TestSTLs.h"
#include "vector"
#include <fstream>
#include <iostream>
#include <math.h>

class DefaultRestController {
public:
  DefaultRestController();
  virtual ~DefaultRestController();
  int addNumbers(int, int);
  double power(int, int);
  std::vector<int> testVector(std::vector<int>);
  TestMany testObject(TestMany);
  std::vector<TestMany> testVectorObject(std::vector<TestMany> param);
  std::string testUploadFile(std::ifstream *ifs, std::string param);
  std::string testUploadFileMulti1(std::ifstream *ifs1, std::ifstream *ifs2,
                                   std::ifstream *ifs3, std::string param);
  std::string testUploadFileMulti2(std::vector<std::ifstream *> vifs,
                                   std::string param);
  TestSTLs sertest(TestSTLs ts);
  // int alloccounter();
};

#endif /* DEFAULTRESTCONTROLLER_H_ */
