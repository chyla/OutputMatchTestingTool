AM_CPPFLAGS      = -I$(top_srcdir)
EXTRA_DIST       = doctest             \
                   lexer/LexerFake.hpp \
                   system/UnixFake.hpp \
                   test_framework.hpp

check_PROGRAMS = lexer_tests \
                 logger_tests \
                 parser_tests \
                 run_process_tests \
                 validate_expectations_and_sut_results_tests \
                 empty_output_expectation_tests \
                 full_output_expectation_tests \
                 partial_output_expectation_tests \
                 exit_code_expectation_tests \
                 line_endings_tests

lexer_tests_SOURCES = main.cpp lexer/LexerTests.cpp
lexer_tests_LDADD = ../src/lexer/Lexer.o \
                    ../src/lexer/detail/to_hex_string.o

logger_tests_SOURCES = main.cpp logger/ConsoleLoggerTests.cpp
logger_tests_LDADD = ../src/logger/ConsoleLogger.o

parser_tests_SOURCES = main.cpp parser/ParserTests.cpp
parser_tests_LDADD =  ../src/lexer/detail/to_hex_string.o \
                      ../src/expectation/FullOutputExpectation.o \
                      ../src/expectation/PartialOutputExpectation.o

run_process_tests_SOURCES = main.cpp RunProcessTests.cpp system/UnixFake.cpp
run_process_tests_LDADD = ../src/RunProcess.o

validate_expectations_and_sut_results_tests_SOURCES = main.cpp ValidateExpectationsAndSutResultsTests.cpp
validate_expectations_and_sut_results_tests_LDADD = ../src/ValidateExpectationsAndSutResults.o

empty_output_expectation_tests_SOURCES = main.cpp \
                                         expectation/EmptyOutputExpectationTests.cpp

full_output_expectation_tests_SOURCES = main.cpp \
                                        expectation/FullOutputExpectationTests.cpp
full_output_expectation_tests_LDADD =  ../src/expectation/FullOutputExpectation.o

partial_output_expectation_tests_SOURCES = main.cpp \
                                           expectation/PartialOutputExpectationTests.cpp
partial_output_expectation_tests_LDADD =  ../src/expectation/PartialOutputExpectation.o

exit_code_expectation_tests_SOURCES = main.cpp expectation/ExitCodeExpectationTests.cpp

line_endings_tests_SOURCES = main.cpp LineEndingsTests.cpp

TESTS = $(check_PROGRAMS)
