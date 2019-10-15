check_PROGRAMS = main lexer_tests parser_tests
main_SOURCES = main.cpp example.cpp

lexer_tests_SOURCES = main.cpp lexer/LexerTests.cpp
lexer_tests_LDADD = ../src/lexer/Lexer.o

parser_tests_SOURCES = main.cpp parser/ParserTests.cpp


TESTS = $(check_PROGRAMS)
