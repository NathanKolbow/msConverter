// Copyright (c) 2022 Nathan Kolbow
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to use, 
// copy, modify, merge, publish, distribute, sublicense, and*or sell copies of the 
// Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "Node.hpp"
#include "Network.hpp"
#include "MSEvents.hpp"

#include <iostream>
#include <iomanip>
#include <set>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <typeinfo>


bool WARNINGS_ENABLED = true;

void disableNetworkWarnings(void) { WARNINGS_ENABLED = false; }

void permuteRandomBranchLength(double change) {

}

// returns true if a permutation was successfully performed, false if
// there were no gammas available to permute
bool Network::permuteRandomGamma(double change) {
    // find all the nodes that actually have gammas
    std::vector<Node*> validNodes;
    for(Node *p : nodes) {
        if(p->getGammaLft() != 0 || p->getGammaRht() != 0)
            validNodes.push_back(p);
    }

    // if none, return false
    if(validNodes.size() == 0)
        return false;

    // select a random node

    // permute its gamma (favors the left by default)

    return false;
}

bool isomorphic(Network *net1, Network *net2) {
    // Find the root of each network
    Node *root1 = net1->getNodes()[0];
    while(root1->getMajorAnc() != NULL)
        root1 = root1->getMajorAnc();
    
    Node *root2 = net2->getNodes()[0];
    while(root2->getMajorAnc() != NULL)
        root2 = root2->getMajorAnc();

    // Check for isomorphism recursively
    return isomorphicRecur(root1, root2);
}

bool isomorphicRecur(Node *p1, Node *p2) {
    // Both are NULL
    if(p1 == p2)
        return true;
    // One is null but the other is not
    if(p1 != p2 && (p1 == NULL || p2 == NULL)) {
        // std::cout << "Not isomorphic; " << (p1==NULL?p2->getName():p1->getName()) << " is not NULL, but its partner is.\n";
        return false;
    }

    // Check for equivalent hybrid statuses
    // if one is a hybrid but not the either, return false.
    if(p1->getMinorAnc() != p2->getMinorAnc() && (p1->getMinorAnc() == NULL || p2->getMinorAnc() == NULL)) {
        // std::cout << "Hybrid status of " << p1->getName() << " and " << p2->getName() << " differs.\n";
        return false;
    }

    // if branch info differs, return false.
    if(!nodeEquivBranches(p1, p2)) {
        // std::cout << "Branches of " << p1->getName() << " and " << p2->getName() << " differ.\n";
        return false;
    }

    // Get the left and right children of each node
    Node *left1 = p1->getLft();
    Node *right1 = p1->getRht();
    Node *left2 = p2->getLft();
    Node *right2 = p2->getRht();

    // Check for equivalence of presence of children
    // if they have differing amount of present children, return false.
    if((left1 == NULL) + (right1 == NULL) != (left2 == NULL) + (right2 == NULL)) {
        // std::cout << "Sum presence of children of " << p1->getName() << " and " << p2->getName() << " differs.\n";
        return false;
    }

    // If we've gotten this far, then we have to recur into the children
    // we don't know which children could be equal, so we try all combinations
    return (isomorphicRecur(left1, left2) && isomorphicRecur(right1, right2)) ||
           (isomorphicRecur(left1, right2) && isomorphicRecur(left2, right1));
}

bool nodeEquivBranches(Node *p1, Node *p2) {
    if(p1->getMinorAnc() == p2->getMinorAnc()) {
        // Neither are hybrids
        return 
            // compare incoming branch lengths
            p1->getMajorBranchLength() == p2->getMajorBranchLength() &&
            // compare outgoing gammas
            ((p1->getGammaLft() == p2->getGammaLft() && p1->getGammaRht() == p2->getGammaRht()) || (p1->getGammaLft() == p2->getGammaRht() && p1->getGammaRht() == p2->getGammaLft()));
    } else {
        // Both are hybrids
        // if NOT(each of the branches of p1 matches one of p2's), return false.
        if(!((p1->getMajorBranchLength() == p2->getMajorBranchLength() && p1->getMinorBranchLength() == p2->getMinorBranchLength()) ||
           (p1->getMajorBranchLength() == p2->getMinorBranchLength() && p1->getMinorBranchLength() == p2->getMajorBranchLength())))
           return false;

        // if neither pairing of outgoing gamma values matches, return false.
        if(!(p1->getGammaLft() == p2->getGammaLft() && p1->getGammaRht() == p2->getGammaRht()) && !(p1->getGammaLft() == p2->getGammaRht() && p1->getGammaRht() == p2->getGammaLft()))
            return false;
    }
    return true;
}

bool isomorphicNewick(std::string newick1, std::string newick2) {
    return isomorphic(new Network(newick1, "newick"), new Network(newick2, "newick"));
}

