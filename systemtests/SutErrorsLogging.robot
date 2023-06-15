*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutErrorsLoggingMatchers.resource
Resource    common/SutExecution.resource
Resource    common/MessageMatchers.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Should not contain error messages when SUT doesn't print them
    ${result} =    Run SUT With Helper    scat    scat-will_return_input_on_output.omtt

    SUT Errors Header Is Not Present    ${result}
    Exit Status Points To All Tests Passed    ${result}

Should contain error messages header when SUT prints errors
    ${result} =    Run SUT With Helper    scaterr    scaterr-error_text_on_input.omtt

    SUT Errors Header Is Present    ${result}

Should contain error messages when SUT prints errors
    ${result} =    Run SUT With Helper    scaterr    scaterr-error_text_on_input.omtt

    Message Is Present    ${result}    example error message

Should mark test as PASS when SUT prints errors and expectations are meet
    ${result} =    Run SUT With Helper    scaterr    scaterr-expect_exit_zero.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Should mark test as FAIL when SUT prints errors and expectations are not meet
    ${result} =    Run SUT With Helper    scaterr    scaterr-failing_scenario-expect_exit_one.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Should contain error messages from each test when SUT prints errors
    ${result} =    Run SUT With Helper    scaterr    scaterr-error_text_on_input.omtt    scaterr-other_error_text_on_input.omtt

    Message Is Present    ${result}    example error message
    Message Is Present    ${result}    other error message
