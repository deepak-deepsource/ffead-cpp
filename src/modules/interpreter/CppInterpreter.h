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
 * CppInterpreter.h
 *
 *  Created on: Aug 22, 2009
 *      Author: sumeet
 */

#ifndef CPPINTERPRETER_H_
#define CPPINTERPRETER_H_
#include "CastUtil.h"
#include "GenericObject.h"
#include "LoggerFactory.h"
#include "Reflector.h"
#include "RegexUtil.h"
#include "StringUtil.h"
#include "bitset"
#include "iostream"
#include "map"
#include "string"
typedef std::map<std::string, GenericObject> mapVars;
typedef std::map<std::string, std::string> mapStrs;

class Obj {
  std::string type;
  void *pointer;

public:
  Obj() { pointer = NULL; }
  ~Obj() {}
  std::string getType() const { return type; }

  void setType(const std::string &type) { this->type = type; }

  void *getPointer() const { return pointer; }

  void setPointer(void *pointer) { this->pointer = pointer; }
};
class CppInterpreter {
  Logger logger;
  mapVars localVariables;
  mapVars boundVariables;
  mapStrs literals;
  mapVars getLocalVariables() const;
  void setLocalVariables(const mapVars &);
  mapVars getBoundVariables() const;
  void setBoundVariables(const mapVars &);
  bool isInBuiltType(const std::string &);
  void storeInbuilt(const std::string &, const std::string &);
  void storeCustom(const std::string &, const std::string &);
  void evaluateUpdateInbuilt(const std::string &, const std::string &,
                             const std::string &, std::vector<std::string>,
                             const bool &);
  void evaluateUpdateCustom(const std::string &, const std::string &,
                            const std::string &,
                            const std::vector<std::string> &, const bool &);
  void executeStatement(const std::string &sep,
                        const std::vector<std::string> &lhs,
                        const std::vector<std::string> &rhs);
  bool evaluateCondition(const std::string &condition);
  bool evalCond(std::vector<std::string> str);
  bool isCommand(const std::string &test);
  bool containsChar(const std::string &varname);
  bool retState(const std::string &type, GenericObject &source,
                GenericObject &target);
  bool retState(const std::string &type, GenericObject &source,
                const std::string &target);
  void hanldeIF(std::vector<std::string>::iterator &iter);
  void handleELSE(std::vector<std::string>::iterator &iter);
  void hanldeFOR(std::vector<std::string>::iterator &iter);
  void hanldeWHILE(std::vector<std::string>::iterator &iter);
  void hanldeCommand(std::vector<std::string>::iterator &itr);
  void handleStatement(std::vector<std::string>::iterator &itr);
  void skipStatement(std::vector<std::string>::iterator &itr);
  void skipCommand(std::vector<std::string>::iterator &itr);
  Obj handleObjectMethodInvocation(const std::string &, const std::string &,
                                   std::vector<std::string>::iterator &itr);
  template <class T>
  std::string evalBrackets(std::vector<std::string>::iterator &itr,
                           const std::vector<std::string>::iterator &enditr) {
    std::string token = *(itr);
    std::vector<std::string> curr, going;
    bool st = false;
    while (itr < enditr) {
      token = *(itr);
      if (token == "(") {
        if (st)
          curr.push_back(evalBrackets<T>(itr, enditr));
        st = true;
      } else if (token == ")") {
        curr.push_back(handleAssignment<T>(going));
        st = false;
        going.clear();
      } else {
        if (containsChar(token)) {
          int val = 0;
          localVariables[token].get<int>(val);
          token = CastUtil::fromNumber(val);
        }
        if (st)
          going.push_back(token);
        else
          curr.push_back(token);
      }
      ++itr;
    }
    return handleAssignment<T>(curr);
  }
  template <class T>
  std::string handleAssignment(std::vector<std::string> opr) {
    std::vector<std::string> temp;
    while (opr.size() > 1) {
      bool continu = false;
      temp.clear();
      while (1) {
        // unsigned int found = -1;
        for (int i = 0; i < (int)opr.size(); i++) {
          if (opr.at(i) == "/" && !continu) {
            T f;
            f = CastUtil::lexical_cast<T>(opr.at(i - 1)) /
                CastUtil::lexical_cast<T>(opr.at(i + 1));
            for (int k = 0; k < (i - 1); k++) {
              temp.push_back(opr.at(k));
            }
            temp.push_back(CastUtil::lexical_cast<std::string>(f));
            for (int k = i + 2; k < (int)opr.size(); k++) {
              temp.push_back(opr.at(k));
            }
            continu = true;
            // found = i;
          }
          /*else if((opr.size()>(i+2) && opr.at(i+2)!='/') && i!=found &&
          i!=found+1 && i!=found-1)
          {
                  temp.push_back(opr.at(i));
          }*/
        }
        if (!continu)
          break;
        else {
          continu = false;
          if (temp.size() > 0)
            opr = temp;
          temp.clear();
        }
      }
      continu = false;
      temp.clear();
      while (1) {
        // unsigned int found = -1;
        for (int i = 0; i < (int)opr.size(); i++) {
          if (opr.at(i) == "*" && !continu) {
            T f;
            f = CastUtil::lexical_cast<T>(opr.at(i - 1)) *
                CastUtil::lexical_cast<T>(opr.at(i + 1));
            for (int k = 0; k < i - 1; k++) {
              temp.push_back(opr.at(k));
            }
            temp.push_back(CastUtil::lexical_cast<std::string>(f));
            for (int k = i + 2; k < (int)opr.size(); k++) {
              temp.push_back(opr.at(k));
            }
            continu = true;
            // found = i;
          }
          /*else if(i!=found && i!=found+1 && i!=found-1)
          {
                  temp.push_back(opr.at(i));
          }*/
        }
        if (!continu)
          break;
        else {
          continu = false;
          if (temp.size() > 0)
            opr = temp;
          temp.clear();
        }
      }
      continu = false;
      temp.clear();
      /*while(1)
      {
              unsigned int found = -1;
              for(int i=0;i<(int)opr.size();i++)
              {
                      if(opr.at(i)=="-" && !continu)
                      {
                              T f;
                              f =
      CastUtil::lexical_cast<T>(opr.at(i-1))-CastUtil::lexical_cast<T>(opr.at(i+1));
                              for(int k=0;k<i-1;k++)
                              {
                                      temp.push_back(opr.at(k));
                              }
                              temp.push_back(CastUtil::lexical_cast<std::string>(f));
                              for(int k=i+2;k<(int)opr.size();k++)
                              {
                                      temp.push_back(opr.at(k));
                              }
                              continu = true;
                              found = i;
                      }
              }
              if(!continu)
                      break;
              else
              {
                      continu = false;
                      if(temp.size()>0)opr = temp;
                      temp.clear();
              }
      }
      continu = false;
      temp.clear();*/
      while (1) {
        // unsigned int found = -1;
        for (int i = 0; i < (int)opr.size(); i++) {
          if (opr.at(i) == "+" && !continu) {
            T f;
            f = CastUtil::lexical_cast<T>(opr.at(i - 1)) +
                CastUtil::lexical_cast<T>(opr.at(i + 1));
            for (int k = 0; k < i - 1; k++) {
              temp.push_back(opr.at(k));
            }
            temp.push_back(CastUtil::lexical_cast<std::string>(f));
            for (int k = i + 2; k < (int)opr.size(); k++) {
              temp.push_back(opr.at(k));
            }
            continu = true;
            // found = i;
          } else if (opr.at(i) == "-" && !continu) {
            T f;
            f = CastUtil::lexical_cast<T>(opr.at(i - 1)) +
                CastUtil::lexical_cast<T>(opr.at(i) + opr.at(i + 1));
            for (int k = 0; k < i - 1; k++) {
              temp.push_back(opr.at(k));
            }
            temp.push_back(CastUtil::lexical_cast<std::string>(f));
            for (int k = i + 2; k < (int)opr.size(); k++) {
              temp.push_back(opr.at(k));
            }
            continu = true;
            // found = i;
          }
          /*else if(i!=found && i!=found+1 && i!=found-1)
          {
                  temp.push_back(opr.at(i));
          }*/
        }
        if (!continu)
          break;
        else {
          continu = false;
          if (temp.size() > 0)
            opr = temp;
          temp.clear();
        }
      }
    }
    // if(opr.size()>0)
    return opr.at(0);
    // return "";
  }

public:
  CppInterpreter();
  virtual ~CppInterpreter();
  void eval(std::string);
  template <class T> void bind(const std::string &name, T &t) {
    boundVariables[name] << t;
  }
  template <class T> void bind(const std::string &name, T *t) {
    boundVariables[name] << t;
  }
  /*template <class T> void bind(const std::map<std::string,T>& mapT)
  {
      std::map<std::string,std::string>::iterator it;
      for(it=mapT.begin();it!=mapT.end();++it)
      {
                      GenericObject o;
                      o << it->second;
                      boundVariables[it->first] = o;
      }
  }
  template <class T> T getVariable(const std::string& name)
  {
      GenericObject o = localVariables[name];
      return o.getValue<T>();
  }
  template <class T> T getCollectionVariable(const std::string& name, const
  std::string& type, const std::string& index="")
  {
      if(type=="vector")
      {

      }
      else if(type=="deque")
      {

      }
      else if(type=="list")
      {

      }
      else if(type=="stack")
      {

      }
      else if(type=="queue")
      {

      }
      else if(type=="priority_queue")
      {

      }
      else if(type=="set")
      {

      }
      else if(type=="multiset")
      {

      }
      else if(type=="map")
      {

      }
      else if(type=="bitset")
      {

      }
  }*/
};

#endif /* CPPINTERPRETER_H_ */
