FROM ubuntu:18.04


RUN apt update
RUN apt install -y software-properties-common
RUN add-apt-repository 'deb http://security.ubuntu.com/ubuntu xenial-security main'
RUN apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
RUN apt-get update
RUN apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev \
libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev \
libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev unzip
RUN apt install -y libjson-c-dev
RUN apt install -y mosquitto-clients libmosquitto-dev
RUN apt-get install -y libcanberra-gtk-module


RUN apt-get install -y wget
RUN apt-get install -y vim

RUN cd \
    && wget https://github.com/opencv/opencv/archive/3.2.0.zip \
    && unzip 3.2.0.zip \
    && cd opencv-3.2.0 \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make -j24 \
    && make install \
    && cd \
    && rm 3.2.0.zip

RUN cd \
    && wget https://github.com/opencv/opencv_contrib/archive/3.2.0.zip \
    && unzip 3.2.0.zip \
    && cd opencv-3.2.0/build \
    && cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-3.2.0/modules/ .. \
    && make -j24 \
    && make install \
    && cd ../.. \
    && rm 3.2.0.zip

RUN cd \
    && git clone https://github.com/oreillymedia/Learning-OpenCV-3_examples.git \
    && cd Learning-OpenCV-3_examples \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make -j8


RUN mkdir /workspace
WORKDIR /workspace

ENV LC_ALL C.UTF - 8
ENV LANG C.UTF - 8
ENV LANGUAGE C.UTF - 8


ENTRYPOINT ["/bin/bash", "-l"]