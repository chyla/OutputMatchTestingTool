*** Settings ***
Resource    common.resource


*** Test Cases ***
Should exit with 251 status when executed with '--help' option
    ${result} =    Run SUT    args=--help

    Should Be Equal As Integers    ${result.rc}    251

Should exit with 251 status when executed with '--version' option
    ${result} =    Run SUT    args=--version

    Should Be Equal As Integers    ${result.rc}    251

Should exit with 252 status when executed without any option
    ${result} =    Run SUT    args=

    Should Be Equal As Integers    ${result.rc}    252
