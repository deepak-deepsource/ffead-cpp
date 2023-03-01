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
 * ConfiguartionData.cpp
 *
 *  Created on: 19-Jun-2012
 *      Author: sumeetc
 */

#include "ConfigurationData.h"

ConfigurationData *ConfigurationData::instance = NULL;

Reflector *ConfigurationData::getReflector() {
  return &(getInstance()->reflector);
}

void ConfigurationData::enableFeatures(
    bool enableCors, bool enableSecurity, bool enableFilters,
    bool enableControllers, bool enableContMpg, bool enableContPath,
    bool enableContExt, bool enableContRst, bool enableExtra,
    bool enableScripts, bool enableSoap, bool enableLogging,
    bool enableExtControllers, bool enableJobs, bool enableStaticResponses) {
  getInstance()->enableCors = enableCors;
  getInstance()->enableSecurity = enableSecurity;
  getInstance()->enableFilters = enableFilters;
  getInstance()->enableControllers = enableControllers;
  getInstance()->enableContMpg = enableContMpg;
  getInstance()->enableContPath = enableContPath;
  getInstance()->enableContExt = enableContExt;
  getInstance()->enableContRst = enableContRst;
  getInstance()->enableExtra = enableExtra;
  getInstance()->enableScripts = enableScripts;
  getInstance()->enableSoap = enableSoap;
  getInstance()->enableLogging = enableLogging;
  getInstance()->enableExtControllers = enableExtControllers;
  getInstance()->enableJobs = enableJobs;
  getInstance()->enableStaticResponses = enableStaticResponses;
}

ConfigurationData::ConfigurationData() {
  logger = LoggerFactory::getLogger("ConfigurationData");
  dlib = NULL;
  ddlib = NULL;
  enableCors = false;
  enableSecurity = false;
  enableFilters = false;
  enableControllers = false;
  enableExtControllers = false;
  enableContMpg = false;
  enableContPath = false;
  enableContExt = false;
  enableContRst = false;
  enableExtra = false;
  enableScripts = false;
  enableSoap = false;
  enableLogging = false;
  enableJobs = false;
  enableStaticResponses = false;
  serverType = SERVER_BACKEND::EMBEDDED;
}

ConfigurationData *ConfigurationData::getInstance() {
  if (instance == NULL) {
    instance = new ConfigurationData;
  }
  return instance;
}

SecurityProperties const &ConfigurationData::getSecurityProperties() {
  return (getInstance()->securityProperties);
}

CoreServerProperties const &ConfigurationData::getCoreServerProperties() {
  return (getInstance()->coreServerProperties);
}

bool ConfigurationData::isServingContext(std::string_view cntxtName) {
  return getInstance()->servingContexts.find(cntxtName) !=
             getInstance()->servingContexts.end() &&
         getInstance()->servingContexts.find(cntxtName)->second;
}

ClassInfo *ConfigurationData::getClassInfo(const std::string &cs,
                                           const std::string &app) {
  return getInstance()->ffeadContext.reflector->getClassInfo(cs, app);
}

ClassInfo *ConfigurationData::getClassInfo(const std::string &cs,
                                           std::string_view app) {
  return getInstance()->ffeadContext.reflector->getClassInfo(cs, app);
}

void ConfigurationData::clearAllSingletonBeans() {
  getInstance()->ffeadContext.clearAllSingletonBeans(
      getInstance()->servingContexts);
}
void ConfigurationData::setCoreServerProperties(
    CoreServerProperties coreServerProperties) {
  getInstance()->coreServerProperties = coreServerProperties;
}

