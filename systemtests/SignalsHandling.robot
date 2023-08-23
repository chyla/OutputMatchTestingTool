*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/MessageMatchers.resource


*** Test Cases ***
Terminate children process when killed with SIGHUP
    Run Parameterized Test    SIGHUP

Terminate children process when killed with SIGINT
    Run Parameterized Test    SIGINT

Terminate children process when killed with SIGTERM
    Run Parameterized Test    SIGTERM

Terminate children process when killed with SIGUSR1
    Run Parameterized Test    SIGUSR1

Terminate children process when killed with SIGUSR2
    Run Parameterized Test    SIGUSR2

Ignore SIGPIPE signal while children process is running
    ${signal} =    Set Variable    SIGPIPE
    ${children_process} =    Set Variable    sleep3
    ${handle} =    Run SUT With Helper And Don't Wait For Finishing    ${children_process}    will_return_empty_output_on_empty_input.omtt

    Wait Until Children Process Is Running

    Send Signal To SUT    ${handle}    ${signal}

    ${result} =    Get SUT Process Results    ${handle}

    Children Process Is Not Running    ${handle}    ${children_process}

    Verdict Is Present    ${result}
    Fatal Error Message Is Not Present    ${result}


*** Keywords ***
Run Parameterized Test
    [Arguments]    ${signal}
    ${children_process} =    Set Variable    sleep3
    ${handle} =    Run SUT With Helper And Don't Wait For Finishing    ${children_process}    will_return_empty_output_on_empty_input.omtt

    Wait Until Children Process Is Running

    Send Signal To SUT    ${handle}    ${signal}

    Children Process Is Not Running    ${handle}    ${children_process}

    ${result} =    Get SUT Process Results    ${handle}

    Verdict Is Not Present    ${result}
    Signal Received Fatal Error Message Is Present    ${result}

Wait Until Children Process Is Running
    Sleep    50ms
