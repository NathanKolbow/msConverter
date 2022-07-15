#ifndef SIMSUITE_CORE_NODE_HPP_
#define SIMSUITE_CORE_NODE_HPP_

#include <string>
#include <iostream>

class Node {
    public:
        Node(void);
        Node* getLft(void) { return left; }
        Node* getRht(void) { return right; }

        Node* getMajorAnc(void) { return majorAncestor; }
        Node* getMinorAnc(void) { return minorAncestor; }

        int getIndex(void) { return index; }
        void setLft(Node* p) { left = p; }
        void setRht(Node* p) { right = p; }
        
        double getMajorBranchLength(void) { return majorBranchLength; }
        void setMajorBranchLength(double x) { majorBranchLength = x; }
        double getMinorBranchLength(void) { return minorBranchLength; }
        void setMinorBranchLength(double x) { minorBranchLength = x; }
        
        void setMajorAnc(Node* p) { majorAncestor = p; }
        void setMinorAnc(Node* p) { minorAncestor = p; }

        void setGammaLft(double g) { gammaLft = g; }
        void setGammaRht(double g) { gammaRht = g; }
        double getGammaLft(void) { return gammaLft; }
        double getGammaRht(void) { return gammaRht; }
        void setGamma(double g) { gamma = g; }
        double getGamma(void) { return gamma; }

        void setBootSupport(double supp) { bootSupport = supp; }
        double getBootSupport(void) { return bootSupport; }

        void setTime(double t) { time = t; }
        double getTime(void) { return time; }

        void setIndex(int x) { index = x; }
        void setName(std::string s) { name = s; }
        void setName(int i) { name = std::to_string(i); }
        std::string getName(void) { return name; }
        
        void printInfo(void);

        void setHiddenID(int id) { hiddenID = id; }
        int getHiddenID(void) { return hiddenID; }

        std::string getNewickFormattedName(bool, double);
    
    protected:
        Node* left;
        Node* right;
        Node* majorAncestor;         // NOTE: Depending on input, majorAnc and minorAnc do NOT neccessarily correspond to the anc. the contributes >50% and <50% genes respectively
        Node* minorAncestor;
        int index;
        std::string name;
        double majorBranchLength;    // NOTE: BRANCH LENGTHS CORRESPOND TO **INCOMING** BRANCHES, NOT OUTGOING
        double minorBranchLength;
        double time;
        double gammaLft;             // NOTE: OUTGOING GAMMA, i.e. this is the percent of genes this node gives to its left child
        double gammaRht;             // same as with gammaLft
        double bootSupport;
        double gamma;
        int hiddenID;                // Used in the Network::toms() function, so that node names are not disrupted 
};

#endif