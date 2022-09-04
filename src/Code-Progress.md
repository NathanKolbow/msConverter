# Code Progress TODO Checklist

### TODO Prior to JOSS Submission:

1. Adjust the code so that nodes are n-ary instead of binary
2. Adjust code so that `Node.hiddenID` no longer exists (manage these hidden IDs in the code where the ms conversion is done)
3. Implement `Network::permuteBranchLengths` and `Network::permuteGamma` for testing
4. Allow Newick to specify the gamma for a only a single incoming branch on a hybrid node, and for the other to be inferred