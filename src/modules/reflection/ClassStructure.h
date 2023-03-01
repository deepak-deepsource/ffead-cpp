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
 * ClassStructure.h
 *
 *  Created on: 14-Jun-2013
 *      Author: sumeetc
 */

#ifndef CLASSSTRUCTURE_H_
#define CLASSSTRUCTURE_H_
#include "MarkerHandler.h"
#include "map"
#include "string"
#include "vector"
#include <tuple>

typedef std::vector<std::string> strVec;
typedef std::map<std::string, std::string> propMap;

class PropStructure {
  std::string type;
  std::string name;
  std::string decl;
  std::map<std::string, std::vector<Marker>> markers;
  friend class Reflection;
  friend class ConfigurationHandler;
  friend class ClassStructure;
};

class MethStructure {
  std::string name;
  std::map<int, std::string> argstypes;
  std::map<int, std::map<std::string, std::vector<Marker>>> argMarkers;
  std::string decl;
  std::string retType;
  std::map<std::string, std::vector<Marker>> markers;
  friend class Reflection;
  friend class ConfigurationHandler;
  friend class ClassStructure;
};

class ClassStructure {
  std::map<std::string, std::vector<Marker>> markers;
  bool prosetser;
  strVec pub, pri, pro;
  std::vector<PropStructure> pubps, prips, props;
  std::vector<MethStructure> pubms, prims, proms;
  std::vector<std::tuple<int, std::string>> bases;
  std::string classN, baseClassN, nmSpc;
  std::vector<std::string> namespaces;
  std::vector<PropStructure> getAllProps() {
    std::vector<PropStructure> a;
    a.insert(a.end(), prips.begin(), prips.end());
    a.insert(a.end(), props.begin(), props.end());
    a.insert(a.end(), pubps.begin(), pubps.end());
    return a;
  }
  PropStructure getPs(std::string name) {
    PropStructure t;
    std::vector<PropStructure> aps = getAllProps();
    for (int var = 0; var < (int)aps.size(); ++var) {
      if (aps.at(var).name == name) {
        return aps.at(var);
      }
    }
    return t;
  }
  friend class Reflection;
  friend class ConfigurationHandler;

public:
  std::string appName, incfile;
  std::string toString() {
    std::string out;
    std::string tab;
    for (int var = 0; var < (int)namespaces.size(); ++var) {
      out.append("using namespace " + namespaces.at(var) + "n");
    }
    std::vector<std::string> clnms =
        StringUtil::splitAndReturn<std::vector<std::string>>(nmSpc, "::");
    if (clnms.size() != 0) {
      for (int var = 0; var < (int)clnms.size(); ++var) {
        out.append(tab + "namespace " + clnms.at(var) + " {n");
        tab += "t";
      }
    }

    out.append(tab + "class " + classN);
    if (baseClassN != "") {
      out.append(" : " + baseClassN + " {n");
    }
    for (int var = 0; var < (int)pri.size(); ++var) {
      out.append(tab + "t" + pri.at(var) + "n");
    }
    if (pro.size() > 0) {
      out.append(tab + "protected:n");
    }
    for (int var = 0; var < (int)pro.size(); ++var) {
      out.append(tab + "t" + pro.at(var) + "n");
    }
    if (pub.size() > 0) {
      out.append(tab + "public:n");
    }
    for (int var = 0; var < (int)pub.size(); ++var) {
      out.append(tab + "t" + pub.at(var) + "n");
    }

    out.append(tab + "}n");

    if (clnms.size() != 0) {
      for (int var = 0; var < (int)clnms.size(); ++var) {
        for (int va1r = 0; va1r < var; ++va1r) {
          out.append("t");
        }
        out.append("}n");
      }
    }
    return out;
  }
  std::string getTreatedClassName(const bool &flag) {
    if (flag) {
      std::string nm = nmSpc;
      StringUtil::replaceAll(nm, "::", "_");
      return nm + classN;
    } else {
      return classN;
    }
  }
  std::string getFullyQualifiedClassName() {
    /*if(nmSpc!="")
    {
            return nmSpc+classN;
    }*/
    return nmSpc + classN;
  }
  std::vector<std::string> getNamespaces() { return namespaces; }
  std::string getNamespace() { return nmSpc; }
};

#endif /* CLASSSTRUCTURE_H_ */