void ConfigurationData::initializeAllSingletonBeans() {
  getInstance()->dlib = dlopen(INTER_LIB_FILE, RTLD_NOW);
  if (getInstance()->dlib == NULL) {
    std::cout << dlerror() << std::endl;
    std::cout << "Could not load Library" << std::endl;
    exit(0);
  }
  getInstance()->reflector = Reflector(getInstance()->dlib);

  getInstance()->ddlib = dlopen(DINTER_LIB_FILE, RTLD_NOW);
  if (getInstance()->ddlib == NULL) {
    std::cout << dlerror() << std::endl;
    std::cout << "Could not load dynamic Library" << std::endl;
    exit(0);
  }
  TemplateUtil::init(getInstance()->ddlib);
  SerializeBase::init(ConfigurationData::getInstance()->dlib);

  std::map<std::string, std::string, std::less<>>::iterator it =
      ConfigurationData::getInstance()->servingContextRouterNames.begin();
  for (;
       it != ConfigurationData::getInstance()->servingContextRouterNames.end();
       ++it) {
    std::string rt = it->second;
    std::vector<std::string> prts =
        StringUtil::splitAndReturn<std::vector<std::string>>(rt, ";");
    // std::string scappName =
    // ConfigurationData::getInstance()->servingContextAppNames[prts[1]];

    Bean bean(prts[0], "", prts[0], "singleton", false, false, prts[1]);
    ConfigurationData::getInstance()->ffeadContext.addBean(bean);
  }

  getInstance()->ffeadContext.initializeAllSingletonBeans(
      getInstance()->servingContexts, &(getInstance()->reflector));

  it = ConfigurationData::getInstance()->servingContextRouterNames.begin();
  for (;
       it != ConfigurationData::getInstance()->servingContextRouterNames.end();
       ++it) {
    std::string rt = it->second;
    std::vector<std::string> prts =
        StringUtil::splitAndReturn<std::vector<std::string>>(rt, ";");
    std::string scappName =
        ConfigurationData::getInstance()->servingContextAppNames[prts[1]];

    CommonUtils::setAppName(scappName);

    ClassInfo *rtcls =
        getInstance()->reflector.getClassInfo(prts[0], scappName);
    if (rtcls->getClassName() != "") {
      void *_temp = ConfigurationData::getInstance()->ffeadContext.getBean(
          prts[0], prts[1]);
      Router *router = (Router *)_temp;
      ConfigurationData::getInstance()->servingContextRouters[prts[1]] = router;
    }
  }

  std::map<std::string,
           std::map<std::string, std::vector<RestFunction>, std::less<>>,
           std::less<>> &rstCntMap =
      ConfigurationData::getInstance()->rstCntMap;
  std::map<std::string,
           std::map<std::string, std::vector<RestFunction>, std::less<>>,
           std::less<>>::iterator rsit = rstCntMap.begin();
  for (; rsit != rstCntMap.end(); ++rsit) {
    resFuncMap &rstFMp = rsit->second;
    resFuncMap::iterator rfit = rstFMp.begin();
    for (; rfit != rstFMp.end(); ++rfit) {
      std::vector<RestFunction> &fncl = rfit->second;
      for (int var = 0; var < (int)fncl.size(); ++var) {
        RestFunction &rf = fncl.at(var);
        std::string vtyp = rf.rtype;
        std::string type;
        int serOpt = rf.serOpt > 2000
                         ? rf.serOpt - 2000
                         : (rf.serOpt > 1000 ? rf.serOpt - 1000 : rf.serOpt);
        if (serOpt % 100 == 0) {
          std::string className = rf.rtype;
          if (serOpt == 100) {
            type = "vector";
            StringUtil::replaceFirst(className, "std::vector<", "");
            StringUtil::replaceFirst(className, "vector<", "");
            if (className.find(",") != std::string::npos)
              vtyp = className.substr(0, className.find(","));
            else
              vtyp = className.substr(0, className.find(">"));
          } else if (serOpt == 200) {
            type = "list";
            StringUtil::replaceFirst(className, "std::list<", "");
            StringUtil::replaceFirst(className, "list<", "");
            if (className.find(",") != std::string::npos)
              vtyp = className.substr(0, className.find(","));
            else
              vtyp = className.substr(0, className.find(">"));
          } else if (serOpt == 300) {
            type = "set";
            StringUtil::replaceFirst(className, "std::set<", "");
            StringUtil::replaceFirst(className, "set<", "");
            if (className.find(",") != std::string::npos)
              vtyp = className.substr(0, className.find(","));
            else
              vtyp = className.substr(0, className.find(">"));
          } else if (serOpt == 400) {
            type = "multiset";
            StringUtil::replaceFirst(className, "std::multiset<", "");
            StringUtil::replaceFirst(className, "multiset<", "");
            if (className.find(",") != std::string::npos)
              vtyp = className.substr(0, className.find(","));
            else
              vtyp = className.substr(0, className.find(">"));
          } else if (serOpt == 500) {
            type = "queue";
            StringUtil::replaceFirst(className, "std::queue<", "");
            StringUtil::replaceFirst(className, "queue<", "");
            if (className.find(",") != std::string::npos)
              vtyp = className.substr(0, className.find(","));
            else
              vtyp = className.substr(0, className.find(">"));
          } else if (serOpt == 600) {
            type = "deque";
            StringUtil::replaceFirst(className, "std::deque<", "");
            StringUtil::replaceFirst(className, "deque<", "");
            if (className.find(",") != std::string::npos)
              vtyp = className.substr(0, className.find(","));
            else
              vtyp = className.substr(0, className.find(">"));
          }
          StringUtil::replaceAll(vtyp, "::", "_");
          std::string appn = rsit->first;
          StringUtil::replaceAll(appn, "-", "_");
          RegexUtil::replace(appn, "[^a-zA-Z0-9_]+", "");
          rf.s = Serializer::getSerFuncForObject(vtyp, appn);
          rf.sc = Serializer::getSerFuncForObjectCont(vtyp, appn, "vector");
          rf.scm = Serializer::getSerFuncForObjectCont(vtyp, appn, "set");
          rf.us = Serializer::getUnSerFuncForObject(vtyp, appn);
          rf.usc = Serializer::getUnSerFuncForObjectCont(vtyp, appn, "vector");
          rf.uscm = Serializer::getUnSerFuncForObjectCont(vtyp, appn, "set");
        }
        for (int var1 = 0; var1 < (int)rf.params.size(); ++var1) {
          RestFunctionParams &rfp = rf.params.at(var1);
          if (rfp.serOpt % 100 == 0) {
            std::string className = rfp.type;
            if (rfp.serOpt == 100) {
              type = "vector";
              StringUtil::replaceFirst(className, "std::vector<", "");
              StringUtil::replaceFirst(className, "vector<", "");
              if (className.find(",") != std::string::npos)
                vtyp = className.substr(0, className.find(","));
              else
                vtyp = className.substr(0, className.find(">"));
            } else if (rfp.serOpt == 200) {
              type = "list";
              StringUtil::replaceFirst(className, "std::list<", "");
              StringUtil::replaceFirst(className, "list<", "");
              if (className.find(",") != std::string::npos)
                vtyp = className.substr(0, className.find(","));
              else
                vtyp = className.substr(0, className.find(">"));
            } else if (rfp.serOpt == 300) {
              type = "set";
              StringUtil::replaceFirst(className, "std::set<", "");
              StringUtil::replaceFirst(className, "set<", "");
              if (className.find(",") != std::string::npos)
                vtyp = className.substr(0, className.find(","));
              else
                vtyp = className.substr(0, className.find(">"));
            } else if (rfp.serOpt == 400) {
              type = "multiset";
              StringUtil::replaceFirst(className, "std::multiset<", "");
              StringUtil::replaceFirst(className, "multiset<", "");
              if (className.find(",") != std::string::npos)
                vtyp = className.substr(0, className.find(","));
              else
                vtyp = className.substr(0, className.find(">"));
            } else if (rfp.serOpt == 500) {
              type = "queue";
              StringUtil::replaceFirst(className, "std::queue<", "");
              StringUtil::replaceFirst(className, "queue<", "");
              if (className.find(",") != std::string::npos)
                vtyp = className.substr(0, className.find(","));
              else
                vtyp = className.substr(0, className.find(">"));
            } else if (rfp.serOpt == 600) {
              type = "deque";
              StringUtil::replaceFirst(className, "std::deque<", "");
              StringUtil::replaceFirst(className, "deque<", "");
              if (className.find(",") != std::string::npos)
                vtyp = className.substr(0, className.find(","));
              else
                vtyp = className.substr(0, className.find(">"));
            }
            StringUtil::replaceAll(vtyp, "::", "_");
            std::string appn = rsit->first;
            StringUtil::replaceAll(appn, "-", "_");
            RegexUtil::replace(appn, "[^a-zA-Z0-9_]+", "");
            rf.s = Serializer::getSerFuncForObject(vtyp, appn);
            rf.sc = Serializer::getSerFuncForObjectCont(vtyp, appn, "vector");
            rf.scm = Serializer::getSerFuncForObjectCont(vtyp, appn, "set");
            rf.us = Serializer::getUnSerFuncForObject(vtyp, appn);
            rf.usc =
                Serializer::getUnSerFuncForObjectCont(vtyp, appn, "vector");
            rf.uscm = Serializer::getUnSerFuncForObjectCont(vtyp, appn, "set");
          }
        }
      }
    }
  }
}

