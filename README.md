# YAGE

Yet Another Game Engine

> Work in progress

### Build status
|Windows|Linux|Coverage
|:-------:|:------:|:------:|
|[![Build status](https://ci.appveyor.com/api/projects/status/d71ntjggmu4slj5m/branch/master?svg=true)](https://ci.appveyor.com/project/MrJaqbq/yage/branch/master) |[![Build Status](https://travis-ci.org/MrJaqbq/YAGE.svg?branch=master)](https://travis-ci.org/MrJaqbq/YAGE) |[![codecov](https://codecov.io/gh/MrJaqbq/YAGE/branch/master/graph/badge.svg)](https://codecov.io/gh/MrJaqbq/YAGE)|

### Tests status
[![Test status](http://flauschig.ch/batch.php?type=tests&account=MrJaqbq&slug=YAGE)](https://ci.appveyor.com/project/MrJaqbq/yage/build/tests)

# Features
## Entity Component System

YAGE has very fast entity component system. Below are sample benchmark results from Manjaro Linux 64bit on Intel i3-3120M @ 2.5GHz

[small excel table will go here]

[image will go here]

## Custom Memory Allocators

YAGE desing aims to allocate one big chunk of memory at start of the application, without any additional allocations during runtime. Engine manages that chunk using MemoryBlock and Allocators.
This is heavily inspired by memory management articles on [autodesk stirgraph] and [molecular musings blogs].

```C++
uint32_t chunk_size = 1024 * 1024;
void* chunk = malloc(chunk_size);

Memory::IMemoryBlock* block = new Memory::MemoryBlock(chunk, chunk_size);

auto someObj = YAGE_CREATE_NEW(block, SomeClass)(/* Constructor Parameters */);

// ... 

Memory::SafeDelete(block, someObj);

```

MemoryBlock creation is done using MemoryManager, that watches for possible memory leaks on block level. Additionaly MemoryBlock allows to specify additional checks using policy pattern, like bound checking, single allocation tracking and valgrind support.
