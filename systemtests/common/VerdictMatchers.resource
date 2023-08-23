*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


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
