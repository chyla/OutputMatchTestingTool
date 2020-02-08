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
Run SUT Process Without Waiting To Completion
    [Arguments]    @{options}
    ${handle} =    Start Process    ${SUT_PATH}     @{options}
    [Return]    ${handle}

Get SUT Process Results
    [Arguments]    ${handle}
    Wait For Process    handle=${handle}
    Process Should Be Stopped    handle=${handle}
    ${result} =    Get Process Result    handle=${handle}
    [Return]    ${result}

Run SUT Process
    [Arguments]    @{options}
    ${handler} =    Run SUT Process Without Waiting To Completion    @{options}
    ${result} =    Get SUT Process Results    ${handler}
    [Return]    ${result}

Run SUT With Helper
    [Arguments]    ${helper_app}    ${omtt_test}
    ${helper_path} =    Helper App Path    ${helper_app}
    ${omtt_test_path} =    Omtt Test Path    ${omtt_test}
    ${result} =    Run SUT Process    --sut=${helper_path}     ${omtt_test_path}
    [Return]    ${result}

Run SUT With Helper And Don't Wait For Finishing
    [Arguments]    ${helper_app}    ${omtt_test}
    ${helper_path} =    Helper App Path    ${helper_app}
    ${omtt_test_path} =    Omtt Test Path    ${omtt_test}
    ${handle} =    Run SUT Process Without Waiting To Completion    --sut=${helper_path}     ${omtt_test_path}
    [Return]    ${handle}

Run SUT With Interpreter Argument And Raw Helper App Path
    [Arguments]    ${helper_interpreter}    ${helper_app}    ${omtt_test}
    ${interpreter_path} =    Helper App Path    ${helper_interpreter}
    ${omtt_test_path} =    Omtt Test Path    ${omtt_test}
    ${result} =    Run SUT Process    --interpreter=${interpreter_path}    --sut=${helper_app}    ${omtt_test_path}
    [Return]    ${result}

Run SUT With More Than One Interpreter Argument And Raw Helper App Path
    [Arguments]    ${helper_interpreter}    ${helper_app}    ${omtt_test}
    ${interpreter_path} =    Helper App Path    ${helper_interpreter}
    ${omtt_test_path} =    Omtt Test Path    ${omtt_test}
    ${result} =    Run SUT Process    --interpreter=${interpreter_path}     --interpreter=${interpreter_path}     --sut=${helper_app}     ${omtt_test_path}
    [Return]    ${result}

Terminate SUT
    [Arguments]    ${handle}    ${signal}
    Send Signal To Process    ${signal}    handle=${handle}
    Wait Until SUT Process Handle The Signal

Children Process Is Not Running
    [Arguments]    ${handle}    ${children}
    ${pid} =     Get Process Id     ${handle}
    ${ps_command} =    Set Variable    ps -o user=,pid=,ppid=,gid=,command= -${pid}
    ${result} =    Run Process    ${ps_command}    shell=True
    Should Not Contain    ${result.stdout}    ${children}

Helper App Path
    [Arguments]    ${helper_app}
    [Return]    ${HELPER_APPS_DIR}/${helper_app}

Omtt Test Path
    [Arguments]    ${omtt_test}
    [Return]    ${OMTT_TESTS_DIR}/${omtt_test}

Wait Until SUT Process Handle The Signal
    Sleep    10ms
