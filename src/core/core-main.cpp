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


#include "Network.hpp"
#include "MSEvents.hpp"
#include "../SimSuite.hpp"
#include "Node.hpp"
#include "../tests/test_helpers.hpp"

#include <iostream>

namespace ss = SimSuite;

int main(int narg, char *argv[]) {
    std::string fifty = "(t50:18.0,(t26:17.0,(((t33:1.0,t46:1.0):10.0,#H51:1.0::0.17):5.0,(t10:15.0,((t11:1.0,t43:1.0):13.0,((( ((((t1:3.0,(t48:2.0,(t27:1.0)#H59:1.0::0.746):1.0):1.0,t17:4.0):1.0,(((t45:2.0,(t39:1.0)#H61:1.0::0.7 63):1.0,t28:3.0):1.0,#H61:3.0::0.237):1.0):1.0,((t38:1.0,t21:1.0):1.0,#H59:1.0::0.254):4.0):5.0,((((( t16:6.0,(((t5:3.0,(t19:2.0,(t42:1.0)#H65:1.0::0.542):1.0):1.0,#H65:3.0::0.458):1.0)#H63:1.0::0.758 ):1.0,t44:7.0):1.0,((t2:1.0,t15:1.0):5.0,#H63:1.0::0.242):2.0):1.0,((t18:4.0,(t20:3.0,#H67:1.0::0.27) :1.0):1.0,((t3:1.0,t31:1.0):1.0)#H67:3.0::0.73):4.0):1.0)#H51:1.0::0.83):1.0,((t7:2.0,(t37:1.0)#H53: 1.0::0.631):1.0,(t24:2.0,#H53:1.0::0.369):1.0):9.0):1.0,(((t23:8.0,(t13:7.0,#H55:1.0::0.105):1.0):1. 0,((((t40:1.0,t34:1.0):3.0,(((t12:1.0,t14:1.0):1.0,t30:2.0):1.0)#H69:1.0::0.504):1.0,((t32:2.0,(t41:1. 0,t4:1.0):1.0):2.0,#H69:1.0::0.496):1.0):1.0,t25:6.0):3.0):2.0,(t9:10.0,(t35:9.0,(t29:8.0,(t47:7.0,(((( t36:1.0,t22:1.0):3.0,(t6:3.0,(t8:2.0,#H57:1.0::0.374):1.0):1.0):1.0,(t49:1.0)#H57:4.0::0.626):1.0)# H55:1.0::0.895):1.0):1.0):1.0):1.0):2.0):1.0):1.0):1.0):1.0):1.0);";
    
    Network newickNet(fifty, "newick");

    std::string msArgs = ss::newickToMS(fifty);
    argsOnly(msArgs);

    std::cerr << msArgs << std::endl;

    Network msNet(msArgs, "ms");

    bool iso = isomorphic(newickNet, msNet);

    // std::cerr << std::endl << std::endl;
    // for(Node *p : msNet.getNodes())
    //     p->printInfo();

    std::cerr << "isomorphic(newickNet, msNet): " << (iso ? "true" : "false") << std::endl;

    return 0;
}