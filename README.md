# ms Converter

This code converts Newick trees/networks into a format accepted by the phylogenetic simulation software [`ms`](http://home.uchicago.edu/~rhudson1/source/mksamples.html).

---

## Examples

```bash
# Read by file and output to a file
./ms-converter newick-trees.file --output=ms-args.file

# Read by Newick and output to the command line
# This also shows some WARNINGS that ms-converter throws which can be silenced with --quiet
./ms-converter --newick="((1:0.1,((2:0.2,(3:0.3,(4:0.4)Y#H1:3.0)g:0.6)e:0.7,(((Y#H1:0.8,5:0.9)h:1.0,6:1.1)f:1.2)X#H2:1.3)c:1.4)a:1.5,((X#H2:0.4,7:1.7)d:1.8,8:1.9)b:2.0)r;"
```

The program can also directly run `ms` for you:

```bash
# Read Newick strings from the file newicks.file (new line deliminated), simulate under each Newick 50 times, and output all of the simulated trees to sim.trees
./ms-converter newicks.file --run --n 50 --output=sim.trees

# Do the same as above, but output the raw `ms` output to the terminal, not just the simulated trees
./ms-converter newicks.file --run -n 50 --dirty
```

---

## Present restrictions:

* In the Newick format provided to `ms-converter`, gamma must be specified on _both_ nodes involved in a hybrid event, and _all_ unspecified branch lengths are assumed to be 0.

---

## Compiling from Source

Requirements:
* [CMake](https://cmake.org/)
* [Boost C++ Libraries](https://boost.org) (v1.60.0 or greater) with at least the `program_options` library compiled. Installation instructions:
    * MacOSX, homebrew: `brew install boost`, MacPorts: `port install boost`
    * [Windows](https://valelab4.ucsf.edu/svn/3rdpartypublic/boost/more/getting_started/windows.html)
    * Ubuntu: `apt-get install libboost-all-dev`
* C++ compiler and standard libraries

Once these requirements are fulfilled, compile via the following commands:

```bash
git clone https://github.com/NathanKolbow/msConverter.git
cd msConverter
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The executable `ms-converter` will then be located in `build/ms-converter` (`build/Release/ms-converter.exe` on Windows)