void ConfigurationData::clearInstance() {
  if (instance != NULL) {
    delete instance;
  }
}

propMap ConfigurationData::getAppProperties(const std::string &name) {
  if (name != "" && getInstance()->appPropertiesMap.find(name) !=
                        getInstance()->appPropertiesMap.end()) {
    return getInstance()->appPropertiesMap.find(name)->second;
  }
  std::string aname = CommonUtils::getAppName(name);
  if (getInstance()->appPropertiesMap.find(aname) !=
      getInstance()->appPropertiesMap.end()) {
    return getInstance()->appPropertiesMap.find(aname)->second;
  }
  return propMap();
}

ConfigurationData::~ConfigurationData() {
  if (dlib != NULL) {
    dlclose(dlib);
  }
  if (ddlib != NULL) {
    dlclose(ddlib);
  }
}

Security::Security() {
  sessTimeout = 0;
  logger = LoggerFactory::getLogger("Security");
}

Security::~Security() {}

bool Security::isLoginConfigured() {
  return (loginProvider != "" && loginUrl != "");
}
bool Security::isSecureConfigured() { return secures.size() != 0; }
bool Security::isLoginUrl(std::string_view url, const std::string &actUrl) {
  return (actUrl == (std::string(url) + "/_ffead_security_cntxt_login_url"));
}
bool Security::isLoginPage(std::string_view cntxtName,
                           const std::string &actUrl) {
  return actUrl == loginUrl;
}

