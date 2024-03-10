FROM ubuntu:18.04

# Set timezone:
RUN ln -snf "/usr/share/zoneinfo/$CONTAINER_TIMEZONE" /etc/localtime && echo "$CONTAINER_TIMEZONE" > /etc/timezone

# install dependencies
RUN apt-get update && apt-get install -qq -y cmake python3 git && apt-get clean
RUN apt-get update && apt-get install -qq -y libedit-dev wget gnupg software-properties-common && apt-get clean
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update && apt-get install -qq -y gcc-9 g++-9 && apt-get clean

# install libxml2
RUN apt-get update && apt-get install -y libxml2 libxml2-dev libxml2++ libxml2++-dev && apt-get clean

ARG LLVM_VERSION="-18"
ARG LLVM_VERSION_NUM="18"

# add llvm repository
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic${LLVM_VERSION} main"
    # && apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial${LLVM_VERSION} main"

# install LLVM
# LLVM
RUN apt-get update && apt-get -y install libllvm${LLVM_VERSION}-ocaml-dev libllvm${LLVM_VERSION_NUM} llvm${LLVM_VERSION} llvm${LLVM_VERSION}-dev llvm${LLVM_VERSION}-doc llvm${LLVM_VERSION}-examples llvm${LLVM_VERSION}-runtime && apt-get clean
# Clang and co
RUN apt-get update && apt-get -y install clang${LLVM_VERSION} clang-tools${LLVM_VERSION} clang${LLVM_VERSION}-doc libclang-common${LLVM_VERSION}-dev libclang${LLVM_VERSION}-dev libclang1${LLVM_VERSION} clang-format${LLVM_VERSION} python3-clang${LLVM_VERSION} clangd${LLVM_VERSION} clang-tidy${LLVM_VERSION} && apt-get clean
# compiler-rt
# RUN apt-get update && apt-get -y install libclang-rt${LLVM_VERSION}-dev && apt-get clean
# polly
# RUN apt-get update && apt-get -y install libpolly${LLVM_VERSION}-dev && apt-get clean
# libfuzzer
# RUN apt-get update && apt-get -y install libfuzzer${LLVM_VERSION}-dev && apt-get clean
# lldb
# RUN apt-get update && apt-get -y install lldb${LLVM_VERSION} && apt-get clean
# lld (linker)
RUN apt-get update && apt-get -y install lld${LLVM_VERSION} && apt-get clean
# libc++
RUN apt-get update && apt-get -y install libc++${LLVM_VERSION}-dev libc++abi${LLVM_VERSION}-dev && apt-get clean
# OpenMP
# RUN apt-get update && apt-get -y install libomp${LLVM_VERSION}-dev && apt-get clean
# libunwind
RUN apt-get update && apt-get -y install libunwind${LLVM_VERSION}-dev && apt-get clean

# RUN apt-get update \
#     && apt-get -y install libllvm-12-ocaml-dev llvm-12-dev llvm-12-doc llvm-12-examples llvm-12-runtime \
#     && apt-get -y install clang-12 clang-tools-12 clang-12-doc libclang-common-12-dev libclang-12-dev libclang1-12 \
#     && apt-get -y install clang-format-12 python3-clang-12 clangd-12 lld-12 \
#     && apt-get clean
# RUN apt-get update \
#     && apt-get -y install libc++-12-dev libunwind-12-dev libc++abi-12-dev \
#     && apt-get clean

# create symlinks
RUN ln -sf /usr/lib/llvm${LLVM_VERSION}/lib/*.so /usr/lib/ \
    && ln -sf /usr/lib/llvm${LLVM_VERSION}/lib/*.so.${LLVM_VERSION_NUM} /usr/lib/

# create llvm-config symlink
RUN ln -sf /usr/bin/llvm-config${LLVM_VERSION} /usr/bin/llvm-config
RUN rm /usr/bin/ld && ln -s /usr/bin/ld.lld${LLVM_VERSION} /usr/bin/ld

# copy source code
COPY . /cdotc
WORKDIR /cdotc/utils

# # clean
# RUN rm -f include/cdotc/AST/Attributes.def include/cdotc/AST/Attr.inc \
#     && rm -rf include/cdotc/Parse/ParseAttr.inc include/cdotc/Diagnostics/Diagnostics.def \
#     && rm -rf include/cdotc/Query/Inc \
#     && rm -rf .build/ \
#     && rm -rf bin/

# # create build directory
# RUN mkdir -p .build

# # build CMake target
# WORKDIR /cdotc/.build
# RUN cmake .. -DCMAKE_BUILD_TYPE=REe
RUN mkdir guuux

# clean
RUN ./build_script --clean

# build
RUN ./build_script --test --debug --use-llvm /usr/bin/llvm-config${LLVM_VERSION} --use-cxx /usr/bin/clang++${LLVM_VERSION}

# This will keep the container running indefinitely until you manually stop it
CMD tail -f /dev/null