FROM --platform=linux/arm64 ubuntu:noble

RUN apt-get update
RUN apt-get install -y git
RUN apt-get install -y cmake 
RUN apt-get install -y make 
RUN apt-get install -y bash 
RUN apt-get install -y zip 
RUN apt-get install -y libcurl4-gnutls-dev 
RUN apt-get install -y zlib1g 
RUN apt-get install -y zlib1g-dev 
RUN apt-get install -y ninja-build 
RUN apt-get install -y curl 
RUN apt-get install -y unzip 
RUN apt-get install -y perl 
RUN apt-get install -y build-essential
RUN apt-get install -y linux-libc-dev
RUN apt-get install -y pkg-config
RUN apt-get install -y libpthread-stubs0-dev
RUN apt-get install -y vim
WORKDIR /home
ENV VCPKG_FORCE_SYSTEM_BINARIES 1
RUN git -C vcpkg pull || git clone https://github.com/Microsoft/vcpkg.git
WORKDIR /home/vcpkg
RUN ./bootstrap-vcpkg.sh
ENV VCPKG_ROOT=/home/vcpkg
WORKDIR /home/root/_src
COPY ./vcpkg.json .
COPY ./vcpkg-configuration.json .
RUN  /home/vcpkg/vcpkg install
COPY ./src/ ./src/
COPY ./test/ ./test/
COPY CMakeLists.txt .
RUN  cmake -B build -S . -DVCPKG_BUILD_TYPE=release -DCMAKE_BUILD_TYPE=Release -DBUILD_TYPE=Release
#RUN  cmake -B build -S . 
WORKDIR /home/root/_src/build
RUN  cmake --build .
RUN ctest
RUN  cmake --build . --target aws-lambda-package-aws-slack-notification 
CMD [ "cp", "aws-slack-notification.zip", "/release_build/"]