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
 * Mutex.h
 *
 *  Created on: 10-Aug-2012
 *      Author: sumeetc
 */

#ifndef MUTEX_H_
#define MUTEX_H_
#include "AppDefines.h"
#include "exception"
#include "string"
#include <atomic>
#include <fcntl.h>
#include <pthread.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

#ifdef OS_MINGW
struct flock {
  int l_type;
  int l_len;
};
#endif

class Mutex {
protected:
  pthread_mutex_t mut;

public:
  Mutex();
  virtual ~Mutex();
  virtual void lock();
  virtual void unlock();
};

class DummyMutex : public Mutex {
public:
  DummyMutex();
  virtual ~DummyMutex();
  void lock();
  void unlock();
};

class ReadWriteMutex {
  pthread_rwlock_t mut;

public:
  ReadWriteMutex();
  virtual ~ReadWriteMutex();
  void rlock();
  void wlock();
  void unlock();
};

class ConditionMutex : public Mutex {
  pthread_cond_t cond;

public:
  ConditionMutex();
  virtual ~ConditionMutex();
  void lock();
  void unlock();
  void conditionalWait();
  void conditionalNotifyOne();
  void conditionalNotifyAll();
  void wait();
  void interrupt();
};

class FileBasedLock {
  FILE *fp;
  std::string lkFile;
  struct flock fl;

public:
  FileBasedLock(const std::string &lkFile);
  virtual ~FileBasedLock();
  void lock();
  void unlock();
};
#endif /* MUTEX_H_ */
