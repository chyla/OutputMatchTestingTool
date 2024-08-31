*** Comments ***
Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/MessageMatchers.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Raise an error when SUT binary doesn't exists
    ${some_existing_test} =    Set Variable    interpreter-will_print_some_script_to_stdout.omtt
    ${non_existing_binary} =    Set Variable     some_non_existing_binary
    ${result} =    Run SUT With Helper    ${non_existing_binary}    ${some_existing_test}

    Fatal Error During SUT Execution Message Is Present     ${result}
    Verdict Is Not Present    ${result}
    Exit Status Points To Fatal Error    ${result}

Execute interpreter passed on command line with script specified in sut argument
    ${result} =    Run SUT With Interpreter Argument And Raw Helper App Path    secho    some_script    interpreter-will_print_some_script_to_stdout.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Raise an error when interpreter passed on command line doesn't exists
    ${some__non_important_omtt_test} =    Set Variable    interpreter-will_print_some_script_to_stdout.omtt
    ${result} =    Run SUT With Interpreter Argument And Raw Helper App Path    non_existing_interpreter    some_script    ${some__non_important_omtt_test}

    Fatal Error During SUT Execution Message Is Present     ${result}
    Verdict Is Not Present    ${result}
    Exit Status Points To Fatal Error    ${result}

Raise an error when interpreter is passed on command line more than one time
    ${some__non_important_omtt_test} =    Set Variable    interpreter-will_print_some_script_to_stdout.omtt
    ${result} =    Run SUT With More Than One Interpreter Argument And Raw Helper App Path    non_existing_interpreter    some_script    ${some__non_important_omtt_test}

    Argument More Than Once Error Message Is Present    ${result}
    Verdict Is Not Present    ${result}
    Exit Status Points To Invalid Command Line Options    ${result}

Raise an error when unknown option is passed on command line
    ${result} =    Run SUT Process    --some-non-existing-option

    Unrecognised Argument Error Message Is Present    ${result}
    Verdict Is Not Present    ${result}
    Exit Status Points To Invalid Command Line Options    ${result}