void Network::buildFromMS(std::vector<MSEvent*> events) {
    // First, order the events backwards in time (from tips to root) so that they
    // can be read in order.
    sort(events.begin(), events.end(), [](MSEvent *a, MSEvent *b) {
        return a->getTime() < b->getTime();
    });

    // Start by figuring out how many leaves we have. This is equal to the largest number
    // in the MSEvent's minus the total number of MSSplitEvent's
    int max = 0;
    int splits = 0;
    for(MSEvent *e : events) {
        // Add a split. Splits necessarily create a new, larger number, so we don't need to
        // check these events for a maximum number
        if(e->getEventType() == split)
            splits++;
        else {
            max = std::max(std::max(max, ((MSJoinEvent*)e)->getMajorTaxa()), ((MSJoinEvent*)e)->getMinorTaxa());
        }
    }
    int ntaxa = max - splits;

    // activeNodes includes the most recent version of any given taxa. I.e. after a split event where taxa
    // 3 is turned into taxa 3 & taxa 7, the old node for taxa 3 will be removed, and both the new nodes added.
    // Also, if taxa 3 & taxa 4 are joined with command -ej <t> 3 4 (i.e. deleting taxa 3), the destroyed node
    // for taxa 3 will remain in activeNodes, but the old version of taxa 4 will be removed and the new one added.
    //
    // We have to do things in this way because otherwise we run into issues when a node is both itself a hybrid
    // and apart of a hybridization event.
    std::vector<Node*> activeNodes;

    // Populate the nodes list with the leaves
    for(int i=1; i<=ntaxa; i++) {
        Node *p = new Node();
        p->setName(i);
        p->setTime(0);
        nodes.push_back(p);
        activeNodes.push_back(p);
    }
    
    // Now, just process each event, and build the network backwards in time
    for(unsigned int i=0; i < events.size(); i++) {
        // Because we are doing things in chronological order, the time that a join event occurs should be
        // greater than or equal to the times of *each* of the nodes involved in the event. So, all we have
        // to do is create a parent pointing to each of the nodes involved.
        if(events[i]->getEventType() == join) {
            MSJoinEvent *e = (MSJoinEvent*)events[i];

            // Both taxa involved in the join event should already exist
            Node *fromTaxa = NULL;
            Node *toTaxa = NULL;
            int fromIdx;  // used for removing the node from activeNodes
            int toIdx;

            for(unsigned int i=0; i<activeNodes.size(); i++) {
                if(fromTaxa == NULL && activeNodes[i]->getName().compare(std::to_string(e->getMajorTaxa())) == 0) {
                    fromTaxa = activeNodes[i];
                    fromIdx = i;
                } else if(toTaxa == NULL && activeNodes[i]->getName().compare(std::to_string(e->getMinorTaxa())) == 0) {
                    toTaxa = activeNodes[i];
                    toIdx = i;
                }

                if(fromTaxa != NULL && toTaxa != NULL)
                    break;
            }

            // If we couldn't find one or both of the taxa involved, this is an error; quit
            if(fromTaxa == NULL || toTaxa == NULL) {
                std::cerr << "ERROR: When finding both taxa in a join event, one or both taxa could not be found; quitting." << std::endl;
                exit(-1);
            }

            // Create a new parent node pointing back to both of these nodes
            Node *p = new Node();
            p->setTime(e->getTime());
            p->setName(fromTaxa->getName());
            
            // Make the children point to their new parent and
            // Set the branch lengths. Remember, branch lengths are INCOMING, not outgoing.
            fromTaxa->setMajorAnc(p);
            fromTaxa->setMajorBranchLength(p->getTime() - fromTaxa->getTime());
            toTaxa->setMajorAnc(p);
            toTaxa->setMajorBranchLength(p->getTime() - toTaxa->getTime());

            // Now fromTaxa and toTaxa are the nodes that we actually want to connect to, so we create the connections
            // There shouldn't be an issue always setting fromTaxa as left and toTaxa as right...I believe.
            p->setLft(fromTaxa);
            p->setRht(toTaxa);

            // Remove fromTaxa and toTaxa from activeNodes
            if(fromIdx > toIdx) {
                activeNodes.erase(std::next(activeNodes.begin(), fromIdx));
                activeNodes.erase(std::next(activeNodes.begin(), toIdx));
            } else {
                activeNodes.erase(std::next(activeNodes.begin(), toIdx));
                activeNodes.erase(std::next(activeNodes.begin(), fromIdx));
            }

            // Add p to activeNodes AND nodes
            activeNodes.push_back(p);
            nodes.push_back(p);
        } else if(events[i]->getEventType() == split) {
            MSSplitEvent *e = (MSSplitEvent*)events[i];
            // Split events are straightforward, but require the use of an intermediate node that will be removed later.
            // This is because ms's protocol creates a new node everytime a split occurs, we essentially end up with a
            // duplicate node in our network. There are many ways that this could potentially be dealt with, but this is
            // how we choose to deal with it.
            Node *p = NULL;
            int pIdx;

            for(unsigned int i=0; i<activeNodes.size(); i++) {
                if(activeNodes[i]->getName().compare(std::to_string(e->getTaxa())) == 0) {
                    p = activeNodes[i];
                    pIdx = i;
                    break;
                }
            }
            
            // If p is still NULL, this is an error; quit
            if(p == NULL) {
                std::cerr << "ERROR: Node involved in split event not found when split event was reached; quitting." << std::endl;
                exit(-1);
            }

            // If p is a leaf then we actually need to create a new node
            // getLft will equal getRht exactly when both are NULL.
            if(p->getLft() == p->getRht() && p->getTime() == 0) {
                Node *newNode = new Node();
                newNode->setName(p->getName());
                newNode->setTime(e->getTime());
                newNode->setLft(p);
                p->setMajorAnc(newNode);
                p->setMajorBranchLength(newNode->getTime());  // p->getTime() is 0
                nodes.push_back(newNode);

                // Remove the leaf from activeNodes
                activeNodes.erase(std::next(activeNodes.begin(), pIdx));

                // Now newNode is the node we want to operate on
                p = newNode;
                activeNodes.push_back(newNode);
                pIdx = activeNodes.size() - 1;
            } else {
                // This event corresponds to Node p splitting, so this event gives us p's time
                p->setTime(e->getTime());
            }

            // Create both of the new nodes
            Node *maj = new Node();
            maj->setName(p->getName());
            maj->setLft(p);
            maj->setGamma(e->getGamma());

            Node *min = new Node();
            min->setName((ntaxa++)+1);
            min->setLft(p);
            min->setGamma(1-e->getGamma());

            // Link p to its ancestors
            p->setMajorAnc(maj);
            p->setMinorAnc(min);

            // We do not know the times on either ancestor, so we cannot set p's branch length yet!
            // Remove p from activeNodes
            activeNodes.erase(std::next(activeNodes.begin(), pIdx));
            
            // Add min and maj to activeNodes
            activeNodes.push_back(min);
            activeNodes.push_back(maj);

            // Add min and maj to nodes
            nodes.push_back(min);
            nodes.push_back(maj);
        } else {
            std::cerr << "ERROR: MSEvent is neither a split or join event; quitting." << std::endl;
            exit(-1);
        }
    }

    // All of the nodes in the network have indistinguishable names right now, so let's fix that
    postmsPatchAndRename();
}

