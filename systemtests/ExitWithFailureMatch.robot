*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/MessageMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when SUT returns with exit code other than zero
    ${result} =    Run SUT With Helper    false    exit_with_failure.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Exit Status Doesnt Match Message Is Not Present when SUT returns exit code other than zero
    ${result} =    Run SUT With Helper    false    exit_with_failure.omtt

    Exit Status Doesnt Match Message Is Not Present    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when SUT returns with zero exit code
    ${result} =    Run SUT With Helper    true    exit_with_failure.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Exit Status Doesnt Match Message Is Present when SUT returns zero exit code
    ${result} =    Run SUT With Helper    true    exit_with_failure.omtt

    Exit Status Doesnt Match Message Is Present    ${result}
    Exit Status Points To One Test Failed    ${result}

Exit Code Is Present when SUT returns zero exit code
    ${result} =    Run SUT With Helper    true    exit_with_failure.omtt

    ${realExitCode} =    Set Variable    0
    Exit Code Is Present    ${result}    ${realExitCode}
    Exit Status Points To One Test Failed    ${result}

Exit Status Description Is Present when SUT returns exit code zero
    ${result} =    Run SUT With Helper    true    exit_with_failure.omtt

    Should Contain    ${result.stdout}    Expected: exit with failure
    Should Contain    ${result.stdout}    Got: exit with success
