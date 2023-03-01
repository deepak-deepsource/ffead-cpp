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
 * JSONSerialize.h
 *
 *  Created on: 12-Jun-2013
 *      Author: sumeetc
 */
#ifndef JSONSERIALIZE_H_
#define JSONSERIALIZE_H_
#include "JSONUtil.h"
#include "SerializeBase.h"
#include <stdexcept>

// #undef HAVE_RAPID_JSON
#ifdef HAVE_RAPID_JSON
// Copied from
// https://github.com/Tencent/rapidjson/blob/a627774/test/unittest/unittest.h#L109
class AssertException : public std::logic_error {
public:
  AssertException(const char *w) : std::logic_error(w) {}
  AssertException(const AssertException &rhs) : std::logic_error(rhs) {}
  virtual ~AssertException() throw() {}
};
#define RAPIDJSON_ASSERT(x)                                                    \
  if (!(x))                                                                    \
  throw AssertException(RAPIDJSON_STRINGIFY(x))
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#endif

class JSONSerialize;

class TemplateJson {
public:
  std::string const t;
  std::vector<int> const tpos;

public:
  TemplateJson(std::string t, std::initializer_list<int> pos)
      : t(t), tpos(pos) {}
};

#ifdef HAVE_RAPID_JSON
class RapiJsonState {
  std::string *str;
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer;
  friend class JSONSerialize;
  friend class Serializer;
  RapiJsonState(void *str) : str((std::string *)str), writer(s) {}
};
#endif

class JSONSerialize : public SerializeBase {
  static JSONSerialize _i;
  void *getSerializableObject(void *exobj);
  void cleanSerializableObject(void *_1);
  void startContainerSerialization(void *_1, const std::string &className,
                                   const std::string &container);
  void endContainerSerialization(void *_1, const std::string &className,
                                 const std::string &container);
  void afterAddContainerSerializableElement(void *_1, const int &counter,
                                            const int &size);
  void addContainerSerializableElement(void *_1, const std::string &tem);
  void addContainerSerializableElementMulti(void *_1, const std::string &tem);
  std::string fromSerializableObjectToString(void *_1);
  std::string elementToSerializedString(void *_1, const int &counter);
  std::string getConatinerElementClassName(void *_1,
                                           const std::string &className);
  void *getContainerElement(void *_1, const int &counter,
                            const int &counter1 = -1);
  void addPrimitiveElementToContainer(void *_1, int serOpt, const int &counter,
                                      const std::string &className, void *cont,
                                      const std::string &container);
  void *getUnserializableObject(const std::string &_1);
  void cleanUnserializableObject(void *_1);
  void cleanValidUnserializableObject(void *_1);
  void *getValidUnserializableObject(const std::string &_1);
  int getContainerSize(void *_1);
  std::string getUnserializableClassName(void *_1,
                                         const std::string &className);
  void *getPrimitiveValue(void *_1, int serOpt, const std::string &className);
  friend class Serializer;

public:
  JSONSerialize();
  JSONSerialize(void *);
  ~JSONSerialize();

  int getSerializerType() { return 1; }

  std::string serializePrimitive(int serOpt, const std::string &className,
                                 void *t, void *serobject);
  template <class T>
  static std::string serialize(T &t, int serOpt,
                               const std::string &appName = "") {
    std::string className = CastUtil::getClassName(t);
    if (serOpt == -1)
      serOpt = identifySerOption(className);
    return _handleAllSerialization(serOpt, className, &t, appName, &_i, NULL,
                                   NULL, NULL, NULL);
  }
  template <class T>
  static std::string serializePointer(T *t, int serOpt,
                                      const std::string &appName = "") {
    std::string className = CastUtil::getClassName(t);
    if (serOpt == -1)
      serOpt = identifySerOption(className);
    return _handleAllSerialization(serOpt, className, t, appName, &_i, NULL,
                                   NULL, NULL, NULL);
  }
  static std::string serializeUnknown(void *t, int serOpt,
                                      const std::string &className,
                                      void *serobject,
                                      const std::string &appName = "");
  static std::string serializeUnknown(void *t, int serOpt,
                                      const std::string &className, Ser f1,
                                      SerCont f2, SerCont f3, void *serobject,
                                      const std::string &appName);
  static std::string serializeObject(void *t, Ser f1, void *serobject,
                                     bool withTmpl = false);
  static std::string serializeObjectCont(void *t, SerCont f,
                                         const std::string &container,
                                         void *serobject);