void Network::postmsPatchAndRename(void) {
    // This function serves two purposes.
    //    1. in order to build our network from ms commands, we give our nodes names which are not distinct;
    //       this function gives them distinct names (rename)
    //    2. in the process of building from ms we are left with redundant nodes; this function removes those nodes (patch)

    // Internal nodes are given lower case letter names: a, b, c, ..., z, za, zb, ..., zz, ..., zzz, ...
    // Leaves are given natural number names: 1, 2, 3, ...

    // Loop through every node in the network, correcting and ultimately removing the redundant nodes, and renaming all other nodes
    std::vector<int> removeMe;
    int leafCount = 0;
    int internalNodeCount = 0;
    int hybridCount = 1;
    for(unsigned int i=0; i < nodes.size(); i++) {
        // Check if the node is redundant
        if(nodes[i]->getTime() == -1) {
            // REDUNDANT
            // Redundant nodes contain three pieces of information:
            //   1. inheritance probability
            //   2. a child
            //   3. an ancestor
            // All that we need to do it: connect child <--> ancestor, and give the gamma value
            // to the ancestor on the proper branch

            // These nodes only ever have one anc and one child (major and left, respectively)
            Node *anc = nodes[i]->getMajorAnc();
            Node *child = nodes[i]->getLft();

            // If what we got is not what we expected, throw an error. This should never be thrown, this is just a sanity check really
            if(anc == NULL || child == NULL || nodes[i]->getRht() != NULL || nodes[i]->getMinorAnc() != NULL) {
                std::cerr << "ERROR: When patching network in the MSEvent reading process, one or more of the children or ancestors of a redundant node were not what we expected; quitting." << std::endl;
                exit(-1);
            }

            // anc --> child
            if(anc->getLft() != NULL) {
                anc->setRht(child);
                anc->setGammaRht(child->getGamma());
            } else {
                anc->setLft(child);
                anc->setGammaLft(child->getGamma());
            }

            // child --> anc
            child->setMajorAnc(anc);
            child->setMajorBranchLength(anc->getTime() - child->getTime());

            // Queue the redundant node for removal
            removeMe.push_back(i);
        } else {
            // Give this node a new name! Different naming rules for internal and external nodes,
            // so we have to figure out which one this is first
            if(nodes[i]->getLft() == nodes[i]->getRht()) {
                // This is a leaf; these are never hybrids
                nodes[i]->setName(getLeafName(leafCount++));
            } else {
                // This is an internal node; we have to check whether it is a hybrid
                if(nodes[i]->getMinorAnc() != NULL)
                    // hybrid node
                    nodes[i]->setName(getInternalName(internalNodeCount++));
                else
                    // not a hybrid node
                    nodes[i]->setName(getInternalName(internalNodeCount++) + "#" + std::to_string(hybridCount++));
            }
        }
    }

    // Sort the indices to be removed in reverse order, so that we can remove them without disrupting the order along the way
    std::sort(removeMe.begin(), removeMe.end(),
    [](int a, int b) {
        return(a > b);
    }
    );
    for(int idx : removeMe) {
        Node *n = nodes[idx];
        nodes.erase(std::next(nodes.begin(), idx));

        // Don't leak memory
        delete n;
    }

}

// Gets an internal node name from an integer; used in the postmsPatchAndRename function
// 0 --> a, 1 --> b, ..., 25 --> z, 26 --> za, ..., 51 --> zz, ..., 77 --> zzz, ...
std::string Network::getInternalName(int i) {
    char ch = 97;
    std::string name = "";
    while(i >= 26) {
        name += "z";
        i -= 26;
    }
    ch += i;
    name += ch;
    return name;
}

// Gets an internal node name from an integer; used in the postmsPatchAndRename function
// 0 --> a, 1 --> b, ..., 25 --> z, 26 --> za, ..., 51 --> zz, ..., 77 --> zzz, ...
std::string Network::getLeafName(int i) {
    char ch = 65;
    std::string name = "";
    while(i >= 26) {
        name += "Z";
        i -= 26;
    }
    ch += i;
    name += ch;
    return name;
}

int Network::getTotalExtantTaxa(void) {
    int count = 0;
    for(Node *p : nodes) {
        if(p->getLft() == p->getRht())
            count++;
    }
    return count;
}

// IMPORTANT NOTE: for ms commands, RIGHT NOW (to be changed), the only accepted input form is
//  a series of -ej <t> <i> <j> and -es <t> <i> <gamma> options
Network::Network(std::string str, std::string strFormat) {
    if(strFormat.compare("newick") == 0)
        buildFromNewick(str);
    else if(strFormat.compare("ms") == 0) {
        std::vector<MSEvent*> events = parseMSEvents(str);
        buildFromMS(events);
    }
}

