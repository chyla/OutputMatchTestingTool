*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


*** Keywords ***
SUT Errors Header Is Not Present
    [Arguments]    ${result}
    Should Not Contain    ${result.stdout}    => SUT error messages printed during test execution:

SUT Errors Header Is Present
    [Arguments]    ${result}
    Should Contain    ${result.stdout}    => SUT error messages printed during test execution:
