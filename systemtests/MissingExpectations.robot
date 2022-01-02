*** Comments ***
Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when no expectations are given explicitly in test
    ${result} =    Run SUT With Helper    true    test_with_no_expectations.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}
