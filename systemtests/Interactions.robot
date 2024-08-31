*** Comments ***
Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when exit code and output are correct
    ${result} =    Run SUT With Helper    scat    scat-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when exit code is different but output match
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-exit_code_is_different_and_full_output_match.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when exit code match but output is different
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-exit_code_match_and_full_output_is_different.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when exit code and output are different
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-exit_code_is_different_and_full_output_is_different.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as PASS when output match is before exit code match
    ${result} =    Run SUT With Helper    scat    scat-match_full_output_match_is_before_exit_code_match.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when exit code, full output and partial output match are correct
    ${result} =    Run SUT With Helper    scat    scat-match_exit_code_and_full_output_and_partial_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when exit code, full output match are coorect but partial output match failed
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-match_exit_code_and_full_output_and_incorrect_partial_output.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}
