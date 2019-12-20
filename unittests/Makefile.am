check_PROGRAMS = lexer_tests parser_tests  run_process_tests

lexer_tests_SOURCES = main.cpp lexer/LexerTests.cpp
lexer_tests_LDADD = ../src/lexer/Lexer.o \
                    ../src/lexer/detail/to_hex_string.o

parser_tests_SOURCES = main.cpp parser/ParserTests.cpp
parser_tests_LDADD =  ../src/lexer/detail/to_hex_string.o

run_process_tests_SOURCES = main.cpp RunProcessTests.cpp system/UnixFake.cpp
run_process_tests_LDADD = ../src/RunProcess.o

TESTS = $(check_PROGRAMS)
