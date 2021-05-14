FROM ubuntu:latest

RUN apt-get update && \
    apt-get install software-properties-common -y && \
    add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get install -y tzdata && \
    apt-get install -y \
    build-essential \
    fakeroot \
    git \
    make \
    cmake \
    valgrind \
    python-all-dev \
    python3-dev \
    python3-pip \
    python3-setuptools \
    sudo \
    curl \
    wget \
    cppcheck \
    gcc-11 --no-install-recommends \
    g++-11 \
    libsdl2-dev \
    && rm -rf /var/lib/apt/lists/*

RUN curl -LSso toolchain.tar.bz2 \ 
    https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2 && \
    tar -xjvf toolchain.tar.bz2

ENV PATH="/gcc-arm-none-eabi-10-2020-q4-major/bin:${PATH}"

RUN pip3 install jinja2 pygments wheel
RUN pip3 install pros-cli

ENV LC_ALL C.UTF-8
ENV LANG C.UTF-8

WORKDIR /
