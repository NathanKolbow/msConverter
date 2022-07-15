# Code Progress TODO Checklist

### src/core/:

1. ~~Adjust code so that if taxon are already named 1, 2, 3, 4, ... then they are not overwritten in the process of converting from Newick --> ms~~
2. ~~Add a warning for when a hybrid node is included without a gamma specified, or with a gamma specified as 0 or 1.~~
3. ~~Fix Newick parser to be more ammenable to whitespace~~
4. ~~Add a warning to the Newick parser when branch lengths are not specified~~
5. ~~Add functionality to read Newick from input instead of taking a hard-coded Newick string from main~~
6. ~~Add functionality to read Newick from a file and then convert that to ms (`SimSuite::newickFileToMS(std::string location)`)~~
7. ~~When converting to ms, if the tree is not ultrametric automatically extend the leaf branches to be ultrametric, then give a warning to the user.~~

### src/tests/:

1. Finish setting up automated testing
2. Lots of testing!!!!