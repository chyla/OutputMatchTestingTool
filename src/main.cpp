/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "config.h"
#include "headers/ReadFile.hpp"
#include "headers/TestData.hpp"
#include "headers/lexer/Lexer.hpp"
#include "headers/parser/Parser.hpp"
#include "headers/RunProcess.hpp"
#include "headers/TestExecutionSummary.hpp"
#include "headers/ValidateExpectationsAndSutResults.hpp"
#include "headers/ErrorCodes.hpp"

#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;


int
main(int argc, char **argv)
{
    po::variables_map vm;

    try {
        po::options_description sutOptions("System under test");
        sutOptions.add_options()
            ("sut", po::value<std::string>(), "path to SUT")
            ;

        po::options_description interpreterOptions("Interpreter");
        interpreterOptions.add_options()
            ("interpreter", po::value<std::string>(), "path to interpreter")
            ;

        po::options_description miscOptions("Miscellaneous");
        miscOptions.add_options()
            ("help", "display this help text and exit")
            ("version", "display version information and exit")
            ;

        po::options_description cmdline_options;
        cmdline_options.add(sutOptions);
        cmdline_options.add(interpreterOptions);
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

        po::store(po::command_line_parser(argc, argv)
                      .options(allOptions)
                      .positional(positional).run(),
                  vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << "USAGE: " << argv[0] << " [OPTION] --sut SUT_PATH TEST_FILE\n"
                      << cmdline_options;
            return omtt::HELP_OR_VERSION_INFORMATION_PRINTED;
        }

        if (vm.count("version")) {
            std::cout << argv[0] << " " << VERSION << '\n'
                      << "Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.\n"
                      << "All rights reserved.\n"
                      << "\n"
                      << "Distributed under the terms of the BSD 3-Clause License.\n";
            return omtt::HELP_OR_VERSION_INFORMATION_PRINTED;
        }
    }
    catch (std::exception &ex) {
        std::cerr << "commad line arguments error: " << ex.what() << '\n';
        return omtt::INVALID_COMMAND_LINE_OPTIONS;
    }

    if (vm.count("test-file") == 0) {
        std::cerr << "commad line arguments error: missing test file path.\n";
        return omtt::INVALID_COMMAND_LINE_OPTIONS;
    }

    if (vm.count("sut") == 0) {
        std::cerr << "commad line arguments error: missing sut.\n";
        return omtt::INVALID_COMMAND_LINE_OPTIONS;
    }

    const std::string testFile = vm["test-file"].as<std::string>();
    const std::string sut = vm["sut"].as<std::string>();
    std::optional<std::string> interpreter;

    if (vm.count("interpreter") == 1) {
        interpreter = vm["interpreter"].as<std::string>();
    }

    int numberOfTestsFailed = 0;

    std::cout << "Testing: " << sut << '\n';
    std::cout << "Running test: " << testFile << '\n';

    try {
        const std::string &testFileBuffer = omtt::readFile(testFile);
        omtt::lexer::Lexer lexer(testFileBuffer);
        omtt::parser::Parser parser(lexer);

        const omtt::TestData &testData = parser.parse();

        omtt::ProcessResults processResults;
        if (interpreter.has_value()) {
            processResults = omtt::RunProcess(*interpreter, {sut}, testData.input);
        }
        else {
            processResults = omtt::RunProcess(sut, {}, testData.input);
        }

        const omtt::TestExecutionSummary &summary = omtt::ValidateExpectationsAndSutResults(testData, processResults);
        std::cout << summary << '\n';

        if (summary.verdict != omtt::Verdict::PASS) {
            ++numberOfTestsFailed;
        }
    }
    catch (std::exception &ex) {
        std::cerr << "fatal error: " << ex.what() << "\n";
        return omtt::FATAL_ERROR;
    }

    return numberOfTestsFailed;
}
