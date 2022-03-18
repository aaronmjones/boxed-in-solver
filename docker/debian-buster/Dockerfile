FROM debian:buster

WORKDIR /tmp

# Install general dependencies
RUN apt-get update && apt-get -y --no-install-recommends install \
`# General dependencies` \
  #ca-certificates \
  build-essential \
  cmake \
  git \
  googletest \
  wget

# # CMake
# RUN wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz && \
#   tar -zxvf cmake-3.15.2.tar.gz && \
#   cd cmake-3.15.2 && \
#   ./bootstrap && \
#   make && \
#   sudo make install && \
#   cd .. && \
#   rm -rf cmake-3.15.2

RUN apt-get -y --no-install-recommends install \
`# C++ dependencies` \
  googletest \
  libboost-all-dev \
  libboost-program-options1.67.0

# Cleanup
RUN apt-get clean && \
  rm -rf /var/lib/apt/lists/* && \
  rm -rf /tmp/* && \
  rm -rf /var/tmp/*

WORKDIR /boxed-in-solver/build
