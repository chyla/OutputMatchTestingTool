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

Helper App Path
    [Arguments]    ${helper_app}
    [Return]    ${HELPER_APPS_DIR}/${helper_app}

Omtt Test Path
    [Arguments]    ${omtt_test}
    [Return]    ${OMTT_TESTS_DIR}/${omtt_test}
