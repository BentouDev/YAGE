# YAGE

Yet Another Game Engine

### Build status
|Windows (VS2017)|Windows (VS2019)|Linux|
|:-------:|:------:|:--:|
|[![Build status](https://ci.appveyor.com/api/projects/status/d71ntjggmu4slj5m?svg=true)](https://ci.appveyor.com/project/MrJaqbq/yage) |[![Build Status](https://dev.azure.com/bentoo/Yage%20CI/_apis/build/status/Yage%20CI-CI?branchName=master)](https://dev.azure.com/bentoo/Yage%20CI/_build/latest?definitionId=5&branchName=master)|[![Build Status](https://travis-ci.org/BentouDev/YAGE.svg?branch=master)](https://travis-ci.org/BentouDev/YAGE) |

### Tests 
|Status|Coverage|
|:--:|:--:|
|[![Test status](https://flauschig.ch/batch.php?type=tests&account=MrJaqbq&slug=YAGE)](https://ci.appveyor.com/project/MrJaqbq/yage/build/tests)|[![codecov](https://codecov.io/gh/BentouDev/YAGE/branch/master/graph/badge.svg)](https://codecov.io/gh/BentouDev/YAGE)|

## Build from source
### Build
```bash
# out of source build
mkdir build
cd build
cmake ..
```
### Installation
```bash
cmake install ..
```
### Packaging
```bash
cpack -G ZIP
```