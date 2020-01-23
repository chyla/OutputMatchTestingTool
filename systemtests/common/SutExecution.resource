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
