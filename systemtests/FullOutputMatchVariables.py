full_match_context_message = "Expected context:\nS    o    m    e    _    o    t    h    e    r    SPC  \n                    ^                                  \n0x53 0x6f 0x6d 0x65 0x5f 0x6f 0x74 0x68 0x65 0x72 0x20 \nOutput context:\nS    o    m    e    SPC  l    o    n    g    SPC  t    \n                    ^                                  \n0x53 0x6f 0x6d 0x65 0x20 0x6c 0x6f 0x6e 0x67 0x20 0x74"
special_characters_message = (
"Expected context:\n\
\n\
\n\
\n\
Output context:\n\
SPC  TAB  LF   VT   FF   CR   NP   \n\
^                                  \n\
0x20 0x09 0x0a 0x0b 0x0c 0x0d 0x01")


class CheckMoreThanOneExpectStatement:
    first_full_match_not_found_message = "Output doesn't match.\n\
First difference at byte: 1\n\
Expected context:\n\
H    E    L    L    O    SPC  w    o    \n\
     ^                                  \n\
0x48 0x45 0x4c 0x4c 0x4f 0x20 0x77 0x6f \n\
Output context:\n\
H    e    l    l    o    SPC  w    o    \n\
     ^                                  \n\
0x48 0x65 0x6c 0x6c 0x6f 0x20 0x77 0x6f"

    third_full_match_not_found_message = "Output doesn't match.\n\
First difference at byte: 6\n\
Expected context:\n\
H    e    l    l    o    SPC  W    O    R    L    D    LF   \n\
                              ^                             \n\
0x48 0x65 0x6c 0x6c 0x6f 0x20 0x57 0x4f 0x52 0x4c 0x44 0x0a \n\
Output context:\n\
H    e    l    l    o    SPC  w    o    r    l    d    !    \n\
                              ^                             \n\
0x48 0x65 0x6c 0x6c 0x6f 0x20 0x77 0x6f 0x72 0x6c 0x64 0x21"
