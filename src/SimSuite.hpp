#ifndef SIMSUITE_SIMSUITE_HPP_
#define SIMSUITE_SIMSUITE_HPP_

#include <string>
#include <vector>

namespace SimSuite {
    std::string newickToMS(std::string newickStr, int ntrees, bool coalescent2N);
    std::vector<std::string> newickFileToMS(std::string location, int ntrees, bool coalescent2N);
    
    std::string newickToMSSafe(std::string newickStr, int ntrees, bool coalescent2N);
    std::vector<std::string> newickFileToMSSafe(std::string location, int ntrees, bool coalescent2N);

    std::string msToNewick(std::string msStr);
    void disableWarnings(void);
}

#endif