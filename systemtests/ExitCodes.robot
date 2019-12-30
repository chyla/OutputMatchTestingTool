*** Settings ***
Resource    common.resource


*** Test Cases ***
Should exit with 0 status when executed test pass
    ${result} =    Run SUT With    scat    scat-will_return_empty_output_on_empty_input.omtt

    Should Be Equal As Integers    ${result.rc}    0

Should exit with 1 status when one executed test fail
    ${result} =    Run SUT With    scat    scat-failing_scenario_when_will_expect_text_on_empty_output.omtt

    Should Be Equal As Integers    ${result.rc}    1

Should exit with 251 status when executed with '--help' option
    ${result} =    Run SUT    args=--help

    Should Be Equal As Integers    ${result.rc}    251

Should exit with 251 status when executed with '--version' option
    ${result} =    Run SUT    args=--version

    Should Be Equal As Integers    ${result.rc}    251

Should exit with 252 status when executed without any option
    ${result} =    Run SUT    args=

    Should Be Equal As Integers    ${result.rc}    252

Should exit with 252 status when executed only with --sut option
    ${helper} =    Helper App Path    scat
    ${result} =    Run SUT    args=--sut=${helper}

    Should Be Equal As Integers    ${result.rc}    252

Should exit with 252 status when executed only with omtt test path
    ${some_correct_omtt_test_path} =    Omtt Test Path    scat-will_return_empty_output_on_empty_input.omtt
    ${result} =    Run SUT    args=${some_correct_omtt_test_path}

    Should Be Equal As Integers    ${result.rc}    252
