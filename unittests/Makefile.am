check_PROGRAMS = lexer_tests \
                 parser_tests \
                 run_process_tests \
                 validate_expectations_and_sut_results_tests \
                 full_output_expectation_tests \
		 exit_code_expectation_tests

lexer_tests_SOURCES = main.cpp lexer/LexerTests.cpp
lexer_tests_LDADD = ../src/lexer/Lexer.o \
                    ../src/lexer/detail/to_hex_string.o

parser_tests_SOURCES = main.cpp parser/ParserTests.cpp
parser_tests_LDADD =  ../src/lexer/detail/to_hex_string.o

run_process_tests_SOURCES = main.cpp RunProcessTests.cpp system/UnixFake.cpp
run_process_tests_LDADD = ../src/RunProcess.o

validate_expectations_and_sut_results_tests_SOURCES = main.cpp ValidateExpectationsAndSutResultsTests.cpp
validate_expectations_and_sut_results_tests_LDADD = ../src/ValidateExpectationsAndSutResults.o

full_output_expectation_tests_SOURCES = main.cpp expectation/FullOutputExpectationTests.cpp

exit_code_expectation_tests_SOURCES = main.cpp expectation/ExitCodeExpectationTests.cpp

TESTS = $(check_PROGRAMS)
