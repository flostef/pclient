FROM ubuntu:22.04 as base
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y

FROM base as build
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y build-essential clang-12 cmake libboost-dev libssl-dev libworkflow-dev
COPY . /pclient
RUN ls /usr/local/lib/
RUN set -ex && cd /pclient && cmake . && make

FROM base as runtime
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y libssl-dev libworkflow-dev
COPY --from=build /pclient/pclient /pclient
ENV LD_LIBRARY_PATH /usr/local/lib
ENTRYPOINT ["/pclient"]