// Expected string right now is anything of the form: "-ej <t> <i> <j> -es <t> <i> <gamma> ..."
std::vector<MSEvent*> Network::parseMSEvents(std::string str) {
    std::vector<MSEvent*> events;
    std::vector<std::string> tks;

    // First, we are going to parse the input string into commands
    int spaceCount = 0;
    int last_idx = -1;
    for(unsigned int i=0; i <= str.length(); i++) {
        if(str[i] == ' ' || str[i] == '\0') {
            spaceCount++;
            if(spaceCount == 4) {
                // This is the end of a single command; push the token
                tks.push_back(str.substr(last_idx+1, i-last_idx-1));
                spaceCount = 0;
                last_idx = i;
            }
        }
    }

    // When we get out of the for loop, spaceCount should be exactly 0. Throw an error if this is not the case
    if(spaceCount != 0) {
        std::cerr << "ERROR: Input ms command seqeuence was not in expected format; quitting." << std::endl;
        exit(-1);
    }

    // Parse the events
    for(unsigned int i=0; i < tks.size(); i++) {
        std::string tk = tks[i];

        if(tk[2] == 'j') {
            // Join event
            // parse the time
            int startIdx = 4;
            int length = 1;
            while(tk[startIdx + length] != ' ')
                length++;
            double time = stod(tk.substr(startIdx, length));

            // parse the minor taxa
            startIdx += length+1;
            length = 1;
            while(tk[startIdx + length] != ' ')
                length++;
            int minorTaxa = stoi(tk.substr(startIdx, length));

            // parse the major taxa
            startIdx += length+1;
            length = 1;
            while(tk[startIdx + length] != ' ' && tk[startIdx + length] != '\0')
                length++;
            int majorTaxa = stoi(tk.substr(startIdx, length));

            // Create and push the event
            MSJoinEvent *e = new MSJoinEvent(time, minorTaxa, majorTaxa);
            events.push_back(e);
        } else if(tk[2] == 's') {
            // Split event
            // parse the time
            int startIdx = 4;
            int length = 1;
            while(tk[startIdx + length] != ' ')
                length++;
            double time = stod(tk.substr(startIdx, length));

            // parse the taxa
            startIdx += length + 1;
            length = 1;
            while(tk[startIdx + length] != ' ')
                length++;
            int taxa = stoi(tk.substr(startIdx, length));

            // parse gamma
            startIdx += length + 1;
            length = 1;
            while(tk[startIdx + length] != ' ' && tk[startIdx + length] != '\0')
                length++;
            double gamma = stod(tk.substr(startIdx, length));

            // Create and push the event
            MSSplitEvent *e = new MSSplitEvent(time, taxa, gamma);
            events.push_back(e);
        } else {
            // Improper input
            std::cerr << "ERROR: Input ms command invalid; quitting." << std::endl;
            exit(-1);
        }
    }

    return events;
}

std::string Network::getMSString(void) {
    // Check to make sure the tree is ultrametric
    double endTime = -1;
    bool ultrametric = true;

    // used if the tree is not ultrametric
    std::vector<Node*> leaves;
    std::vector<int> times;
    for(Node *p : nodes) {
        if(p->getLft() == p->getRht()) {
            leaves.push_back(p);
            if(endTime != -1) {
                // Floating point issues...
                endTime = std::max(endTime, p->getTime());
                if(std::abs(p->getTime() - endTime) > 1e-9)
                    ultrametric = false;
            } else {
                endTime = p->getTime();
            }
        }
    }

    if(!ultrametric) {
        if(WARNINGS_ENABLED) {
            std::cerr << "WARNING: ms requires ultramteric input, and the input was not ultrametric, so branch lengths are automatically being extended to make the input ultrametric." << std::endl;
        }

        // we are going to edit the leaf times to make the tree ultrametric, and we have
        // to store their original lengths in order to revert back
        for(Node *p : leaves) {
            times.push_back(p->getTime());
            p->setTime(endTime);
        }
    }

    std::vector<MSEvent*> events = toms(endTime);
    std::stringstream ss;
    ss << "ms " << leaves.size() << " " << 1 << " -T -I " << leaves.size();
    for(unsigned int i=0; i < leaves.size(); i++)
        ss << " 1";

    std::string str = ss.str() + " ";

    for(MSEvent* e : events) {
        str += ((e->getEventType() == join) ? ((MSJoinEvent*)e)->toString() : ((MSSplitEvent*)e)->toString()) + " ";
    }

    if(!ultrametric) {
        // revert the branch lengths back
        for(unsigned int i = 0; i < leaves.size(); i++)
            leaves[i]->setTime(times[i]);
    }

    return str.substr(0, str.length()-1);
}

