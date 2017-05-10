# Tensorflow-cl

Run Tensorflow on OpenCL™ 1.2 devices

## Summary

This repo was created from the original Tensorflow repository at:

- https://github.com/tensorflow/tensorflow

Please see the main repository for full Tensorflow documentation.  This readme will only focus on the OpenCL porting aspects of Tensorflow.

## Test results, on v0.14.0 wheel

| test | Intel HD5500, beignet 1.2.1 | NVIDIA 940M, driver v367.57 |
|----- |-------|-----|
| unit tests (`py.test -v`) | pass | pass |
| [linear_regression.py](https://github.com/hughperkins/TensorFlow-Examples/blob/enforce-gpu/examples/2_BasicModels/linear_regression.py) | slow, but works   | slow, but works   |
| [logistic_regression.py](https://github.com/hughperkins/TensorFlow-Examples/blob/enforce-gpu/examples/2_BasicModels/logistic_regression.py) | ok  | ok   |
| [nearest_neighbor.py](https://github.com/hughperkins/TensorFlow-Examples/blob/enforce-gpu/examples/2_BasicModels/nearest_neighbor.py) | ok (accuracy 0.92)  | ok (accuracy 0.92)   |
| [multilayer_perceptron.py](https://github.com/hughperkins/TensorFlow-Examples/blob/enforce-gpu/examples/3_NeuralNetworks/multilayer_perceptron.py) | missing adam  | missing adam |
| [recurrent_network.py](https://github.com/hughperkins/TensorFlow-Examples/blob/enforce-gpu/examples/3_NeuralNetworks/recurrent_network.py)| missing adam   |  missing adam  |
| [autoencoder.py](https://github.com/hughperkins/TensorFlow-Examples/blob/enforce-gpu/examples/3_NeuralNetworks/autoencoder.py)|  missing rmsprop  |    |

## Installation 

The environments used for testing/development are:
- Ubuntu 16.04, with:
  - NVIDIA K80, and
- Mac Sierra, with:
  - Intel HD Graphics 530
  - Radeon Pro 450
  (using a nice Mac Book Pro 4th generation that my employer [ASAPP](http://www.asapp.com) have provided me with recently :-) )

For Ubuntu 16.04, there is a wheel, that you can install from directly.
- You need:
  - the tensorflow non-gpu installation pre-requisites,
   - an OpenCL 1.2-enabled GPU, and  OpenCL 1.2-enabled drivers
   - python 3
- Simply download https://github.com/hughperkins/tensorflow-cl/releases/download/v0.14.0/tensorflow-0.11.0rc0-py3-none-any.whl , and
- Install using pip:
```
pip install --upgrade tensorflow-0.11.0rc0-py3-none-any.whl
```

For Mac OS X, I just started working on this in May, so no wheel as such yet, and it doesnt quite run yet.  You can [build from source](doc/build-from-source), but you might want to just choose [watch](https://github.com/hughperkins/tensorflow-cl/subscription) on this repository, and wait for the build to be working, and `wheel`d. Or you create an issue, to ask about Mac build, and I'll post Mac build updates into that issue.  Actually, I'll make the issue now, and you can just post/subscribe to it, if you wish [here](https://github.com/hughperkins/tensorflow-cl/issues/30).

More generally, if you want to participate in development, or just for fun, you can [build from source](doc/build-from-source.md)

## Testing


### Setup

```
pip install -r tensorflow/stream_executor/cl/test/requirements.txt
```

### Run

```
py.test -v
```

## Design/architecture

- tensorflow code stays 100% [NVIDIA® CUDA™](https://www.nvidia.com/object/cuda_home_new.html)
- [cuda-on-cl](https://github.com/hughperkins/cuda-on-cl) compiles the CUDA code into OpenCL
- Cedric Nugteren's [CLBlast](https://github.com/CNugteren/CLBlast) provides BLAS (matrix multiplications)

## Related projects

### DNN Libraries
- [OpenCL Torch](https://github.com/hughperkins/distro-cl)
- [DeepCL](https://github.com/hughperkins/DeepCL)

### OpenCL middleware
- [CLBlast](https://github.com/CNugteren/CLBlast) BLAS for OpenCL
- [cuda-on-cl](https://github.com/hughperkins/cuda-on-cl)  Compile CUDA apps for OpenCL
- [EasyCL](https://github.com/hughperkins/EasyCL)   Handles running kernels, passing in arguments etc, on OpenCL

## News

- May 9 2017:
  - Mac build runs ok :-)  See the release at [Mac build and wheel](https://github.com/hughperkins/tensorflow-cl/releases/tag/v0.15.0)
  - tested on Mac Sierra, using Radeon Pro 450 GPU
- May 2017:
  - My employer [ASAPP](http://asapp.com) have given me use of a nice Mac Book Pro 4th Generation, complete with Radeon Pro 450 GPU :-)  I've started looking into getting tensorflow-cl to build/run on it. Actually, it already builds. Just some small(-ish?) teething problems with getting it to run. Watch this space, or post/subscribe into [Mac build doesnt run yet](https://github.com/hughperkins/tensorflow-cl/issues/30) issue
- Dec 3:
  - BUILT A MAC WHEEL!!!  This is entirely untested.  But the wheel is here: https://s3.amazonaws.com/hughperkinstravis/cache/tensorflow-cl/travis/tensorflowpkg.tar.gz  (Simply untar it, and `pip install` it)
    - corresponding travis log is at https://travis-ci.org/hughperkins/tensorflow-cl/builds/180917138 and https://travis-ci.org/hughperkins/tensorflow-cl/builds/180410593
    - note that I had to built this in several stages, since it's a 3 hour build, and the logs for this are at https://s3.amazonaws.com/hughperkinstravis/cache/tensorflow-cl/travis/90-c520cc1-log.txt and https://s3.amazonaws.com/hughperkinstravis/cache/tensorflow-cl/travis/91-c55079d-log.txt
    - hmmm, doesnt seem to import yet though.   https://travis-ci.org/hughperkins/tensorflow-cl#L2419
- Nov 29:
  - Mac build ran to completion!  On Travis.  Build output https://travis-ci.org/hughperkins/tensorflow-cl/builds/179727517  Yes, it didnt run, didnt create the wheel.  But the `build_pip_package` target built to completion.  which is a huge step forward :-)  Travis script here: [.travis.yml](.travis.yaml)
- Nov 25:
  - release wheel [v0.14.0](https://github.com/hughperkins/tensorflow-cl/releases/download/v0.14.0/tensorflow-0.11.0rc0-py3-none-any.whl)
    - this fixes `argmin`, `argmax`, and `softmax`
    - tons of changes under-the-hood
- Nov 10:
  - released wheel [v0.13.0](https://github.com/hughperkins/tensorflow-cl/releases/download/v0.13.0/tensorflow-0.11.0rc0-py3-none-any.whl)
     - beignet test results fairly solidly match K520 results now
     - fixed the regression on `not_equal` operator
     - removed the spam from memory copy  
- Nov 9:
  - fixed unary and binary operators on beignet
  - note that the tools/bazel.rc.templ has changed.  Please make sure to copy the new value into tools/bazel.rc, or re-run configure (probably need to do `bazel clean` anyway, so might as well do `./configure`)
