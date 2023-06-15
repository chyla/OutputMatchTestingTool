*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Allow single line comment at the top of the test file
    ${result} =    Run SUT With Helper    scat    comment-at-the-top-single-line.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Allow multiline comment at the top of the test file
    ${result} =    Run SUT With Helper    scat    comment-at-the-top-multi-line.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Allow multiline comment with multiple asterisks at the top of the test file
    ${result} =    Run SUT With Helper    scat    comment-at-the-top-multi-line-multiple-asterisks.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Text inside INPUT should not be a comment
    ${result} =    Run SUT With Helper    scat    comment-failing_scenario-inside-input.omtt

    Verdict Is Set To FAIL    ${result}
    Exit Status Points To One Test Failed    ${result}

Text inside OUTPUT should not be a comment
    ${result} =    Run SUT With Helper    scat    comment-failing_scenario-inside-output.omtt

    Verdict Is Set To FAIL    ${result}
    Exit Status Points To One Test Failed    ${result}

Text inside INPUT and OUTPUT should not be a comment
    ${result} =    Run SUT With Helper    scat    comment-inside-input-output-should-not-be-a-comment.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Text anywhere after RUN should not be a comment
    ${result} =    Run SUT With Helper    scat    comment-after-run-should-not-be-a-comment.omtt

    Exit Status Points To Fatal Error    ${result}
