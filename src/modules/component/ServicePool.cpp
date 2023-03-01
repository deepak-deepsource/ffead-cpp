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
 * ServicePool.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: sumeet
 */

#include "ServicePool.h"
static ServicePool *instance = NULL;
static std::map<std::string, Service> servicePool;
ServicePool::ServicePool() { instance = NULL; }

ServicePool::~ServicePool() {}

ServicePool *ServicePool::getInstance() {
  if (instance == NULL)
    instance = new ServicePool;
  return instance;
}

std::string ServicePool::registerService(
    const std::string &name,
    const Service
        &service) // will return a unique identification for this service
{
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  timespec en;
  clock_gettime(CLOCK_REALTIME, &en);

  std::string yr = CastUtil::fromNumber(timeinfo->tm_year);
  std::string mo = CastUtil::fromNumber(timeinfo->tm_mon);
  std::string da = CastUtil::fromNumber(timeinfo->tm_mday);
  std::string hr = CastUtil::fromNumber(timeinfo->tm_hour);
  std::string mm = CastUtil::fromNumber(timeinfo->tm_min);
  std::string ms =
      CastUtil::fromNumber(((en.tv_sec * 1000000000) + en.tv_nsec) / 1000000);

  std::string regName = (name + yr + mo + da + hr + mm + ms);
  servicePool[regName] = service;
  return regName;
}

bool ServicePool::unRegisterService(
    const std::string &name) // unregister will require the unique id
{
  std::map<std::string, Service>::iterator it;
  if (it == servicePool.end())
    return false;
  it = servicePool.find(name);
  servicePool.erase(it);
  return true;
}

std::vector<std::string> ServicePool::getServices(
    const std::string &access) // return a list of available services
{
  std::vector<std::string> services;
  std::map<std::string, Service>::iterator it;
  for (it = servicePool.begin(); it != servicePool.end(); ++it) {
    services.push_back(it->first);
  }
  return services;
}

Service ServicePool::getService(const std::string &regName) {
  return servicePool[regName];
}