void Network::buildFromNewick(std::string newickStr) {
    // parse the TRIMMED Newick string
    newickStr.erase(std::remove_if(newickStr.begin(), newickStr.end(), ::isspace), newickStr.end());
    std::vector<std::string> tokens = parseNewick(newickStr);

    // Some bools to make sure that we only warn for certain conditions once
    bool warnedNoBranchLengths = false;
    bool warnedBlankOrZeroGamma = false;
    // build up the Network from the parsed Newick string
    bool readingBranchLength = false;
    bool readingBootSupport = false;
    bool readingGamma = false;
    bool namingInternalNode = false;
    
    bool waitingForGamma = false;
    std::string waitingForGammaNodeName;

    Node* p = NULL;
    for(unsigned int i=0; i<tokens.size(); i++) {
        std::string token = tokens[i];
        if(token == "(") {
            warnBranchLength(readingBranchLength, warnedNoBranchLengths);
            warnHybridGamma(waitingForGamma, warnedBlankOrZeroGamma, waitingForGammaNodeName);
            readingBranchLength = false;
            readingBootSupport = false;
            readingGamma = false;
            namingInternalNode = false;
            // new node
            Node* newNode = new Node;
            nodes.push_back(newNode);
            if(p == NULL) {
                root = newNode;
            } else {
                newNode->setMajorAnc(p);
                if(p->getLft() == NULL)
                    p->setLft(newNode);
                else
                    p->setRht(newNode);
            }
            
            p = newNode;
        } else if(token == ")" || token == ",") {
            warnBranchLength(readingBranchLength, warnedNoBranchLengths);
            warnHybridGamma(waitingForGamma, warnedBlankOrZeroGamma, waitingForGammaNodeName);
            readingBranchLength = false;
            readingBootSupport = false;
            readingGamma = false;
            namingInternalNode = false;
            if(token == ")")
                namingInternalNode = true;

            // move down one node
            if(p->getMajorAnc() == NULL) {
                std::cerr << "ERROR: We cannot find an expected ancestor at i=" << i << "; p == root gives: " << (p == root) << std::endl;
                exit(1);
            }
            p = p->getMajorAnc();
        } else if(token == ":") {
            namingInternalNode = false;

            // if the field for branch length, boot support, or inheritance probability are left blank, then we end up here.
            // so, we want to progress to the next step of reading :br_len:boot_supp:inher_prob
            if(readingBranchLength) {
                warnBranchLength(readingBranchLength, warnedNoBranchLengths);
                readingBranchLength = false;
                readingBootSupport = true;
            } else if(readingBootSupport) {
                readingBootSupport = false;
                readingGamma = true;
            } else if(readingGamma) {
                std::cerr << "ERROR: Read a sequence of four colons (possibly with names/numbers in between some of them). This is not allowed by the format; quitting." << std::endl;
                exit(-1);
            } else {
                // begin reading a branch length
                readingBranchLength = true;
            }
        } else if(token == ";") {
            warnBranchLength(readingBranchLength, warnedNoBranchLengths);
            warnHybridGamma(waitingForGamma, warnedBlankOrZeroGamma, waitingForGammaNodeName);
            namingInternalNode = false;
            // finished!
            if(p != root) {
                std::cerr << "ERROR: We expect to finish at the root node, finished at " << p->getName() << ", " << p << std::endl;
                exit(1);
            }
        } else {
            // we have a taxon name or a branch length
            if(readingBranchLength == true) {
                double x = std::stod(token);
                p->setMajorBranchLength(x);
                readingBranchLength = false;

                // Is there another ':' coming up? We expect them in batches of 3
                if(tokens[i+1] == ":") {
                    readingBootSupport = true;
                    i++;
                }
            } else if(readingBootSupport) {
                double x = std::stod(token);
                p->setBootSupport(x);
                readingBootSupport = false;

                // Is there another ':' coming up? We expect them in batches of 3
                if(tokens[i+1] == ":") {
                    readingGamma = true;
                    i++;
                }
            } else if(readingGamma) {
                double x = std::stod(token);
                p->setGamma(x);
                readingGamma = false;
                waitingForGamma = false;
            } else if(namingInternalNode) {
                p->setName(token);
            } else {
                Node* newNode = new Node;
                nodes.push_back(newNode);
                newNode->setMajorAnc(p);
                
                if(p->getLft() == NULL)
                    p->setLft(newNode);
                else
                    p->setRht(newNode);
                
                char ch;
                int j = 0;
                do {
                    ch = token[j];
                    if(ch == '#') {
                        break;
                    }
                    j += 1;
                } while(ch != '\0');

                if(ch == '#') {
                    // Hybrid node
                    waitingForGamma = true;
                    waitingForGammaNodeName = token;
                }

                newNode->setName(token);
                p = newNode;
            }
            namingInternalNode = false;
        }
    }

    // index the nodes
    int ndeIdx = 0;
    for(unsigned int i=0; i<nodes.size(); i++) {
        if(nodes[i]->getLft() == NULL)
            nodes[i]->setIndex(ndeIdx++);
    }
    
    for(unsigned int i=0; i<nodes.size(); i++) {
        if(nodes[i]->getLft() != NULL)
            nodes[i]->setIndex(ndeIdx++);
    }

    // Stitch the hybrid nodes together
    patchNetwork();

    // Add time information to the nodes
    setTimes();
}

void Network::warnBranchLength(bool readingBranchLength, bool &alreadyWarned) {
    if(readingBranchLength && !alreadyWarned) {
        if(WARNINGS_ENABLED) {
            std::cerr << "WARNING: At least one branch length was left unspecified and is defaulting to length 0." << std::endl;
        }

        alreadyWarned = true;
    }
}

void Network::warnHybridGamma(bool justReadHybrid, bool &warnedBlankOrZeroGamma, std::string nodeName) {
    if(justReadHybrid && !warnedBlankOrZeroGamma) {
        if(WARNINGS_ENABLED) {
            std::cerr << "WARNING: Gamma not specified for hybrid node " << nodeName << ". Topology will be preserved but ALL gamma values will default to 0." << std::endl;
        }

        warnedBlankOrZeroGamma = true;
    }
}

void Network::setTimes(void) {
    // Set root time to 0 and go recursively from there
    root->setTime(0);
    if(root->getLft() != NULL)
        setTimeRecur(root->getLft());
    if(root->getRht() != NULL)
        setTimeRecur(root->getRht());
}

void Network::setTimeRecur(Node *p) {
    Node *majAnc = p->getMajorAnc();
    Node *minAnc = p->getMinorAnc();

    // if we have two ancs and one of their times have not been set yet, we have to wait
    if(majAnc->getTime() == -1 || (minAnc != NULL && minAnc->getTime() == -1))
        return;

    // we will never have root in this function, so majAnc always exists
    double timeFollowingMaj = majAnc->getTime() + p->getMajorBranchLength();
    double timeFollowingMin = (minAnc == NULL) ? timeFollowingMaj : minAnc->getTime() + p->getMinorBranchLength();

    if(std::abs(timeFollowingMaj - timeFollowingMin) > 1e-9) {
        std::cerr << "ERROR: Branch lengths leading to node " << p->getName() << " disagree! Lengths are " << timeFollowingMaj << " and " << timeFollowingMin << "; quitting." << std::endl;
        exit(-1);
    }
    p->setTime(timeFollowingMaj);

    if(p->getLft() != NULL)
        setTimeRecur(p->getLft());
    if(p->getRht() != NULL)
        setTimeRecur(p->getRht());
}

