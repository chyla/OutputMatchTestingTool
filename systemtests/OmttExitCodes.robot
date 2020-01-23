*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Should exit with 0 status when executed test pass
    ${result} =    Run SUT With    scat    scat-will_return_empty_output_on_empty_input.omtt

    Exit Status Points To All Tests Passed    ${result}

Should exit with 1 status when one executed test fail
    ${result} =    Run SUT With    scat    scat-failing_scenario_when_will_expect_text_on_empty_output.omtt

    Exit Status Points To One Test Failed    ${result}

Should exit with 251 status when executed with '--help' option
    ${result} =    Run SUT    args=--help

    Exit Status Points To Help Or Version Information Printed    ${result}

Should exit with 251 status when executed with '--version' option
    ${result} =    Run SUT    args=--version

    Exit Status Points To Help Or Version Information Printed    ${result}

Should exit with 252 status when executed without any option
    ${result} =    Run SUT    args=

    Exit Status Points To Invalid Command Line Options    ${result}

Should exit with 252 status when executed only with --sut option
    ${helper} =    Helper App Path    scat
    ${result} =    Run SUT    args=--sut=${helper}

    Exit Status Points To Invalid Command Line Options    ${result}

Should exit with 252 status when executed only with omtt test path
    ${some_correct_omtt_test_path} =    Omtt Test Path    scat-will_return_empty_output_on_empty_input.omtt
    ${result} =    Run SUT    args=${some_correct_omtt_test_path}

    Exit Status Points To Invalid Command Line Options    ${result}

Should exit with 252 status when executed with non-existing option
    ${result} =    Run SUT    args=--some-non-existing-option

    Exit Status Points To Invalid Command Line Options    ${result}

Should exit with 255 status when executed with non existing test file
    ${non_existing_test} =    Set Variable    non_existing_test.omtt
    ${existing_binary} =    Set Variable     scat
    ${result} =    Run SUT With    ${existing_binary}    ${non_existing_test}

    Exit Status Points To Fatal Error    ${result}
