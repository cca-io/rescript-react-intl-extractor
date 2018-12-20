FROM ubuntu:18.04

RUN apt-get update \
  && DEBIAN_FRONTEND=noninteractive apt-get install -y nodejs npm git rsync m4 curl \
  && rm -rf /var/lib/apt/lists/*

RUN npm i -g esy@0.4.7