std::vector<std::string> Network::parseNewick(std::string ns) {
    std::vector<std::string> tks;
    for(unsigned int i = 0; i < ns.size(); i++) {
        char c = ns[i];
        if(c == '(' || c == ')' || c == ',' || c == ':' || c == ';') {
            std::string tempStr;
            tempStr = c;
            tks.push_back(tempStr);
        } else {
            int j = i;
            std::string tempStr = "";
            while(!(c == '(' || c == ')' || c == ',' || c == ':' || c == ';')) {
                tempStr += c;
                j++;
                c = ns[j];
            }
            i = j - 1;
            tks.push_back(tempStr);
        }
    }

    return tks;
}

// Patches up the hybrid edges on the network 
void Network::patchNetwork() {
    std::vector<std::string> hybridNames;
    std::vector<int> nodeIndices;
    std::vector<int> removeMe;

    int count = 0;
    for(unsigned int i = 0; i < nodes.size(); i++) {
        Node *p = nodes[i];
        // Check if this node is a hybrid
        if(isHybridName(p->getName())) {
            // It's a hybrid! Is it already in the list?
            int idx = hybridNameIndex(p->getName(), hybridNames);
            if(idx == -1) {
                hybridNames.push_back(p->getName());
                nodeIndices.push_back(i);
                count++;
            } else {
                // This is the other node (we aren't actually sure if we're killing it yet)
                Node *temp = nodes[nodeIndices[idx]];

                // One of these nodes necessarily has 0 children, and the other necessarily has 1 child.
                // We will keep the one with 1 child.
                Node *dead = temp->getLft() == temp->getRht() ? temp : p;
                p = (dead == temp) ? p : temp;

                // We need to make sure that we set MajorAnc and MinorAnc properly!!!
                // MajorAnc will be the ancestor from which the node receives >50% of its genes, and MinorAnc
                // its complement. (In the case of a tie, it doesn't matter, so we don't do anything special...)
                // if(dead->getGamma() > p->getGamma()) {
                // Higher inheritance probability coming from the ancestor of dead
                    // p->setMinorAnc(p->getMajorAnc());
                    // p->setMinorBranchLength(p->getMajorBranchLength());
                    // p->setMajorAnc(dead->getMajorAnc());
                    // p->setMajorBranchLength(dead->getMajorBranchLength());

                    // // Set the gammas for each of p's ancestors
                    // if(p->getMajorAnc()->getLft() == p)
                    //     p->getMajorAnc()->setGammaLft(dead->getGamma());
                    // else
                    //     p->getMajorAnc()->setGammaRht(dead->getGamma());
                    
                    // // Now the minor ancestor
                    // if(p->getMinorAnc()->getLft() == p)
                    //     p->getMinorAnc()->setGammaLft(p->getGamma());
                    // else
                    //     p->getMinorAnc()->setGammaRht(p->getGamma());
                // } else {

                // By convention, p's majorAncestor will always stay
                // Higher inheritance probability coming from the ancestor of p
                // p's MajorAnc is already correct, so just set the MinorAnc
                p->setMinorAnc(dead->getMajorAnc());
                p->setMinorBranchLength(dead->getMajorBranchLength());

                // Set the gammas for each of p's ancestors
                if(p->getMajorAnc()->getLft() == p)
                    p->getMajorAnc()->setGammaLft(p->getGamma());
                else
                    p->getMajorAnc()->setGammaRht(p->getGamma());
                
                // Now the minor ancestor
                if(p->getMinorAnc()->getLft() == p)
                    p->getMinorAnc()->setGammaLft(dead->getGamma());
                else
                    p->getMinorAnc()->setGammaRht(dead->getGamma());

                // }

                // Set p as the proper child of its new ancestor
                if(dead->getMajorAnc()->getLft() == dead)
                    dead->getMajorAnc()->setLft(p);
                else
                    dead->getMajorAnc()->setRht(p);

                // Stage the dead node for removal
                if(dead == temp)
                    removeMe.push_back(nodeIndices[idx]);
                else
                    removeMe.push_back(i);
            }
        }
    }

    // Remove the dead nodes from our node list
    // Reverse sort the list so that we don't mess up indices as we go.
    std::sort(removeMe.begin(), removeMe.end(),
        [](int a, int b) {
            return(a > b);
        }
    );
    for(int idx : removeMe) {
        Node *n = nodes[idx];
        nodes.erase(std::next(nodes.begin(), idx));

        // Don't leak memory
        delete n;
    }
}

int Network::hybridNameIndex(std::string val, std::vector<std::string> list) {
    for(unsigned int i = 0; i < list.size(); i++) {
        if(val.compare(list[i]) == 0)
            return i;
    }
    return -1;
}

int Network::getLength(std::string val) { return val.length(); }

bool Network::isHybridName(std::string val) {
    int len = getLength(val);
    for(int i = 0; i < len; i++) {
        if(val[i] == '#')
            return true;
    }
    return false;
}

std::string Network::getNewick(bool randomWrite) {
    std::stringstream ss;
    
    if(root->getLft() != NULL && root->getRht() != NULL)
        writeNetwork(root, ss, false, randomWrite);
    else
        writeNetwork(root->getLft(), ss, false, randomWrite);
    ss << ";";
    
    std::string newick = ss.str();
    return newick;
}

