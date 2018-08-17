# YAGE

Yet Another Game Engine

### Build status
|Windows|Linux|Coverage
|:-------:|:------:|:------:|
|[![Build status](https://ci.appveyor.com/api/projects/status/d71ntjggmu4slj5m?svg=true)](https://ci.appveyor.com/project/MrJaqbq/yage) |[![Build Status](https://travis-ci.org/BentouDev/YAGE.svg?branch=master)](https://travis-ci.org/BentouDev/YAGE) |[![codecov](https://codecov.io/gh/BentouDev/YAGE/branch/master/graph/badge.svg)](https://codecov.io/gh/BentouDev/YAGE)|

### Tests status
[![Test status](https://flauschig.ch/batch.php?type=tests&account=MrJaqbq&slug=YAGE)](https://ci.appveyor.com/project/MrJaqbq/yage/build/tests)

## Usage
### Build
```
mkdir build
cd build
conan remote add yage https://api.bintray.com/conan/bentoudev/yage
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install ..
cmake ..
```

```
cmake install ..
```