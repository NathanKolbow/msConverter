# Code Progress TODO Checklist

### Todo:

1. Adjust the code so that nodes are n-ary instead of binary
2. Adjust code so that `Node.hiddenID` no longer exists (manage these hidden IDs in the code where the ms conversion is done)
3. Implement `Network::permuteBranchLengths` and `Network::permuteGamma` for testing

### Known bugs:

1. Networks with times that tie have the potential to spit out nonsense when using `SimSuite::newickToMS` (especially star phylogenies); tying times should be disallowed.