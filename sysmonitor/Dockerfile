FROM ubuntu:24.04 AS builder
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    libncurses5-dev \
    libncursesw5-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY monitor.cpp /src/monitor.cpp
RUN g++ -std=c++17 -O2 monitor.cpp -o /src/monitor -lncurses

FROM debian:stable-slim
RUN apt-get update && apt-get install -y libncurses6 && rm -rf /var/lib/apt/lists/*
COPY --from=builder /src/monitor /usr/local/bin/monitor

ENV PROC_ROOT=/proc
ENTRYPOINT ["/usr/local/bin/monitor"]

CMD []


