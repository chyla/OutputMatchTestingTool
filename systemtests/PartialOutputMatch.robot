*** Comments ***
Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/MessageMatchers.resource
Resource    common/OmttExitStatusMatchers.resource

Variables    PartialOutputMatchVariables.py


*** Test Cases ***
Mark test as PASS when empty text is given in match
    ${result} =    Run SUT With Helper    scat    scat-partial_match-empty_text_match_part_of_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when new line character is present in SUT output
    ${result} =    Run SUT With Helper    scat    scat-partial_match-new_line_character.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when oneline text is present in SUT output
    ${result} =    Run SUT With Helper    scat    scat-partial_match-oneline_match_part_of_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when multiline text is present in SUT output
    ${result} =    Run SUT With Helper    scat    scat-partial_match-multiline_match_part_of_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when more than one partial expectation is given and text is in part of SUT output
    ${result} =    Run SUT With Helper    scat    scat-partial_match-more_than_one_text_expectation.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when text is not present in SUT output
    ${result} =    Run SUT With Helper    scat    scat-partial_match-failing_scenario-partial_match_text_not_found_in_sut_output.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when expectation text and part of SUT output differs in letter case
    ${result} =    Run SUT With Helper    scat    scat-partial_match-part_output_expectation_text_differs_in_letter_case.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as PASS when keyword other than EXPECT occours in expectation and SUT output
    ${result} =    Run SUT With Helper    scat    scat-partial_match-keyword_in_expectation.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when EXPECT keyword occours in expectation after space and is printed by SUT
    ${result} =    Run SUT With Helper    scat    scat-partial_match-expect_keyword_in_expectation.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Partial match context message should have changed special characters to readable form
    ${result} =    Run SUT With Helper    scat    scat-partial_match-failing_scenario-special_characters.omtt

    Message Is Present    ${result}    ${text_context_with_readable_special_characters}

Mark test as FAIL when more than one partial expectation is given and text is not in part of SUT output
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-partial_match-more_than_one_text_expectation_failed.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

    Message Is Present    ${result}    ${first_context_text_partial_match_failed}
    Message Is Present    ${result}    ${second_context_text_partial_match_failed}
    Message Is Present    ${result}    ${third_context_text_partial_match_failed}