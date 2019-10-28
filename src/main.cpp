/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include <iostream>

#include <boost/program_options.hpp>

#include "config.h"

namespace po = boost::program_options;


int
main(int argc, char **argv)
{
    po::options_description sutOptions("System under test");
    sutOptions.add_options()
        ("sut", po::value<std::string>(), "path to SUT")
        ;

    po::options_description miscOptions("Miscellaneous");
    miscOptions.add_options()
        ("help", "display this help text and exit")
        ("version", "display version information and exit")
        ;

    po::options_description cmdline_options;
    cmdline_options.add(sutOptions);
    cmdline_options.add(miscOptions);

    po::options_description hidden;
    hidden.add_options()
        ("test-file", po::value<std::string>(), "Test file to run.")
        ;

    po::options_description allOptions;
    allOptions.add(cmdline_options);
    allOptions.add(hidden);

    po::positional_options_description positional;
    positional.add("test-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(allOptions)
                  .positional(positional).run(),
              vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "USAGE: " << argv[0] << " [OPTION] --sut SUT_PATH TEST_FILE\n"
                  << cmdline_options;
        return 0;
    }

    if (vm.count("version")) {
        std::cout << argv[0] << " " << VERSION << '\n'
                  << "Copyright (c) 2019, Adam Chyła <adam@chyla.org>.\n"
                  << "All rights reserved.\n"
                  << "\n"
                  << "Distributed under the terms of the BSD 3-Clause License.\n";
        return 0;
    }

    std::cout << "Hello\n";

    return 0;
}
