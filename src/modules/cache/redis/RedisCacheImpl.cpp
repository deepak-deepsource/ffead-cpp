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
 * RedisCacheImpl.cpp
 *
 *  Created on: 07-May-2014
 *      Author: sumeetc
 */

#include "RedisCacheImpl.h"

RedisCacheImpl::RedisCacheImpl(ConnectionPooler *pool) {
  this->pool = pool;
  this->properties = pool->getProperties();
  this->defaultExpireSeconds = -1;
  if (properties.getProperty("expiryTime") != "") {
    try {
      this->defaultExpireSeconds =
          CastUtil::toInt(properties.getProperty("expiryTime"));
    } catch (const std::exception &e) {
    }
  }
  if (this->defaultExpireSeconds <= 0) {
    this->defaultExpireSeconds = 3600;
  }
}

RedisCacheImpl::~RedisCacheImpl() {}

bool RedisCacheImpl::remove(const std::string &key) {
  redisReply *reply = execute("DEL %s", key.c_str());
  return replyStatus(reply);
}

long long RedisCacheImpl::increment(const std::string &key, const int &number) {
  redisReply *reply = execute("INCRBY %s %d", key.c_str(), number);
  std::string val = replyValue(reply);
  if (val == "") {
    throw std::runtime_error("Command Failed");
  } else {
    return CastUtil::toLonglong(val);
  }
}

long long RedisCacheImpl::decrement(const std::string &key, const int &number) {
  redisReply *reply = execute("DECRBY %s %d", key.c_str(), number);
  std::string val = replyValue(reply);
  if (val == "") {
    throw std::runtime_error("Command Failed");
  } else {
    return CastUtil::toLonglong(val);
  }
}

long double RedisCacheImpl::incrementFloat(const std::string &key,
                                           const double &number) {
  redisReply *reply = execute("INCRBYFLOAT %s %f", key.c_str(), number);
  std::string val = replyValue(reply);
  if (val == "") {
    throw std::runtime_error("Command Failed");
  } else {
    return CastUtil::toLongdouble(val);
  }
}

long double RedisCacheImpl::decrementFloat(const std::string &key,
                                           const double &number) {
  return incrementFloat(key, -number);
}

std::map<std::string, std::string> RedisCacheImpl::statistics() {
  redisReply *reply = execute("INFO", 0);
  std::string sValue = replyValue(reply);
  std::map<std::string, std::string> stats;
  if (sValue == "")
    return stats;
  std::vector<std::string> data =
      StringUtil::splitAndReturn<std::vector<std::string>>(sValue, "\r\n");
  for (int var = 0; var < (int)data.size(); ++var) {
    if (data.at(var).find(":") != std::string::npos) {
      std::string key = data.at(var).substr(0, data.at(var).find(":"));
      std::string value = data.at(var).substr(data.at(var).find(":") + 1);
      stats[key] = value;
    }
  }
  return stats;
}

bool RedisCacheImpl::flushAll() {
  redisReply *reply = execute("FLUSHALL", 0);
  return replyStatus(reply);
}

