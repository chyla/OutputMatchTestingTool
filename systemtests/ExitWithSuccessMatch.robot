*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/MessageMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when SUT returns zero exit code
    ${result} =    Run SUT With Helper    true    exit_with_success.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Exit Status Doesnt Match Message Is Not Present when SUT returns zero exit code
    ${result} =    Run SUT With Helper    true    exit_with_success.omtt

    Exit Status Doesnt Match Message Is Not Present    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when SUT returns exit code other than zero
    ${result} =    Run SUT With Helper    false    exit_with_success.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Exit Status Doesnt Match Message Is Present when SUT returns exit code other than zero
    ${result} =    Run SUT With Helper    false    exit_with_success.omtt

    Exit Status Doesnt Match Message Is Present    ${result}
    Exit Status Points To One Test Failed    ${result}

Exit Code Is Present when SUT returns exit code other than zero
    ${result} =    Run SUT With Helper    false    exit_with_success.omtt

    ${realExitCode} =    Set Variable    1
    Exit Code Is Present    ${result}    ${realExitCode}
    Exit Status Points To One Test Failed    ${result}

Exit Status Description Is Present when SUT returns exit code other than zero
    ${result} =    Run SUT With Helper    false    exit_with_success.omtt

    Should Contain    ${result.stdout}    Expected: exit with success
    Should Contain    ${result.stdout}    Got: exit with failure
