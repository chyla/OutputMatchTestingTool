*** Settings ***
Library    Process


*** Variables ***
${SUT_PATH} =   src/omtt


*** Keywords ***
Run SUT
    [Arguments]    ${args}
    Run Process    ${SUT_PATH} ${args}    shell=True    alias=job
    Process Should Be Stopped    job
    ${result} =    Get Process Result    job
    [Return]    ${result}
