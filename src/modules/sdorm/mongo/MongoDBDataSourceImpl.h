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
 * MongoDBDataSourceImpl.h
 *
 *  Created on: 02-Jun-2014
 *      Author: sumeetc
 */

#ifndef MONGODBDATASOURCEIMPL_H_
#define MONGODBDATASOURCEIMPL_H_

#include "DataSourceInterface.h"
#include "TemplateEngine.h"
#include "mongoc.h"

class QueryComponent {
  bool isAnd;
  bool undecided;
  std::vector<Condition> andClauses;
  std::vector<Condition> orClauses;
  std::vector<Condition> tempClauses;
  std::vector<QueryComponent *> andChildren;
  std::vector<QueryComponent *> orChildren;
  std::vector<QueryComponent *> tempChildren;
  QueryComponent *parent;
  bson_t *actualQuery;
  friend class MongoDBDataSourceImpl;
  QueryComponent();
  virtual ~QueryComponent();
};

class MongoDBDataSourceImpl : public DataSourceInterface {
  Logger logger;
  Connection *conn;
  mongoc_collection_t *collection;
  static QueryComponent *getQueryComponent(const std::vector<Condition> &conds);
  static void populateQueryComponents(QueryComponent *sq);
  static bson_t *createSubMongoQuery(std::vector<Condition> &conds);
  static void appendGenericObject(bson_t *b, const std::string &name,
                                  GenericObject &o);
  static std::map<std::string, std::map<std::string, Condition>>
  toMap(std::vector<Condition> &conds);
  void getBSONObjectFromObject(const std::string &clasName, void *object,
                               bson_t *, const bool &isIdBsonAppend = true);
  std::string initializeQueryParts(Query &cquery, bson_t **opts,
                                   bson_t **querySpec,
                                   std::string &operationName);
  std::string initializeDMLQueryParts(Query &cquery, bson_t **data,
                                      bson_t **query,
                                      std::string &operationName);
  std::string getQueryForRelationship(const std::string &column,
                                      const std::string &type, void *val);
  void getMapOfProperties(bson_t *data,
                          std::map<std::string, GenericObject> *map);
  void *getObject(bson_t *data, uint8_t *buf, uint32_t len,
                  const std::string &clasName);
  void storeProperty(ClassInfo *clas, void *t, void *colV, const Field &fe);
  void *getResults(const std::string &tableNm, Query &cquery, bson_t *fields,
                   bson_t *querySpec, const bool &isObj,
                   const bool &isCountQuery);
  void *getResults(const std::string &tableNm, QueryBuilder &qb, bson_t *fields,
                   bson_t *query, const bool &isObj);
  void _conn();
  mongoc_collection_t *_collection(const char *);
  void _release(mongoc_collection_t *);

public:
  DSType getType();
  MongoDBDataSourceImpl(ConnectionPooler *pool, Mapping *mapping);
  virtual ~MongoDBDataSourceImpl();
  bool startTransaction();
  bool commit();
  bool rollback();
  void procedureCall(const std::string &);
  void empty(const std::string &clasName);
  long getNumRows(const std::string &clasName);
  bool executeUpdate(Query &query);
  std::vector<std::map<std::string, GenericObject>> execute(QueryBuilder &qb);
  std::vector<std::map<std::string, GenericObject>> execute(Query &query);

protected:
  bool executeInsert(Query &query, void *entity);
  bool isGetDbEntityForBulkInsert();
  void *getDbEntityForBulkInsert(void *entity, const std::string &clasName,
                                 std::string &error);
  bool executeInsertBulk(Query &query, std::vector<void *> entities,
                         std::vector<void *> dbEntities);
  bool executeUpdateBulk(Query &query, std::vector<void *> entities,
                         std::vector<void *> dbEntities);
  bool executeUpdate(Query &query, void *entity);
  bool remove(const std::string &clasName, GenericObject &id);

  void *executeQuery(Query &query, const bool &isObj);
  void *executeQuery(QueryBuilder &qb, const bool &isObj);

  void executePreTable(DataSourceEntityMapping &dsemp, GenericObject &idv);
  void executePostTable(DataSourceEntityMapping &dsemp, GenericObject &idv);
  void executeSequence(DataSourceEntityMapping &dsemp, GenericObject &idv);
  void executeIdentity(DataSourceEntityMapping &dsemp, GenericObject &idv);
  void executeCustom(DataSourceEntityMapping &dsemp,
                     const std::string &customMethod, GenericObject &idv);

  bool startSession(void *);
  bool startSession();
  bool endSession();
};

#endif /* MONGODBDATASOURCEIMPL_H_ */
