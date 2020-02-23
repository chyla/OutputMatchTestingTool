*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Keywords ***
Verdict Is Present
    [Arguments]    ${result}
    Should Contain    ${result.stdout}    Verdict:

Verdict Is Set To Pass
    [Arguments]    ${result}
    Should Contain    ${result.stdout}    Verdict: PASS

Verdict Is Set To Fail
    [Arguments]    ${result}
    Should Contain    ${result.stdout}    Verdict: FAIL

Verdict Is Not Present
    [Arguments]    ${result}
    Should Not Contain    ${result.stdout}    Verdict:
