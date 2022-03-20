FROM debian:buster

WORKDIR /tmp

# Install general dependencies
RUN apt-get update && apt-get -y --no-install-recommends install \
`# General dependencies` \
  #ca-certificates \
  build-essential \
  cmake \
  git \
  wget

RUN apt-get -y --no-install-recommends install \
`# C++ dependencies` \
  googletest \
  libboost-all-dev \
  libboost-program-options1.67.0 \
  libfmt-dev

# Cleanup
RUN apt-get clean && \
  rm -rf /var/lib/apt/lists/* && \
  rm -rf /tmp/* && \
  rm -rf /var/tmp/*

WORKDIR /boxed-in-solver/build
