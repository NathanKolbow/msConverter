#ifndef SIMSUITE_SIMSUITE_HPP_
#define SIMSUITE_SIMSUITE_HPP_

#include <string>
#include <vector>

namespace SimSuite {
    std::string newickToMS(std::string newickStr);
    std::vector<std::string> newickFileToMS(std::string location);
    
    std::string newickToMSSafe(std::string newickStr);
    std::vector<std::string> newickFileToMSSafe(std::string location);
    
    std::string newickToMS(std::string newickStr, int ntrees);
    std::vector<std::string> newickFileToMS(std::string location, int ntrees);
    
    std::string newickToMSSafe(std::string newickStr, int ntrees);
    std::vector<std::string> newickFileToMSSafe(std::string location, int ntrees);

    std::string msToNewick(std::string msStr);
    std::string msFromUserInputNewick(void);
    void disableWarnings(void);
}

#endif