*** Comments ***
Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when output is empty using EMPTY keywords
    ${result} =    Run SUT With Helper    scat    scat-empty_input.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when input is empty with EMPTY keyword but multi word text is expected in output
    ${result} =    Run SUT With Helper    scat    scat-empty_input-match_multi_word_text_in_output.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}
