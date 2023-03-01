/*
        Copyright 2009-2013, Sumeet Chhetri

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
 * DistGlobalCache.h
 *
 *  Created on: 07-Apr-2013
 *      Author: sumeetc
 */

#ifndef DISTGLOBALCACHE_H_
#define DISTGLOBALCACHE_H_
#include "BinarySerialize.h"
#include "PooledDistoCacheConnectionFactory.h"

class DistGlobalCache {
  DistoCacheClientUtils *cl;

public:
  DistGlobalCache() { cl = PooledDistoCacheConnectionFactory::getConnection(); }
  template <class T> void add(const std::string &key, const T &value) {
    std::string serValue = BinarySerialize::serialize<T>(value, -1);
    cl->addObjectEntry(key, serValue);
  }
  template <class K, class V>
  void addMap(const std::string &key, std::map<K, V> &value) {
    std::string serValue = BinarySerialize::serializeMap<K, V>(value);
    cl->addObjectEntry(key, serValue);
  }
  template <class T> T get(const std::string &key) {
    std::string serValue = cl->getObjectEntryValue(key);
    return BinarySerialize::unserialize<T>(serValue, -1);
  }
  template <class K, class V> std::map<K, V> getMap(const std::string &key) {
    std::string serValue = cl->getObjectEntryValue(key);
    return BinarySerialize::unSerializeToMap<K, V>(serValue);
  }
  void erase(const std::string &key) { cl->removeObjectEntry(key); }
  ~DistGlobalCache() {
    PooledDistoCacheConnectionFactory::releaseConnection(cl);
  }
};

#endif /* DISTGLOBALCACHE_H_ */
