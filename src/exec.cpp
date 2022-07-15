#include <boost/program_options.hpp>

#include <iostream>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "print ussage message")
        ("file", po::value<std::string>(), "file location")
        ("newick", po::value<std::string>(), "Newick formatted network or tree")
    ;
    po::positional_options_description p;
    p.add("file", -1);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    
    if(vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }
    if(vm.count("file")) {
        std::cout << "File my guy" << std::endl;
    }
    if(vm.count("newick")) {
        std::cout << "Newick my dude" << std::endl;
    }
}