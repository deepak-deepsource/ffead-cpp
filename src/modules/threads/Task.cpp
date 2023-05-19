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
 * Task.cpp
 *
 *  Created on: Mar 23, 2010
 *      Author: sumeet
 */

#include "Task.h"

Task::Task()
    : tunit(-1), type(-1), priority(false), console(false), cleanUp(false),
      isFuture(false), hdlr(NULL) {}
Task::Task(const int &priority)
    : tunit(-1), type(-1), console(false), cleanUp(false), isFuture(false),
      hdlr(NULL) {
  this->priority = priority;
}
Task::Task(const int &tunit, const int &type)
    : priority(false), console(false), cleanUp(false), isFuture(false),
      hdlr(NULL) {
  this->tunit = tunit;
  this->type = type;
}
Task::~Task() {}

void Task::init() {
  tunit = -1;
  type = -1;
  cleanUp = false;
}

void Task::setPriority(const int &priority) {
  this->priority = priority;
  init();
}
void Task::setTunitType(const int &tunit, const int &type) {
  this->tunit = tunit;
  this->type = type;
  cleanUp = false;
}

bool Task::isWaitOver(Timer *timer) {
  bool flag = false;
  if (type == TimeUnit::NANOSECONDS) {
    long long diff = timer->elapsedNanoSeconds();
    if (tunit <= diff) {
      flag = true;
    }
  } else if (type == TimeUnit::MICROSECONDS) {
    long long diff = timer->elapsedMicroSeconds();
    if (tunit <= diff) {
      flag = true;
    }
  } else if (type == TimeUnit::MILLISECONDS) {
    long long diff = timer->elapsedMilliSeconds();
    if (tunit <= diff) {
      flag = true;
    }
  } else if (type == TimeUnit::SECONDS) {
    long long diff = timer->elapsedSeconds();
    if (tunit <= diff) {
      flag = true;
    }
  } else if (type == TimeUnit::MINUTES) {
    long long diff = timer->elapsedSeconds() / 60;
    diff = diff / 60;
    if (tunit <= diff) {
      flag = true;
    }
  } else if (type == TimeUnit::HOURS) {
    long long diff = timer->elapsedSeconds();
    diff = diff / 3600;
    if (tunit <= diff) {
      flag = true;
    }
  } else if (type == TimeUnit::DAYS) {
    long long diff = timer->elapsedSeconds();
    diff = diff / 86400;
    if (tunit <= diff) {
      flag = true;
    }
  }
  return flag;
}

void Task::setCleanUp(const bool &cleanUp) { this->cleanUp = cleanUp; }

bool Task::operator<(const Task &other) {
  return this->priority < other.priority;
}

void Task::setTid(int tid) {}

int Task::getTid() { return -1; }
