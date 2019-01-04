#!/bin/bash

set -e
set -x

# BUILDTYPE=
echo BUILDTYPE ${BUILDTYPE}
TEST_TMPDIR=/tmp/bazel/ bazel build ${LOCALRESOURCES} ${BUILDTYPE} -c dbg  @grpc//:grpc_cpp_plugin
TEST_TMPDIR=/tmp/bazel/ bazel build ${LOCALRESOURCES} ${BUILDTYPE} -c dbg @protobuf//:protoc
mkdir -p bazel-out/host/bin/external/grpc
mkdir -p bazel-out/host/bin/external/protobuf
ln -sf  $PWD/bazel-bin/external/protobuf/protoc bazel-out/host/bin/external/protobuf/protoc
ln -sf $PWD/bazel-bin/external/grpc/grpc_cpp_plugin bazel-out/host/bin/external/grpc/grpc_cpp_plugin
TEST_TMPDIR=/tmp/bazel/ bazel build ${LOCALRESOURCES} ${BUILDTYPE} -c dbg --verbose_failures  --logging 6 //tensorflow/tools/pip_package:build_pip_package
