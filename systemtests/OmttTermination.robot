*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
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

Terminate children process when killed with SIGPIPE
    Run Parameterized Test    SIGPIPE

Terminate children process when killed with SIGTERM
    Run Parameterized Test    SIGTERM

Terminate children process when killed with SIGUSR1
    Run Parameterized Test    SIGUSR1

Terminate children process when killed with SIGUSR2
    Run Parameterized Test    SIGUSR2


*** Keywords ***
Run Parameterized Test
    [Arguments]    ${signal}
    ${children_process} =    Set Variable    sleep60
    ${handle} =    Run SUT With Helper And Don't Wait For Finishing    ${children_process}    will_return_empty_output_on_empty_input.omtt

    Wait Until Children Process Is Running

    Terminate SUT    ${handle}    ${signal}

    Children Process Is Not Running    ${handle}    ${children_process}

    ${result} =    Get SUT Process Results    ${handle}

    Verdict Is Not Present    ${result}
    Fatal Error Message Is Not Present    ${result}

Wait Until Children Process Is Running
    Sleep    10ms
