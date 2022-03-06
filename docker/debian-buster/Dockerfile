FROM debian:buster

# Install packages
RUN apt-get update \
  && apt-get -y --no-install-recommends install \
  build-essential \
  cmake \
  git \
  libboost-all-dev \
  libboost-program-options1.67.0 \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR /boxed-in-solver/build