std::vector<std::string> Network::getRandomNewickRepresentations(int n) {
    std::vector<std::string> l;
    for(int i=0; i < n; i++)
        l.push_back(getNewick(true));
    return l;
}

void Network::writeNetwork(Node* p, std::stringstream& ss, bool minorHybrid, bool randomWrite) {
    // randomWrite is a variable that was added to get different Newick strings for the same tree.
    // This will make automating testing our isomorphicNewick function MUCH easier (in one direction at least)
    //
    // Different rules apply when dealing with hybrids, but we still want to traverse them normally once.
    // The variable `minorHybrid` allows us to traverse a hybrid node twice, differently both times.
    if(p != NULL) {
        if(p->getLft() == NULL || minorHybrid) {
            ss << p->getNewickFormattedName(minorHybrid, (minorHybrid ? (p == p->getMinorAnc()->getLft() ? p->getMinorAnc()->getGammaLft() : p->getMinorAnc()->getGammaRht()) : -1));
        } else {
            ss << "(";

            if(randomWrite && p->getRht() != NULL) {
                // With a 50% chance, go right instead of left
                float randVal = (float) rand() / RAND_MAX;
                if(randVal > 0.50) {
                    // Write the network as per usual.
                    writeNetwork(p->getRht(), ss, p->getGammaRht() != 0 && p->getRht()->getMinorAnc() == p, randomWrite);
                    ss << ",";
                    writeNetwork(p->getLft(), ss, p->getGammaLft() != 0 && p->getLft()->getMinorAnc() == p, randomWrite);

                    if(p->getMajorAnc() == NULL)
                        // Root only have a name; not branch lengths or boot support or gamma
                        ss << ")" << p->getName();
                    else
                        ss << ")" << p->getNewickFormattedName(minorHybrid, p->getMinorAnc() == NULL ? -1 : (p->getMajorAnc()->getLft() == p ? p->getMajorAnc()->getGammaLft() : p->getMajorAnc()->getGammaRht()));

                    return;
                }
            }
            
            // Write the network as per usual.
            writeNetwork(p->getLft(), ss, p->getGammaLft() != 0 && p->getLft()->getMinorAnc() == p, randomWrite);
            if(p->getRht() != NULL) {
                ss << ",";
                writeNetwork(p->getRht(), ss, p->getGammaRht() != 0 && p->getRht()->getMinorAnc() == p, randomWrite);
            }
            if(p->getMajorAnc() == NULL)
                // Root only have a name; not branch lengths or boot support or gamma
                ss << ")" << p->getName();
            else
                ss << ")" << p->getNewickFormattedName(minorHybrid, p->getMinorAnc() == NULL ? -1 : (p->getMajorAnc()->getLft() == p ? p->getMajorAnc()->getGammaLft() : p->getMajorAnc()->getGammaRht()));
        }
    }
}

Network::~Network(void) {
    for(unsigned int i = 0; i < nodes.size(); i++)
        delete nodes[i];
}

void Network::listNodes(void) {
    for(unsigned int i = 0; i < nodes.size(); i++) {
        std::cout << "Node " << i << ": " << nodes[i]->getName() << " (" << nodes[i] << ") " << std::endl << std::flush;
        nodes[i]->printInfo();
    }
}