  /*template <class K,class V> static std::string serialize(const std::map<K,V>&
  mp, const std::string& appName = "")
  {
          std::map<K,V> mpt  = mp;
          AMEFEncoder enc;
          AMEFObject object;
          K k;
          std::string kclassName = CastUtil::getClassName(k);
          V v;
          std::string serval;
          std::string vclassName = CastUtil::getClassName(v);
          kclassName = "map<"+kclassName+":"+vclassName+">";
          object.setName(kclassName);
          while (mpt.begin()!=mpt.end())
          {
                  std::string key = serialize<K>(mpt.begin()->first,appName);
                  std::string value = serialize<V>(mpt.begin()->second,appName);
                  mpt.erase(mpt.begin());
                  object.addPacket(value, key);
          }
          return enc.encodeB(&object, false);
  }

  template <class K,class V> static std::string serialize(const
  std::multimap<K,V>& mp, const std::string& appName = "")
  {
          std::multimap<K,V> mpt  = mp;
          AMEFEncoder enc;
          AMEFObject object;
          K k;
          std::string kclassName = CastUtil::getClassName(k);
          V v;
          std::string serval;
          std::string vclassName = CastUtil::getClassName(v);
          kclassName = "multimap<"+kclassName+":"+vclassName+">";
          object.setName(kclassName);
          while (mpt.begin()!=mpt.end())
          {
                  std::string key = serialize<K>(mpt.begin()->first,appName);
                  std::string value = serialize<V>(mpt.begin()->second,appName);
                  mpt.erase(mpt.begin());
                  object.addPacket(value, key);
          }
          return enc.encodeB(&object, false);
  }*/
  template <class T>
  static T unserialize(const std::string &objJson, int serOpt,
                       const std::string &appName = "") {
    T t;
    std::string className = CastUtil::getClassName(t);
    if (serOpt == -1)
      serOpt = identifySerOption(className);
    T *tp = (T *)_handleAllUnSerialization(objJson, serOpt, className, appName,
                                           &_i, true, NULL);
    if (tp != NULL) {
      t = *(T *)tp;
      delete ((T *)tp);
    }
    return t;
  }
  template <class T>
  static T unserialize(JSONElement *element, int serOpt,
                       const std::string &appName = "") {
    T t;
    std::string className = CastUtil::getClassName(t);
    if (serOpt == -1)
      serOpt = identifySerOption(className);
    T *tp = (T *)_handleAllUnSerialization("", serOpt, className, appName, &_i,
                                           true, element);
    if (tp != NULL) {
      t = *(T *)tp;
      delete ((T *)tp);
    }
    return t;
  }
  template <class T>
  static T *unserializeToPointer(const std::string &objJson, int serOpt,
                                 const std::string &appName = "") {
    T *t;
    std::string className = CastUtil::getClassName(t);
    if (serOpt == -1)
      serOpt = identifySerOption(className);
    return (T *)_handleAllUnSerialization(objJson, serOpt, className, appName,
                                          &_i, true, NULL);
  }
  template <class T>
  static T *unserializeToPointer(JSONElement *element, int serOpt,
                                 const std::string &appName = "") {
    T *t;
    std::string className = CastUtil::getClassName(t);
    if (serOpt == -1)
      serOpt = identifySerOption(className);
    return (T *)_handleAllUnSerialization("", serOpt, className, appName, &_i,
                                          true, element);
  }

  bool isValidClassNamespace(void *_1, const std::string &className,
                             const std::string &namespc,
                             const bool &iscontainer = false);
  bool isValidObjectProperty(void *_1, const std::string &propname,
                             const int &counter);
  void *getObjectProperty(void *_1, const int &counter,
                          const std::string &propname);
  void startObjectSerialization(void *_1, const std::string &className);
  void endObjectSerialization(void *_1, const std::string &className);
  void afterAddObjectProperty(void *_1, const std::string &propName);
  void addObjectPrimitiveProperty(void *_1, int serOpt,
                                  const std::string &propName,
                                  const std::string &className, void *t);
  void addObjectProperty(void *_1, const std::string &propName,
                         std::string className);
  void *getObjectPrimitiveValue(void *_1, int serOpt,
                                const std::string &className,
                                const std::string &propName);
  static void *unSerializeUnknown(const std::string &objJson, int serOpt,
                                  const std::string &className,
                                  const std::string &appName = "");
  std::string serializeUnknownBase(void *t, int serOpt,
                                   const std::string &className,
                                   const std::string &appName, void *serobject);
  std::string serializeUnknownBaseInt(void *t, int serOpt,
                                      const std::string &className,
                                      const std::string &appName,
                                      void *serobject);
  void *unSerializeUnknownBase(void *unserObj, int serOpt,
                               const std::string &className,
                               const std::string &appName = "");
  void *unSerializeUnknownBase(const std::string &serVal, int serOpt,
                               const std::string &className,
                               const std::string &appName = "");
};

#endif /* JSONSERIALIZE_H_ */
