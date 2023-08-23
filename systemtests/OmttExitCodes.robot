*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Should exit with 0 status when executed test pass
    ${result} =    Run SUT With Helper    scat    will_return_empty_output_on_empty_input.omtt

    Exit Status Points To All Tests Passed    ${result}

Should exit with 1 status when one executed test fail
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario_when_will_expect_text_on_empty_output.omtt

    Exit Status Points To One Test Failed    ${result}

Should exit with 61 status when executed with '--help' option
    ${result} =    Run SUT Process    --help

    Exit Status Points To Information Printed    ${result}

Should exit with 61 status when executed with '--version' option
    ${result} =    Run SUT Process    --version

    Exit Status Points To Information Printed    ${result}

Should exit with 61 status when executed with '--license' option
    ${result} =    Run SUT Process    --license

    Exit Status Points To Information Printed    ${result}

Should exit with 64 status when executed without any option
    ${result} =    Run SUT Process

    Exit Status Points To Invalid Command Line Options    ${result}

Should exit with 64 status when executed only with --sut option
    ${helper} =    Helper App Path    scat
    ${result} =    Run SUT Process    --sut=${helper}

    Exit Status Points To Invalid Command Line Options    ${result}

Should exit with 64 status when executed only with omtt test path
    ${some_correct_omtt_test_path} =    Omtt Test Path    will_return_empty_output_on_empty_input.omtt
    ${result} =    Run SUT Process    args=${some_correct_omtt_test_path}

    Exit Status Points To Invalid Command Line Options    ${result}

Should exit with 64 status when executed with non-existing option
    ${result} =    Run SUT Process    --some-non-existing-option

    Exit Status Points To Invalid Command Line Options    ${result}

Should exit with 60 status when executed with non existing test file
    ${non_existing_test} =    Set Variable    non_existing_test.omtt
    ${existing_binary} =    Set Variable     scat
    ${result} =    Run SUT With Helper    ${existing_binary}    ${non_existing_test}

    Exit Status Points To Fatal Error    ${result}
