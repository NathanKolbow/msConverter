# ms Converter

## Contents

1. [Overview](#overview)
1. [Examples](#examples)
1. [Existing restrictions](#existing-restrictions)
1. [Installation](#compiling-from-source)

This code converts Newick trees/networks into a format accepted by the phylogenetic simulation software [`ms`](http://home.uchicago.edu/~rhudson1/source/mksamples.html).

---

## Overview

`msConverter` is a C++ program made to convert trees and networks in Newick format to the format accepted by [`ms`](https://home.uchicago.edu/~rhudson1/source/mksamples.html).

---

## Examples

```bash
# Read by file and output to a file
./ms-converter newick-trees.file --output=ms-args.file

# Read by Newick and output to the command line
# This also shows some WARNINGS that ms-converter throws which can be silenced with --quiet
./ms-converter --newick="((1:0.1,((2:0.2,(3:0.3,(4:0.4)Y#H1:3.0)g:0.6)e:0.7,(((Y#H1:0.8,5:0.9)h:1.0,6:1.1)f:1.2)X#H2:1.3)c:1.4)a:1.5,((X#H2:0.4,7:1.7)d:1.8,8:1.9)b:2.0)r;"
```

The program can also directly run `ms` for you (unless you're using Windows, sorry):

```bash
## If `ms` is in your system path, all you need to do is add --run

# Read Newick strings from the file newicks.file (new line deliminated), simulate under each Newick 50 times, and output all of the simulated trees to sim.trees
./ms-converter newicks.file --run --n 50 --output=sim.trees

# Do the same as above, but output the raw `ms` output to the terminal, not just the simulated trees
./ms-converter newicks.file --run -n 50 --dirty

## If `ms` is NOT in your system path, you have to tell the program where it is

# Do as above, but specify the location of `ms` (NOTE: this is the FOLDER CONTAINING ms, not ms itself)
./ms-converter newicks.file --run -n 50 --dirty --ms_path=/opt/msdir/
```

---

## Existing restrictions:

* In the Newick format provided to `ms-converter`, gamma must be specified on _both_ nodes involved in a hybrid event, and _all_ unspecified branch lengths are assumed to be 0.
* `ms` is designed to work with extant taxa, so only ultrametric trees and networks make sense within it. So, all input trees/networks are made ultrametric before translated to the `ms` format.

---

## Compiling from Source

Requirements:
* [CMake](https://cmake.org/)
* [Boost C++ Libraries](https://boost.org) (v1.60.0 or greater) with at least the `program_options` library compiled. Installation instructions:
    * MacOSX, homebrew: `brew install boost`, MacPorts: `port install boost`
    * [Windows](https://valelab4.ucsf.edu/svn/3rdpartypublic/boost/more/getting_started/windows.html)
    * Ubuntu: `apt-get install libboost-all-dev`
* C++ compiler and standard libraries (make sure these are up-to-date if compilation fails)

Once these requirements are fulfilled, compile via the following commands:

```bash
git clone https://github.com/NathanKolbow/msConverter.git
mkdir msConverter/build
cd msConverter/build
cmake ..
cmake --build .
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The executable `ms-converter` will then be located in `build/ms-converter` (`build/Release/ms-converter.exe` on Windows)
