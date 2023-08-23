*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


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
