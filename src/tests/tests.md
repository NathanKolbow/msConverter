```julia
using PhyloNetworks, PhyloPlots
n = "((#H2:0.5::0.2,4:2.0):0.5,((1:1.0,(2:0.5)#H1:0.5::0.9):1.0,((#H1:0.5::0.1,3:1.0):0.5)#H2:0.5::0.8):0.5);"
net = readTopology(n)
plot(net,:R,showEdgeLength=true)
```

In `main.cpp`:
```
std::string msString = SimSuite::newickToMS("((#H2:0.5::0.2,4:2.0):0.5,((1:1.0,(2:0.5)#H1:0.5::0.9):1.0,((#H1:0.5::0.1,3:1.0):0.5)#H2:0.5::0.8):0.5);");
```

From `./main`, we get:
```
ms 4 1 -I 4 1 1 1 1 -es 0.500000 3 0.900000 -ej 1.000000 3 2 -ej 1.000000 5 4 -ej 2.500000 2 1 -es 1.500000 4 0.800000 -ej 2.000000 4 2 -ej 2.000000 6 1
```
It looks good, but I wonder if the times need to be in order:
```
ms 4 1 -I 4 1 1 1 1 -es 0.500000 3 0.900000 -ej 1.000000 3 2 -ej 1.000000 5 4 -es 1.500000 4 0.800000 -ej 2.000000 4 2 -ej 2.000000 6 1  -ej 2.500000 2 1
```