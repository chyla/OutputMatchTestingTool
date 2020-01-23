*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Keywords ***
Message Is Present
    [Arguments]    ${result}    ${message}
    Should Contain    ${result.stdout}    ${message}

Missing Keyword Message Is Not Present
    [Arguments]    ${result}
    Should Not Contain    ${result.stderr}    (KEYWORD), but got nothing

Missing Keyword Message Is Present
    [Arguments]    ${result}    ${expectedKeyword}
    ${message} =    Catenate    Expected \'${expectedKeyword}\' (KEYWORD), but got nothing.

    Should Contain    ${result.stderr}    ${message}

Output Doesn't Match Message Is Not Present
    [Arguments]    ${result}
    Should Not Contain    ${result.stdout}    Output doesn't match.
    Should Not Contain    ${result.stdout}    difference at byte

Output Doesn't Match Message Points To Byte
    [Arguments]    ${result}    ${position}
    ${message} =    Catenate    Output doesn't match.\nFirst difference at byte: ${position}

    Should Contain    ${result.stdout}    ${message}

Missing Number Message Is Present
    [Arguments]    ${result}
    Should Contain    ${result.stderr}    Expected number, but got nothing.

Fatal Error During SUT Execution Message Is Present
    [Arguments]    ${result}
    Should Contain    ${result.stderr}    fatal error: during SUT execution:

Argument More Than Once Error Message Is Present
    [Arguments]    ${result}
    Should Contain    ${result.stderr}    commad line arguments error:
    Should Contain    ${result.stderr}    cannot be specified more than once

Invalid Exit Code Is Not Present
    [Arguments]    ${result}
    Should Not Contain    ${result.stdout}    Exit code doesn't match.

Invalid Exit Code Message Is Present
    [Arguments]    ${result}    ${expectedExitCode}    ${realExitCode}
    ${message} =    Catenate    Exit code doesn't match.\nExpected: ${expectedExitCode}\nGot: ${realExitCode}

    Should Contain    ${result.stdout}    ${message}

Unrecognised Argument Error Message Is Present
    [Arguments]    ${result}
    Should Contain    ${result.stderr}    commad line arguments error: unrecognised option
