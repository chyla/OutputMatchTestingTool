*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Keywords ***
SUT Errors Header Is Not Present
    [Arguments]    ${result}
    Should Not Contain    ${result.stdout}    => SUT error messages printed during test execution:

SUT Errors Header Is Present
    [Arguments]    ${result}
    Should Contain    ${result.stdout}    => SUT error messages printed during test execution:
