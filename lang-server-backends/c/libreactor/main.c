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

#include "ffead-cpp.h"
#include <dynamic.h>
#include <err.h>
#include <reactor.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "setup.h"

static reactor_vector reactor_http_message_date_1(reactor_http *http) {
  time_t t;
  struct tm tm;
  static const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  static __thread uint64_t now, last = 0;
  static __thread char date[30] = "Thu, 01 Jan 1970 00:00:00 GMT";

  (void)http;
  now = reactor_core_now();
  if (now - last >= 1000000000) {
    last = now;
    (void)time(&t);
    (void)gmtime_r(&t, &tm);
    (void)strftime(date, 30, "---, %d --- %Y %H:%M:%S GMT", &tm);
    memcpy(date, days[tm.tm_wday], 3);
    memcpy(date + 8, months[tm.tm_mon], 3);
  }

  return (reactor_vector){date, 29};
}

static reactor_status handle(reactor_event *event) {
  reactor_server_session *session = (reactor_server_session *)event->data;

  ffead_request freq;
  freq.server_str = "libreactor";
  freq.server_str_len = 10;
  freq.method = (const char *)session->request->method.base;
  freq.method_len = session->request->method.size;
  freq.path = (const char *)session->request->target.base;
  freq.path_len = session->request->target.size;
  freq.body_len = 0;
  if (session->request->body.size > 0) {
    freq.body = (const char *)session->request->body.base;
    freq.body_len = session->request->body.size;
  }
  freq.version = 1;
  freq.headers_len = session->request->headers.count;

  phr_header_fcp f_headers[session->request->headers.count];
  for (int i = 0; i < (int)session->request->headers.count; i++) {
    f_headers[i].name =
        (const char *)session->request->headers.header[i].name.base;
    f_headers[i].name_len = session->request->headers.header[i].name.size;
    f_headers[i].value =
        (const char *)session->request->headers.header[i].value.base;
    f_headers[i].value_len = session->request->headers.header[i].value.size;
  }
  freq.headers = f_headers;

  int scode = 0;
  const char *out_url;
  size_t out_url_len;
  const char *out_body;
  size_t out_body_len;
  phr_header_fcp out_headers[100];
  size_t out_headers_len;
  const char *smsg;
  size_t smsg_len;

  void *fres = ffead_cpp_handle_c_1(&freq, &scode, &smsg, &smsg_len, &out_url,
                                    &out_url_len, out_headers, &out_headers_len,
                                    &out_body, &out_body_len);

  reactor_http_response response;
  response.version = session->request->version;
  if (scode > 0) {
    for (int i = 0; i < (int)out_headers_len; i++) {
      response.headers.header[i].name.base = (void *)out_headers[i].name;
      response.headers.header[i].name.size = out_headers[i].name_len;
      response.headers.header[i].value.base = (void *)out_headers[i].value;
      response.headers.header[i].value.size = out_headers[i].value_len;
    }
    response.headers.header[out_headers_len].name =
        reactor_vector_string("Date");
    response.headers.header[out_headers_len].value =
        reactor_http_message_date_1(NULL);
    response.headers.header[out_headers_len + 1].name =
        reactor_vector_string("Server");
    response.headers.header[out_headers_len + 1].value =
        reactor_vector_string("libreactor");
    response.headers.count = out_headers_len + 2;
    response.code = scode;
    response.reason.base = (void *)smsg;
    response.reason.size = smsg_len;
    if (out_body_len > 0) {
      response.body.base = (void *)out_body;
      response.body.size = out_body_len;
    }
    reactor_server_respond(session, &response);
  } else {
    if (out_url_len > 0 && access(out_url, F_OK) != -1) {
      char *buffer = 0;
      long length;
      FILE *f = fopen(out_url, "rb");

      if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
          fread(buffer, 1, length, f);
        }
        fclose(f);
        char slen[10];
        snprintf(slen, 9, "%ld", length);
        response.body.base = (void *)buffer;
        response.body.size = (size_t)length;
        response.headers.header[0].name.base = (void *)"Content-Length";
        response.headers.header[0].name.size = 14;
        response.headers.header[0].value.base = slen;
        response.headers.header[0].value.size = strlen(slen);
        response.headers.header[1].name.base = (void *)out_headers[0].name;
        response.headers.header[1].name.size = out_headers[0].name_len;
        response.headers.header[1].value.base = (void *)out_headers[0].value;
        response.headers.header[1].value.size = out_headers[0].value_len;
        response.headers.header[2].name.base = (void *)out_headers[1].name;
        response.headers.header[2].name.size = out_headers[1].name_len;
        response.headers.header[2].value.base = (void *)out_headers[1].value;
        response.headers.header[2].value.size = out_headers[1].value_len;
        response.headers.header[3].name = reactor_vector_string("Date");
        response.headers.header[3].value = reactor_http_message_date_1(NULL);
        response.headers.header[4].name = reactor_vector_string("Server");
        response.headers.header[4].value = reactor_vector_string("libreactor");
        response.headers.count = 5;
        response.code = 200;
        response.reason.base = (void *)"OK";
        response.reason.size = 2;
        reactor_server_respond(session, &response);
        free(buffer);
      } else {
        response.headers.header[0].name = reactor_vector_string("Date");
        response.headers.header[0].value = reactor_http_message_date_1(NULL);
        response.headers.header[1].name = reactor_vector_string("Server");
        response.headers.header[1].value = reactor_vector_string("libreactor");
        response.headers.count = 2;
        reactor_server_not_found(session);
      }
    } else {
      response.headers.header[0].name = reactor_vector_string("Date");
      response.headers.header[0].value = reactor_http_message_date_1(NULL);
      response.headers.header[1].name = reactor_vector_string("Server");
      response.headers.header[1].value = reactor_vector_string("libreactor");
      response.headers.count = 2;
      reactor_server_not_found(session);
    }
  }
  ffead_cpp_resp_cleanup(fres);
  return REACTOR_OK;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    // cout << "No Server root directory specified, quitting..." << std::endl;
    return 0;
  }

  printf("Bootstrapping ffead-cpp start...\n");
  ffead_cpp_bootstrap(argv[1], strlen(argv[1]), 7);
  printf("Bootstrapping ffead-cpp end...\n");

  printf("Initializing ffead-cpp start...\n");
  ffead_cpp_init();
  printf("Initializing ffead-cpp end...\n");

  reactor_server server;
  setup();
  reactor_construct();
  reactor_server_construct(&server, NULL, NULL);
  reactor_server_route(&server, handle, NULL);
  (void)reactor_server_open(&server, "0.0.0.0", argv[2]);
  printf("libreactor listening on port %s\n", argv[2]);
  reactor_run();
  reactor_destruct();

  printf("Cleaning up ffead-cpp start...\n");
  ffead_cpp_cleanup();
  printf("Cleaning up ffead-cpp end...\n");
}
