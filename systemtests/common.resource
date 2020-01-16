*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Library    Process


*** Variables ***
${SUT_PATH} =   src/omtt
${HELPER_APPS_DIR} =   systemtests/apps
${OMTT_TESTS_DIR} =   systemtests/omtt


*** Keywords ***
Run SUT
    [Arguments]    ${args}
    Run Process    ${SUT_PATH} ${args}    shell=True    alias=job
    Process Should Be Stopped    job
    ${result} =    Get Process Result    job
    [Return]    ${result}

Run SUT With
    [Arguments]    ${helper_app}    ${omtt_test}
    ${helper_path} =    Helper App Path    ${helper_app}
    ${omtt_test_path} =    Omtt Test Path    ${omtt_test}
    ${result} =    Run SUT    args=--sut=${helper_path} ${omtt_test_path}
    [Return]    ${result}

Run SUT With Interpreter Argument And Raw Helper App Path
    [Arguments]    ${helper_interpreter}    ${helper_app}    ${omtt_test}
    ${interpreter_path} =    Helper App Path    ${helper_interpreter}
    ${omtt_test_path} =    Omtt Test Path    ${omtt_test}
    ${result} =    Run SUT    args=--interpreter=${interpreter_path} --sut=${helper_app} ${omtt_test_path}
    [Return]    ${result}

Run SUT With More Than One Interpreter Argument And Raw Helper App Path
    [Arguments]    ${helper_interpreter}    ${helper_app}    ${omtt_test}
    ${interpreter_path} =    Helper App Path    ${helper_interpreter}
    ${omtt_test_path} =    Omtt Test Path    ${omtt_test}
    ${result} =    Run SUT    args=--interpreter=${interpreter_path} --interpreter=${interpreter_path} --sut=${helper_app} ${omtt_test_path}
    [Return]    ${result}

Helper App Path
    [Arguments]    ${helper_app}
    [Return]    ${HELPER_APPS_DIR}/${helper_app}

Omtt Test Path
    [Arguments]    ${omtt_test}
    [Return]    ${OMTT_TESTS_DIR}/${omtt_test}

Verdict Is Set To Pass
    [Arguments]    ${result}
    Should Contain    ${result.stdout}    Verdict: PASS

Verdict Is Set To Fail
    [Arguments]    ${result}
    Should Contain    ${result.stdout}    Verdict: FAIL

Verdict Is Not Present
    [Arguments]    ${result}
    Should Not Contain    ${result.stdout}    Verdict: FAIL
    Should Not Contain    ${result.stdout}    Verdict: TRUE

Exit Status Points To All Tests Passed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    0

Exit Status Points To One Test Failed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    1

Exit Status Points To Fatal Error
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    255

Exit Status Points To Invalid Command Line Options
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    252

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
