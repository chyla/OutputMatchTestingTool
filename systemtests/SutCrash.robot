*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/MessageMatchers.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when SUT crash but test has no expectations
    ${crashing_binary} =    Set Variable     crash_null_ptr_dereference
    ${result} =    Run SUT With Helper    ${crashing_binary}    test_with_no_expectations.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when SUT crash and test has exit code expectation with zero value
    ${some_existing_test} =    Set Variable    true-will_exit_with_zero.omtt
    ${crashing_binary} =    Set Variable     crash_null_ptr_dereference
    ${result} =    Run SUT With Helper    ${crashing_binary}    ${some_existing_test}

    Cause Has Max Integer Value As Received From Sut    ${result}
    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}


*** Keywords ***
Cause Has Max Integer Value As Received From Sut
    [Arguments]    ${result}
    Should Contain    ${result.stdout}    Got: 2147483647
