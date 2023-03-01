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
 * SSLCommon.h
 *
 *  Created on: 19-Dec-2013
 *      Author: sumeetc
 */

#ifndef SSLCOMMON_H_
#define SSLCOMMON_H_
#include "Compatibility.h"
#include <string>
#include <strings.h>
#ifndef OS_MINGW
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <iostream>
/*HTTPS related*/
#include <openssl/err.h>
#include <openssl/opensslv.h>
#include <openssl/ssl.h>
#include <signal.h>

class SSLCommon {
  SSLCommon();

public:
  static std::string ciphers;
  virtual ~SSLCommon();
  static void exitSSL(const char *func);
  static void *zeroingMalloc(size_t howmuch);
  static void load_dh_params(SSL_CTX *ctx, char *file);
  static void load_ecdh_params(SSL_CTX *ctx);
  static SSL_CTX *initialize_ctx(const bool &);
  static void loadCerts(SSL_CTX *ctx, char *certFile, char *keyFile,
                        const std::string &caList, const bool &);
  static void check_cert(SSL *ssl, char *host);
  static void closeSSL(const int &fd, SSL *ssl, BIO *bio);
};

#endif /* SSLCOMMON_H_ */
