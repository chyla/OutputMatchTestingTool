*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common.resource


*** Test Cases ***
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
