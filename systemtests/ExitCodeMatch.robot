*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/MessageMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when SUT returns zero exit code and it's not checked explicitly in test
    ${result} =    Run SUT With Helper    true    will_return_empty_output_on_empty_input.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when SUT returns non-zero exit code and it's not checked explicitly in test
    ${result} =    Run SUT With Helper    false    will_return_empty_output_on_empty_input.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when SUT returns zero exit code and it's checked explicitly in test
    ${result} =    Run SUT With Helper    true    true-will_exit_with_zero.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when SUT returns non-zero exit code and it's checked explicitly in test
    ${result} =    Run SUT With Helper    false    false-will_exit_with_one.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when SUT returns exit code other than expected in test
    ${result} =    Run SUT With Helper    true    true-failing_scenario-will_exit_with_one.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as PASS when input is not read but output matches
    ${result} =    Run SUT With Helper    close_input_and_print_some_output    close_input_and_print_some_output-3mb_input_not_read_and_exit_code_matches.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when input is not read and output doesn't match
    ${result} =    Run SUT With Helper    close_input_and_print_some_output    close_input_and_print_some_output-failing_scenario-3mb_input_not_read_and_exit_code_doesnt_match.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Raise an error when the 'CODE' keyword is missing
    ${result} =    Run SUT With Helper    true    true-error_scenario-missing_code_keyword.omtt

    Verdict Is Not Present    ${result}
    Missing Keyword Message Is Present    ${result}    CODE
    Exit Status Points To Fatal Error    ${result}

Raise an error when the exit code value is missing
    ${result} =    Run SUT With Helper    true    true-error_scenario-missing_exit_code_value.omtt

    Verdict Is Not Present    ${result}
    Missing Number Message Is Present    ${result}
    Exit Status Points To Fatal Error    ${result}

Invalid Exit Code Message is not present when exit code is correct
    ${result} =    Run SUT With Helper    true    true-will_exit_with_zero.omtt

    Verdict Is Set To Pass    ${result}
    Invalid Exit Code Is Not Present    ${result}

Invalid Exit Code Message is present when exit code is invalid
    ${result} =    Run SUT With Helper    true    true-failing_scenario-will_exit_with_one.omtt

    Verdict Is Set To Fail    ${result}

    ${expectedExitCode} =    Set Variable    1
    ${realExitCode} =    Set Variable    0
    Invalid Exit Code Message Is Present    ${result}    ${expectedExitCode}    ${realExitCode}

Two Invalid Exit Code Message are present when exit code is invalid in two expectations
    ${result} =    Run SUT With Helper    true    true-failing_scenario-two_exit_code_expectations.omtt

    Verdict Is Set To Fail    ${result}

    ${expectedExitCode} =    Set Variable    1
    ${realExitCode} =    Set Variable    0
    Invalid Exit Code Message Is Present    ${result}    ${expectedExitCode}    ${realExitCode}

    ${secondExpectedExitCode} =    Set Variable    2
    ${secondRealExitCode} =    Set Variable    0
    Invalid Exit Code Message Is Present    ${result}    ${expectedExitCode}    ${realExitCode}

Allow each test keywords with values in separate lines
    ${result} =    Run SUT With Helper    true    true-keywords_and_values_in_separate_lines.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}
