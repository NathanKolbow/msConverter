# ms Converter

This code converts Newick trees/networks into a format accepted by the phylogenetic simulation software [`ms`](http://home.uchicago.edu/~rhudson1/source/mksamples.html).

### Example

```bash
# Read by file and output to a file
./ms-converter newick-trees.file --output=ms-args.file

# Read by Newick and output to the command line
./ms-converter --newick="((1:0.1,((2:0.2,(3:0.3,(4:0.4)Y#H1:3.0)g:0.6)e:0.7,(((Y#H1:0.8,5:0.9)h:1.0,6:1.1)f:1.2)X#H2:1.3)c:1.4)a:1.5,((X#H2:0.4,7:1.7)d:1.8,8:1.9)b:2.0)r;"
```

### Compiling from Source

Requirements:
* [CMake](https://cmake.org/)
* [Boost C++ Libraries](https://boost.org) (v1.60.0 or greater)
* C++ compiler and standard libraries (varies by OS)