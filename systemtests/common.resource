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
    ${result} =    Run SUT    args=--sut=${HELPER_APPS_DIR}/${helper_app} ${OMTT_TESTS_DIR}/${omtt_test}
    [Return]    ${result}
