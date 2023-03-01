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
 * FormHandler.cpp
 *
 *  Created on: Jun 17, 2012
 *      Author: Sumeet
 */

#include "FormHandler.h"

bool FormHandler::handle(HttpRequest *req, HttpResponse *res,
                         Reflector &reflector, Element *ele) {
  // Logger logger = LoggerFactory::getLogger("FormHandler");
  // logger << ele->getTagName() << std::endl;
  // logger << ele->render() << std::endl;
  ClassInfo *clas = ConfigurationData::getClassInfo(ele->getAttribute("bean"),
                                                    req->getCntxt_name());
  ElementList eles = ele->getChildElements();
  std::string json = "{";
  for (unsigned int apps = 0; apps < eles.size(); apps++) {
    if (eles.at(apps).getTagName() == "field") {
      std::string name = eles.at(apps).getAttribute("name");
      Field fld = clas->getField(eles.at(apps).getAttribute("prop"));
      if (fld.getType() == "std::string" || fld.getType() == "string")
        json += "\"" + eles.at(apps).getAttribute("prop") + "\": \"" +
                req->getParamValue(name) + "\",";
      else {
        if (fld.getType() == "short" || fld.getType() == "unsigned short" ||
            fld.getType() == "int" || fld.getType() == "unsigned int" ||
            fld.getType() == "long" || fld.getType() == "unsigned long" ||
            fld.getType() == "char" || fld.getType() == "unsigned char") {
          if (req->getQueryParam(name) == "")
            json += "\"" + eles.at(apps).getAttribute("prop") + "\": 0,";
          else
            json += "\"" + eles.at(apps).getAttribute("prop") +
                    "\": " + req->getParamValue(name) + ",";
        } else if (fld.getType() == "double" || fld.getType() == "float") {
          if (req->getQueryParam(name) == "")
            json += "\"" + eles.at(apps).getAttribute("prop") + "\": 0.0,";
          else
            json += "\"" + eles.at(apps).getAttribute("prop") +
                    "\": " + req->getParamValue(name) + ",";
        } else if (fld.getType() == "bool") {
          if (req->getQueryParam(name) == "")
            json += "\"" + eles.at(apps).getAttribute("prop") + "\": false,";
          else
            json += "\"" + eles.at(apps).getAttribute("prop") +
                    "\": " + req->getParamValue(name) + ",";
        }
      }
    }
  }
  if (json.find(",") != std::string::npos) {
    json = json.substr(0, json.length() - 1);
  }
  json += "}";
  // logger << json << std::endl;

  void *_temp = ConfigurationData::getInstance()->ffeadContext.getBean(
      ele->getAttribute("controller"), req->getCntxt_name());
  args argus;
  argus.push_back("void*");
  argus.push_back("HttpResponse*");
  vals valus;
  ClassInfo *srv = ConfigurationData::getClassInfo(
      ele->getAttribute("controller"), req->getCntxt_name());
  Method meth = srv->getMethod("onSubmit", argus);
  if (meth.getMethodName() != "") {
    int serOpt = SerializeBase::identifySerOption(ele->getAttribute("bean"));
    // logger << ("Fetching Formcontroller for " + ele->getAttribute("bean")) <<
    // std::endl;
    void *_beaninst = JSONSerialize::unSerializeUnknown(
        json, serOpt, ele->getAttribute("bean"),
        std::string(req->getCntxt_name()));
    valus.push_back(_beaninst);
    valus.push_back(res);
    reflector.invokeMethodGVP(_temp, meth, valus, false);
    reflector.destroy(_beaninst, ele->getAttribute("bean"));
    // logger << "Successfully called Formcontroller" << std::endl;
    ConfigurationData::getInstance()->ffeadContext.release(
        _temp, ele->getAttribute("controller"), req->getCntxt_name());
    return true;
  }
  ConfigurationData::getInstance()->ffeadContext.release(
      _temp, ele->getAttribute("controller"), req->getCntxt_name());
  return false;
}