bool Security::addAspect(const SecureAspect &aspect) {
  std::string pathurl = aspect.path;
  StringUtil::trim(pathurl);
  RegexUtil::replace(pathurl, "[/]+", "/");
  if (secures.find("*") != secures.end()) {
    logger << ("Already found secure path mapping for * (All paths), cannot "
               "add any further secure paths, skipping...")
           << std::endl;
    return false;
  } else if (secures.find(pathurl) == secures.end()) {
    bool endval = false, startval = false;
    if (pathurl.at(pathurl.length() - 1) == '*') {
      pathurl = pathurl.substr(0, pathurl.length() - 1);
      startval = true;
    }
    if (pathurl.at(0) == '*') {
      pathurl = pathurl.substr(1);
      endval = true;
    }
    bool exists = false;
    std::string url;
    std::map<std::string, SecureAspect>::iterator it;
    for (it = secures.begin(); it != secures.end(); ++it) {
      url = it->first;
      if (startval && url.find(pathurl) == 0) {
        exists = true;
      } else if (endval && StringUtil::endsWith(url, pathurl)) {
        exists = true;
      } else if (startval && endval && url.find(pathurl) != std::string::npos) {
        exists = true;
      }
    }
    if (!exists) {
      secures[pathurl] = aspect;
      return true;
    } else {
      logger << ("Already found secure path mapping for " + pathurl + "(" +
                 url + "), skipping...")
             << std::endl;
      return false;
    }
  } else {
    logger << ("Duplicate secure aspect found for path" + pathurl +
               ", skipping...")
           << std::endl;
    return false;
  }
}

