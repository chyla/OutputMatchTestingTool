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
