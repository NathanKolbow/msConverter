#ifndef SIMSUITE_CORE_NETWORK_HPP_
#define SIMSUITE_CORE_NETWORK_HPP_

#include "MSEvents.hpp"

#include <vector>
#include <set>
#include <string>
#include <sstream>
class Node;

class Network {
    public:
        void listNodes(void);
        ~Network(void);
        std::string getNewickRepresentation(void) { return getNewick(false); }
        std::vector<std::string> getAllNewicks(void);
        std::vector<std::string> getRandomNewickRepresentations(int);
        int totalNodes(void) { return nodes.size(); }
        Network(std::string str, std::string strFormat);
        Network(std::vector<MSEvent*> events);
        std::string getMSString(void);

        std::vector<Node*> getNodes(void) { return nodes; }
        bool permuteRandomGamma(double gamma);
        void makeUltrametric(void);

    protected:
        Network(void) { }
        Node* root;
        std::vector<Node*> nodes;
        void writeNetwork(Node *p, std::stringstream& ss, bool, bool);
        void writeNetwork(Node *p, std::stringstream *ss, std::vector<std::stringstream*>& allStreams, bool minorHybrid);
        bool isHybridName(std::string);
        int hybridNameIndex(std::string val, std::vector<std::string> list);
        int activeNodesIdx(Node *p, std::vector<Node*>);
        int getLength(std::string);
        int getTotalExtantTaxa(void);
        std::string getNewick(bool);

    private:
        void buildFromNewick(std::string newickStr);
        void buildFromMS(std::vector<MSEvent*> events);
        std::vector<MSEvent*> parseMSEvents(std::string str);
        std::vector<std::string> parseNewick(std::string ns);
        void patchNetwork(void);
        void setTimes(void);
        void setTimeRecur(Node *p);
        double totalTime;
        void postmsPatchAndRename(void);
        std::string getInternalName(int);
        std::string getLeafName(int);
        int postmsPatchAndRenameRecur(Node*, int);
        std::vector<MSEvent*> toms(double);
        void warnBranchLength(bool, bool&);
        void warnHybridGamma(bool justReadHybrid, bool &warnedBlankOrZeroGamma, std::string nodeName);
};

bool isomorphic(Network, Network);
bool isomorphicNewick(std::string, std::string);
bool isomorphicRecur(Node*, Node*);
bool nodeEquivBranches(Node *p1, Node *p2);
void disableNetworkWarnings(void);

#endif