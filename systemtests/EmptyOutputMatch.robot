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

Variables    EmptyOutputMatchVariables.py


*** Test Cases ***
Mark test as PASS when output is empty
    ${result} =    Run SUT With Helper    scat    scat-empty_match.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when output is not empty
    ${result} =    Run SUT With Helper    scat    scat-empty_match-failing_scenario.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Expected empty output message is not present when test PASS
    ${result} =    Run SUT With Helper    scat    scat-empty_match.omtt

    Expected Empty Output Message Is Not Present    ${result}

Context output message is not present when test PASS
    ${result} =    Run SUT With Helper    scat    scat-empty_match.omtt

    Message Is Not Present    ${result}    Output context:

Expected empty output message is present when test FAIL
    ${result} =    Run SUT With Helper    scat    scat-empty_match-failing_scenario.omtt

    Expected Empty Output Message Is Present    ${result}

Empty output context message is present when test FAIL
    ${result} =    Run SUT With Helper    scat    scat-empty_match-failing_scenario.omtt

    Message Is Present    ${result}    ${some_text_context_message}