SecureAspect Security::matchesPath(std::string_view cntxtName,
                                   std::string url) {
  SecureAspect aspect;
  std::map<std::string, SecureAspect>::iterator it;
  for (it = secures.begin(); it != secures.end(); ++it) {
    SecureAspect secureAspect = it->second;
    std::string pathurl = secureAspect.path;
    if (ConfigurationData::urlMatchesPath(cntxtName, pathurl, url)) {
      aspect = secureAspect;
    }
  }
  return aspect;
}

bool ConfigurationData::urlMatchesPath(std::string_view cntxtName,
                                       std::string pathurl, std::string url) {
  if (pathurl == url) {
    return true;
  } else if (StringUtil::startsWith(pathurl, "regex(") &&
             StringUtil::endsWith(pathurl, ")")) {
    std::string regurl = pathurl.substr(6, pathurl.length() - 7);
    if (RegexUtil::find(url, regurl) != -1) {
      return true;
    }
    return false;
  } else if (pathurl == "*") {
    return true;
  } else if (pathurl == "*.*" && url.find(".") != std::string::npos) {
    return true;
  }

  bool urlextmtch = true;
  if (pathurl.find("*.") != std::string::npos) {
    std::string pathext, urlext;
    size_t indx = pathurl.find("*.");
    pathext = pathurl.substr(indx + 1);

    if (url.find(".") == std::string::npos) {
      return false;
    }

    if (indx == 0) {
      urlext = url.substr(url.find("."));
      if (pathext == urlext) {
        return true;
      }
      return false;
    } else {
      pathurl = pathurl.substr(0, pathurl.find("*.") + 1);
      urlext = url.substr(url.find("."));
      url = url.substr(0, url.find("."));
      urlextmtch = pathext == urlext;
    }
  } else if (StringUtil::endsWith(pathurl, ".*")) {
    if (url.find(".") == std::string::npos) {
      return false;
    }

    pathurl = pathurl.substr(0, pathurl.length() - 2);
    url = url.substr(0, url.find("."));
  }

  bool endval = false, startval = false;
  if (pathurl.at(pathurl.length() - 1) == '*') {
    pathurl = pathurl.substr(0, pathurl.length() - 1);
    startval = true;
  }
  if (pathurl.at(0) == '*') {
    pathurl = pathurl.substr(1);
    endval = true;
  }

  if (urlextmtch && startval && endval &&
      url.find(pathurl) != std::string::npos) {
    return true;
  } else if (urlextmtch && startval && url.find(pathurl) == 0) {
    return true;
  } else if (urlextmtch && endval && StringUtil::endsWith(url, pathurl)) {
    return true;
  } else if (urlextmtch && !startval && !endval && pathurl == url) {
    return true;
  }
  return false;
}

HttpRequest *ConfigurationData::getHttpRequest() {
  return getInstance()->httpRequest.getPointer<HttpRequest>();
}

HttpResponse *ConfigurationData::getHttpResponse() {
  return getInstance()->httpRequest.getPointer<HttpResponse>();
}

SERVER_BACKEND ConfigurationData::getServerType() {
  return getInstance()->serverType;
}

int ConfigurationData::getProcessId() { return getpid(); }

bool ConfigurationData::isJobsEnabled() { return getInstance()->enableJobs; }
