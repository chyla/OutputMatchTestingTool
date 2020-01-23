*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when exit code and output are correct
    ${result} =    Run SUT With    scat    scat-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when exit code is different but output match
    ${result} =    Run SUT With    scat    scat-failing_scenario-exit_code_is_different_and_full_output_match.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when exit code match but output is different
    ${result} =    Run SUT With    scat    scat-failing_scenario-exit_code_match_and_full_output_is_different.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when exit code and output are different
    ${result} =    Run SUT With    scat    scat-failing_scenario-exit_code_is_different_and_full_output_is_different.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as PASS when output match is before exit code match
    ${result} =    Run SUT With    scat    scat-match_full_output_match_is_before_exit_code_match.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}
