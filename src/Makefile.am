AM_CPPFLAGS      = @BOOST_CPPFLAGS@
AM_LDFLAGS       = @BOOST_LDFLAGS@

bin_PROGRAMS = omtt
omtt_SOURCES = main.cpp \
               ReadFile.cpp \
               lexer/detail/to_hex_string.cpp \
               lexer/Lexer.cpp
omtt_LDADD   = @BOOST_PROGRAM_OPTIONS_LIB@
