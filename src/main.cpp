/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
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
#include <algorithm>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

typedef std::string Path;
typedef std::vector<Path> TestPaths;

const char * const license_text = R"(BSD 3-Clause License

Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.)";


TestPaths::size_type
RunAllTests(std::optional<Path> interpreter, const Path &sut, const TestPaths &tests);

omtt::TestExecutionSummary
ExecuteTest(std::optional<Path> interpreter, const Path &sut, const Path &test);

int
main(int argc, char **argv)
{
    po::variables_map vm;

    try {
        po::options_description sutOptions("System under test");
        sutOptions.add_options()
            ("sut", po::value<Path>(), "path to SUT")
            ;

        po::options_description interpreterOptions("Interpreter");
        interpreterOptions.add_options()
            ("interpreter", po::value<Path>(), "path to interpreter")
            ;

        po::options_description miscOptions("Miscellaneous");
        miscOptions.add_options()
            ("help", "display this help text and exit")
            ("version", "display version information and exit")
            ("license", "display license text")
            ;

        po::options_description cmdline_options;
        cmdline_options.add(sutOptions);
        cmdline_options.add(interpreterOptions);
        cmdline_options.add(miscOptions);

        po::options_description hidden;
        hidden.add_options()
          ("test-file", po::value<TestPaths>(), "Test files to run.")
            ;

        po::options_description allOptions;
        allOptions.add(cmdline_options);
        allOptions.add(hidden);

        po::positional_options_description positional;
        positional.add("test-file", -1);

        po::store(po::command_line_parser(argc, argv)
                      .options(allOptions)
                      .positional(positional).run(),
                  vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << "USAGE: " << argv[0] << " [OPTION] --sut SUT_PATH TEST_FILE...\n"
                         "\nTesting tool for checking programs console output.\n"
                      << cmdline_options;
            return omtt::INFORMATION_PRINTED;
        }

        if (vm.count("version")) {
            std::cout << argv[0] << " " << VERSION << '\n'
                      << "Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.\n"
                      << "All rights reserved.\n"
                      << "\n"
                      << "Distributed under the terms of the BSD 3-Clause License.\n";
            return omtt::INFORMATION_PRINTED;
        }

        if (vm.count("license")) {
            std::cout << license_text << '\n';
            return omtt::INFORMATION_PRINTED;
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

    const TestPaths &testFiles = vm["test-file"].as<TestPaths>();
    const Path &sut = vm["sut"].as<std::string>();
    std::optional<Path> interpreter;

    if (vm.count("interpreter") == 1) {
        interpreter = vm["interpreter"].as<std::string>();
    }

    std::cout << "Testing: " << sut << '\n';

    try {
        TestPaths::size_type numberOfTestsFailed = RunAllTests(interpreter, sut, testFiles);
        return std::min<TestPaths::size_type>(numberOfTestsFailed, omtt::MAX_TESTS_FAILED);
    }
    catch (std::exception &ex) {
        std::cerr << "fatal error: " << ex.what() << "\n";
        return omtt::FATAL_ERROR;
    }
}

TestPaths::size_type
RunAllTests(std::optional<Path> interpreter, const Path &sut, const TestPaths &tests)
{
    TestPaths::size_type executedTests = 0;
    TestPaths::size_type numberOfTestsFailed = 0;

    for (const auto &test : tests) {
        ++executedTests;
        std::cout << "====================\n"
                     "Running test (" << executedTests << "/" << tests.size() << "): " << test << '\n';

        const omtt::TestExecutionSummary &summary = ExecuteTest(interpreter, sut, test);
        std::cout << summary << '\n';

        if (summary.verdict != omtt::Verdict::PASS) {
            ++numberOfTestsFailed;
        }
    }

    std::cout << "====================\n"
              << tests.size() << " tests total, " << (tests.size() - numberOfTestsFailed) << " passed, " << numberOfTestsFailed << " failed\n";

    return numberOfTestsFailed;
}

omtt::TestExecutionSummary
ExecuteTest(std::optional<Path> interpreter, const Path &sut, const Path &test)
{
    const std::string &testFileBuffer = omtt::readFile(test);
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

    return omtt::ValidateExpectationsAndSutResults(testData, processResults);
}