std::vector<MSEvent*> Network::toms(double endTime) {
    std::vector<MSEvent*> events;
    // First, gather & name all leaves while setting the names of non-leaves to null
    std::vector<std::string> nodeNames;
    std::vector<Node*> activeNodes;
    for(unsigned int i = 0; i < nodes.size(); i++) {
        if(nodes[i] != NULL) {
            if(nodes[i]->getLft() == NULL && nodes[i]->getRht() == NULL) {
                activeNodes.push_back(nodes[i]);
                nodeNames.push_back(nodes[i]->getName());
            } else {
                nodes[i]->setHiddenID(-1);
            }
        }
    }

    // Second, check if all nodes are already named 1, 2, 3, 4, ..., if so, we do NOT want to overwrite names
    bool overwriteNames = false;

    try {
        // We need to sort them as integers, not as strings
        std::sort(nodeNames.begin(), nodeNames.end(), [](std::string a, std::string b) {
            return std::stoi(b) > std::stoi(a);
        });

        for(unsigned int i = 0; i < nodeNames.size(); i++) {
            try {
                if(std::stoi(nodeNames[i]) != i+1) {
                    std::cout << nodeNames[i] << ", " << i+1 << std::endl;
                    overwriteNames = true;
                    break;
                }
            } catch(...) {
                overwriteNames = true;
            }
        }
    } catch(...) {
        // If we caught here, that means there was an error with std::stoi(...) in the std::sort(...) method
        overwriteNames = true;
    }
    

    // If overwriteNames is now true, overwrite the names
    int popnCounter = 1;
    if(overwriteNames) {
        if(WARNINGS_ENABLED) {
            std::cerr << "WARNING: Taxon names do not appear to be natural numbers ascending from 1 (i.e. 1, 2, 3, 4, ...), so taxon names will be overwritten." << std::endl;
        }

        // Set the IDs
        for(unsigned int i = 0; i < activeNodes.size(); i++) {
            activeNodes[i]->setHiddenID(popnCounter++);
        }
    } else {
        // If the names are already good, we still have to set hidden IDs and progress the popnCounter
        for(unsigned int i = 0; i < activeNodes.size(); i++) {
            activeNodes[i]->setHiddenID(std::stoi(activeNodes[i]->getName()));
            popnCounter++;
        }
    }

    /* Go back in time from each of the leaves. Don't worry about times for now. Right now, if the node we're looking at
     * has two ancestors, this is a hybridization event and we need to name both of the ancestors.
     * If the node has one ancestor, this is a coalescence event. If this node's sister is not named, do not perform this
     * coalsecence yet. Wait for the node to be named.
     */
    // TODO: Definitely need to go through every event chronologically. Otherwise directionality is lost.
    // In order to do it chronologically: idk. have to think about this more.
    // Also: maybe we don't actually have to do it chronologically, and the direction can be preserved b/c we specifically
    //       assign MinorAnc() in a way that can be used to maintain directionality.
    while(activeNodes.size() > 0) {
        // Run a for loop that tries to resolve every node in activeNodes
        std::vector<int> removeMe;
        std::vector<Node*> addMe;
        unsigned int i;
        for(i = 0; i < activeNodes.size(); i++) {
            Node *p = activeNodes[i];
            if(p == NULL) {
                std::cerr << "ERROR: Active node is blank; quitting." << std::endl << std::flush;
                exit(-1);
            }

            Node *majAnc = p->getMajorAnc();
            Node *minAnc = p->getMinorAnc();
            // a. if only one ancestor (this will only ever take place when there is a MajorAnc and NOT a MinorAnc)
            if(majAnc != NULL && minAnc == NULL) {
                // I. if the anc is NOT named: take the anc
                if(majAnc->getHiddenID() == -1) {
                    majAnc->setHiddenID(p->getHiddenID());

                    // Remove p and add majAnc
                    removeMe.push_back(i);
                    addMe.push_back(majAnc);
                }
                // II. if anc named: coalesce *INTO* the ancestor
                else {
                    events.push_back(new MSJoinEvent(majAnc->getTime(), p->getHiddenID(), majAnc->getHiddenID()));

                    // Remove p; majAnc is already named, so if it still has more potential things to do, it should be
                    // in activeNodes already. So we don't need to touch it.
                    removeMe.push_back(i);
                }
            }
            // b. if we have two ancestors
            else if(majAnc != NULL && minAnc != NULL) {
                // I. both ancs unnamed: split, give MajorAnc our name and give MinorAnc a new name
                if(majAnc->getHiddenID() == -1 && minAnc->getHiddenID() == -1) {
                    // p is coalescing into majAnc, so gamma on this split comes from majAnc
                    double gamma = (majAnc->getLft() == p) ? majAnc->getGammaLft() : majAnc->getGammaRht();

                    // Split
                    events.push_back(new MSSplitEvent(p->getTime(), p->getHiddenID(), gamma));

                    // Give MajorAnc our name and add MajorAnc to activeNodes
                    majAnc->setHiddenID(p->getHiddenID());
                    addMe.push_back(majAnc);

                    // Give MinorAnc its new name and add it to activeNodes
                    minAnc->setHiddenID(popnCounter++);
                    addMe.push_back(minAnc);

                    // Remove p from activeNodes
                    removeMe.push_back(i);
                }
                // II. if only one anc is named: split, give the blank anc a new name, and then join with the named anc
                else if((majAnc->getHiddenID() == -1) != (minAnc->getHiddenID() == -1)) {
                    Node *namedAnc = (majAnc->getHiddenID() == -1) ? minAnc : majAnc;
                    Node *unnamedAnc = (majAnc->getHiddenID() == -1) ? majAnc : minAnc;

                    // p is coalescing into unnamedAnc, so gamma comes from unnamedAnc 
                    double gamma = (unnamedAnc->getLft() == p) ? unnamedAnc->getGammaLft() : unnamedAnc->getGammaRht();

                    // Split
                    events.push_back(new MSSplitEvent(p->getTime(), p->getHiddenID(), gamma));

                    // Give the blank anc a new name and add it to the queue
                    unnamedAnc->setHiddenID(popnCounter++);
                    addMe.push_back(unnamedAnc);

                    // Join
                    events.push_back(new MSJoinEvent(namedAnc->getTime(), p->getHiddenID(), namedAnc->getHiddenID()));

                    // Remove p
                    removeMe.push_back(i);
                }
                // III. if both ancs are named, split, then join both the new lineages with the existing ones
                else {
                    // p is coalescing into majAnc, so gamma comes from majAnc
                    double gamma = (majAnc->getLft() == p) ? majAnc->getGammaLft() : majAnc->getGammaRht();

                    // Split
                    events.push_back(new MSSplitEvent(p->getTime(), p->getHiddenID(), gamma));

                    // Join left
                    events.push_back(new MSJoinEvent(majAnc->getTime(), p->getHiddenID(), majAnc->getHiddenID()));

                    // Join right
                    events.push_back(new MSJoinEvent(minAnc->getTime(), popnCounter++, minAnc->getHiddenID()));

                    // Remove p
                    removeMe.push_back(i);
                }
            }
            // c. if we don't have ancestors, we are root, so just remove us
            else {
                removeMe.push_back(i);
            }
        }


        // Reverse sort the list so that we don't mess up indices as we go.
        std::sort(removeMe.begin(), removeMe.end(),
            [](int a, int b) {
                return(a > b);
            }
        );
        for(int idx : removeMe) {
            activeNodes.erase(std::next(activeNodes.begin(), idx));
        }
        // Add the addMe nodes to activeNodes
        for(Node *node : addMe) {
            activeNodes.push_back(node);
        }
    }

    // We have all of the events now, but ms has leaf times as 0, whereas we do not, so we need to flip the times
    for(MSEvent *e : events)
        e->setTime(endTime - e->getTime());

    sort(events.begin(), events.end(), [](MSEvent *a, MSEvent *b) {
        return a->getTime() < b->getTime();
    });
    return events;
}

int Network::activeNodesIdx(Node *p, std::vector<Node*> list) {
    for(unsigned int i = 0; i < list.size(); i++) {
        if(list[i] == p)
            return i;
    }
    return -1;
}