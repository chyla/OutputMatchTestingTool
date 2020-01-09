*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common.resource


*** Test Cases ***
Mark test as PASS when no expectations is given and SUT outputs some text
    ${result} =    Run SUT With    scat    scat-return_input_without_checking_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

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

Mark test as PASS when keywords other than EXPECT occours in input and expected output
    ${result} =    Run SUT With    scat    scat-keywords_in_input_and_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when EXPECT keyword occours in input and expected output after space
    ${result} =    Run SUT With    scat    scat-expect_keyword_in_input_and_output_after_space.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Raise missing keyword error when EXPECT keyword occours in expected output
    ${result} =    Run SUT With    scat    scat-error_scenario-expect_keyword_in_output.omtt

    Verdict Is Not Present    ${result}
    Missing Keyword Message Is Present    ${result}    OUTPUT' or 'EXIT
    Exit Status Points To Fatal Error    ${result}
