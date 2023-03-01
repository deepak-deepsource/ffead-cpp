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
 * DataSourceMapping.h
 *
 *  Created on: 18-May-2014
 *      Author: sumeetc
 */

#ifndef DATASOURCEMAPPING_H_
#define DATASOURCEMAPPING_H_
#include "StringUtil.h"
#include "map"

typedef std::map<std::string, std::string> strMap;
typedef std::map<std::string, strMap> smstrMap;

class DataSourceInternalRelation {
  int type; // 1->1-1 2->1-N 3->N-N
  std::string dfk;
  std::string dmappedBy;
  std::string clsName;
  std::string field;
  friend class ConfigurationHandler;

public:
  const std::string &getClsName() const;
  void setClsName(const std::string &clsName);
  const std::string &getField() const;
  void setField(const std::string &field);
  const std::string &getDfk() const;
  void setDfk(const std::string &dfk);
  const std::string &getDmappedBy() const;
  void setDmappedBy(const std::string &dmappedBy);
  int getType() const;
  void setType(const int &type);
};

class DataSourceEntityMapping {
  std::string className;
  std::string tableName;
  bool embedded;
  bool idGenerate;
  std::string idgendbEntityType;
  std::string idgendbEntityName;
  std::string idgentype;
  std::string idgenhiValueColumn;
  std::string idgenentityColumn;
  std::string idgencolumnName;
  int idgenlowValue;
  std::string idPropertyName;
  strMap propertyColumnMapping;
  strMap columnPropertyMapping;
  strMap propertyColumnMappingCS;
  strMap columnPropertyMappingCS;
  std::vector<DataSourceInternalRelation> relations;
  friend class Mapping;
  friend class ConfigurationHandler;

public:
  DataSourceEntityMapping();
  ~DataSourceEntityMapping();
  const std::string &getClassName() const;
  void setClassName(const std::string &className);
  const std::string &getIdPropertyName() const;
  void setIdPropertyName(const std::string &idColumnName);
  bool isEmbedded() const;
  void setIsEmbedded(const bool &isEmbedded);
  const std::string &getTableName() const;
  void setTableName(const std::string &tableName);
  void addPropertyColumnMapping(const std::string &property,
                                const std::string &column);
  void addRelation(const DataSourceInternalRelation &relation);
  std::string getColumnForProperty(const std::string &property);
  std::string getPropertyForColumn(const std::string &column);
  std::vector<DataSourceInternalRelation> getRelations();
  const strMap &getColumnPropertyMapping() const;
  const strMap &getPropertyColumnMapping() const;
  const strMap &getPropertyColumnMappingCS() const;
  const std::string &getIdgencolumnName() const;
  const std::string &getIdgendbEntityName() const;
  const std::string &getIdgendbEntityType() const;
  const std::string &getIdgenentityColumn() const;
  const std::string &getIdgenhiValueColumn() const;
  const std::string &getIdgentype() const;
  bool isIdGenerate() const;
  int getIdgenlowValue() const;
  void setEmbedded(const bool &embedded);
  void setIdgencolumnName(const std::string &idgencolumnName);
  void setIdgendbEntityName(const std::string &idgendbEntityName);
  void setIdgendbEntityType(const std::string &idgendbEntityType);
  void setIdgenentityColumn(const std::string &idgenentityColumn);
  void setIdGenerate(const bool &idGenerate);
  void setIdgenhiValueColumn(const std::string &idgenhiValueColumn);
  void setIdgentype(const std::string &idgentype);
  void setIdgenlowValue(const int &idgenlowValue);
};

class Mapping {
  DataSourceEntityMapping __c;
  strMap __s;
  std::string appName;
  strMap tableClassMapping;
  std::map<std::string, DataSourceEntityMapping> dseMap;
  friend class ConfigurationHandler;

public:
  void addDataSourceEntityMapping(const DataSourceEntityMapping &dsemp);
  DataSourceEntityMapping &getDataSourceEntityMapping(const std::string &clas);
  std::string getPropertyForColumn(const std::string &tableName,
                                   const std::string &columnName);
  strMap &getMappingForTable(const std::string &tableName);
  std::string getTableForClass(const std::string &claz);
  std::string getClassForTable(const std::string &table);
  const std::string &getAppName() const;
  void setAppName(const std::string &appName);
  std::map<std::string, DataSourceEntityMapping> &getDseMap();
};

#endif /* DATASOURCEMAPPING_H_ */
