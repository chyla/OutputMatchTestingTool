*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common.resource


*** Test Cases ***
Mark test as PASS when empty output is matched
    ${result} =    Run SUT With    scat    scat-will_return_empty_output_on_empty_input.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when non-empty output is matched
    ${result} =    Run SUT With    scat    scat-will_return_input_on_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when output and expected output differs in letter case
    ${result} =    Run SUT With    scat    scat-expect_input_with_different_letter_case.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when output contains additional empty line at the end that is not expected
    ${result} =    Run SUT With    scat    scat-will_return_input_with_aditional_empty_line_at_the_end_that_is_not_expected.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when expecting additional empty line at the end
    ${result} =    Run SUT With    scat    scat-expect_additional_empty_line_at_the_end.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}
