AM_CPPFLAGS      = @BOOST_CPPFLAGS@
AM_LDFLAGS       = @BOOST_LDFLAGS@

bin_PROGRAMS = omtt
omtt_SOURCES = main.cpp \
               ReadFile.cpp \
               RunProcess.cpp \
               TestExecutionSummary.cpp \
               ValidateExpectationsAndSutResults.cpp \
               lexer/detail/to_hex_string.cpp \
               lexer/Lexer.cpp \
               expectation/FullOutputExpectation.cpp \
               expectation/PartialOutputExpectation.cpp \
               system/Unix.cpp
omtt_LDADD   = @BOOST_PROGRAM_OPTIONS_LIB@
