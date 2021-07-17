*** Comments ***
Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


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
