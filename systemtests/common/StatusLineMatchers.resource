*** Comments ***
Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Keywords ***
Verify Status Line
    [Arguments]    ${result}    ${total}    ${pass}    ${fail}
    Should Contain    ${result.stdout}    ${total} tests total, ${pass} passed, ${fail} failed
