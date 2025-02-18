FROM sumeetchhetri/ffead-cpp-sql-raw-async-profiled-base:6.1

RUN apt remove -yqq postgresql-${PG_VERSION} postgresql-contrib-${PG_VERSION} gnupg lsb-release && apt autoremove -yqq
RUN rm -rf /ssd/postgresql && rm -rf /tmp/postgresql && rm -rf /tmp/wrk /usr/local/bin/wrk

ENV IROOT=/installs

WORKDIR /

CMD ./run_ffead.sh ffead-cpp-6.0-sql emb postgresql-raw-async memory