bool RedisCacheImpl::set(const std::string &key, GenericObject &value,
                         int expireSeconds) {
  std::string valueStr = value.getSerilaizedState();
  redisReply *reply =
      execute("SET %s %s EX %d", key.c_str(), valueStr.c_str(),
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}

bool RedisCacheImpl::add(const std::string &key, GenericObject &value,
                         int expireSeconds) {
  std::string valueStr = value.getSerilaizedState();
  redisReply *reply =
      execute("SET %s %s EX %d NX", key.c_str(), valueStr.c_str(),
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}

bool RedisCacheImpl::replace(const std::string &key, GenericObject &value,
                             int expireSeconds) {
  std::string valueStr = value.getSerilaizedState();
  redisReply *reply =
      execute("SET %s %s EX %d XX", key.c_str(), valueStr.c_str(),
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}

bool RedisCacheImpl::setRaw(const std::string &key, const char *value,
                            int expireSeconds) {
  redisReply *reply =
      execute("SET %s %s EX %d", key.c_str(), value,
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}

bool RedisCacheImpl::addRaw(const std::string &key, const char *value,
                            int expireSeconds) {
  redisReply *reply =
      execute("SET %s %s EX %d NX", key.c_str(), value,
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}

bool RedisCacheImpl::replaceRaw(const std::string &key, const char *value,
                                int expireSeconds) {
  redisReply *reply =
      execute("SET %s %s EX %d XX", key.c_str(), value,
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}

std::string RedisCacheImpl::getValue(const std::string &key) {
  redisReply *reply = execute("GET %s", key.c_str());
  return replyValue(reply);
}

std::vector<std::string>
RedisCacheImpl::getValues(const std::vector<std::string> &keys) {
  std::vector<std::string> rv;
  mgetRaw(keys, rv);
  return rv;
}

bool RedisCacheImpl::setRaw(const unsigned long long &key,
                            const std::string_view &value, int expireSeconds) {
  redisReply *reply =
      execute("SET %llu %s EX %d", key, value.data(),
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}
bool RedisCacheImpl::addRaw(const unsigned long long &key,
                            const std::string_view &value, int expireSeconds) {
  redisReply *reply =
      execute("SET %llu %s EX %d NX", key, value.data(),
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}
bool RedisCacheImpl::replaceRaw(const unsigned long long &key,
                                const std::string_view &value,
                                int expireSeconds) {
  redisReply *reply =
      execute("SET %llu %s EX %d XX", key, value.data(),
              expireSeconds <= 0 ? defaultExpireSeconds : expireSeconds);
  return replyStatus(reply);
}
std::string RedisCacheImpl::getValue(const unsigned long long &key) {
  redisReply *reply = execute("GET %llu", key);
  return replyValue(reply);
}
void RedisCacheImpl::getValues(const std::vector<unsigned long long> &keys,
                               std::vector<std::string> &values) {
  fcpstream cmd;
  cmd << "MGET ";
  for (int i = 0; i < (int)keys.size(); ++i) {
    cmd << keys.at(i);
    if (i != (int)keys.size() - 1) {
      cmd << " ";
    }
  }
  Connection *connection = pool->checkout();
  redisContext *c = (redisContext *)connection->getConn();
  redisReply *reply = (redisReply *)redisCommand(c, cmd.str().c_str());
  if (reply->type == REDIS_REPLY_ARRAY) {
    for (int i = 0; i < (int)reply->elements; ++i) {
      values.push_back(
          std::string(reply->element[i]->str, reply->element[i]->len));
    }
  }
  freeReplyObject(reply);
  pool->release(connection);
}
bool RedisCacheImpl::remove(const unsigned long long &key) {
  redisReply *reply = execute("DEL %llu", key);
  return replyStatus(reply);
}

void RedisCacheImpl::mgetRaw(const std::vector<std::string> &keys,
                             std::vector<std::string> &values) {
  std::string cmd = "MGET ";
  for (int i = 0; i < (int)keys.size(); ++i) {
    cmd += keys.at(i) + (i == (int)keys.size() - 1 ? "" : " ");
  }
  Connection *connection = pool->checkout();
  redisContext *c = (redisContext *)connection->getConn();
  redisReply *reply = (redisReply *)redisCommand(c, cmd.c_str());
  if (reply->type == REDIS_REPLY_ARRAY) {
    for (int i = 0; i < (int)reply->elements; ++i) {
      values.push_back(
          std::string(reply->element[i]->str, reply->element[i]->len));
    }
  }
  freeReplyObject(reply);
  pool->release(connection);
}

bool RedisCacheImpl::replyStatus(redisReply *reply) {
  bool status = false;
  if (reply->type == REDIS_REPLY_ERROR || reply->type == REDIS_REPLY_NIL) {
    status = false;
  } else {
    status = true;
  }
  freeReplyObject(reply);
  return status;
}

std::string RedisCacheImpl::replyValue(redisReply *reply) {
  if (reply->type == REDIS_REPLY_INTEGER) {
    return CastUtil::fromNumber(reply->integer);
  } else if (reply->type == REDIS_REPLY_STRING) {
    std::string value;
    for (int var = 0; var < reply->len; ++var) {
      value.push_back(reply->str[var]);
    }
    return value;
  }
  freeReplyObject(reply);
  return "";
}

redisReply *RedisCacheImpl::execute(const char *format, ...) {
  Connection *connection = pool->checkout();
  redisContext *c = (redisContext *)connection->getConn();
  va_list vl;
  va_start(vl, format);
  redisReply *reply = (redisReply *)redisvCommand(c, format, vl);
  va_end(vl);
  pool->release(connection);
  return reply;
}

void *RedisCacheImpl::executeCommand(const std::string command, ...) {
  va_list vl;
  va_start(vl, command);
  void *reply = execute(command.c_str());
  va_end(vl);
  return reply;
}

bool RedisCacheImpl::addToQ(const std::string &qname,
                            const std::string &value) {
  redisReply *reply = execute("LPUSH %s %s", qname.c_str(), value.c_str());
  return replyStatus(reply);
}

std::string RedisCacheImpl::getFromQ(const std::string &qname) {
  redisReply *reply = execute("RPOP %s", qname.c_str());
  return replyValue(reply);
}

void RedisCacheImpl::init() {}

RedisCacheConnectionPool::RedisCacheConnectionPool(
    const ConnectionProperties &props) {
  createPool(props);
}

RedisCacheConnectionPool::~RedisCacheConnectionPool() { destroyPool(); }

void *RedisCacheConnectionPool::newConnection(const bool &isWrite,
                                              const ConnectionNode &node) {
  int connTimeoutUs = node.getConnectionTimeout() * 1000;
  int seconds = node.getConnectionTimeout() / 1000;
  int microseconds = connTimeoutUs - (seconds * 1000000);

  struct timeval timeout = {seconds, microseconds}; // 1.5 seconds
  redisContext *c =
      redisConnectWithTimeout(node.getHost().c_str(), node.getPort(), timeout);
  if (c == NULL || c->err) {
    if (c) {
      printf("Connection error: %s\n", c->errstr);
      redisFree(c);
    } else {
      printf("Connection error: can't allocate redis context\n");
    }
    return NULL;
  }
  return c;
}
void RedisCacheConnectionPool::closeConnection(void *conn) {
  redisFree((redisContext *)conn);
}

void RedisCacheConnectionPool::initEnv() {}

void RedisCacheConnectionPool::destroy() {}
