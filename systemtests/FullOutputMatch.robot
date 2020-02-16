*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/MessageMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Mark test as PASS when no expectations is given and SUT outputs some text
    ${result} =    Run SUT With Helper    scat    scat-return_input_without_checking_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when empty output is matched
    ${result} =    Run SUT With Helper    scat    will_return_empty_output_on_empty_input.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when non-empty output is matched
    ${result} =    Run SUT With Helper    scat    scat-will_return_input_on_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when output and expected output differs in letter case
    ${result} =    Run SUT With Helper    scat    scat-expect_input_with_different_letter_case.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when output contains additional empty line at the end that is not expected
    ${result} =    Run SUT With Helper    scat    scat-will_return_input_with_aditional_empty_line_at_the_end_that_is_not_expected.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as FAIL when expecting additional empty line at the end
    ${result} =    Run SUT With Helper    scat    scat-expect_additional_empty_line_at_the_end.omtt

    Verdict Is Set To Fail    ${result}
    Exit Status Points To One Test Failed    ${result}

Mark test as PASS when keywords other than EXPECT occours in input and expected output
    ${result} =    Run SUT With Helper    scat    scat-keywords_in_input_and_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when EXPECT keyword occours in input and expected output after space
    ${result} =    Run SUT With Helper    scat    scat-expect_keyword_in_input_and_output_after_space.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when 3MB input match 3MB output
    ${result} =    Run SUT With Helper    scat    scat-3mb_input_match_3mb_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when 9MB input match 9MB output
    ${result} =    Run SUT With Helper    scat    scat-9mb_input_match_9mb_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as PASS when input is not read but output matches
    ${result} =    Run SUT With Helper    close_input_and_print_some_output    close_input_and_print_some_output-3mb_input_not_read_and_output_matches.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Mark test as FAIL when input is not read and output doesn't match
    ${result} =    Run SUT With Helper    close_input_and_print_some_output    close_input_and_print_some_output-failing_scenario-3mb_input_not_read_and_output_doesnt_match.omtt

    Verdict Is Set To Fail    ${result}
    Output Doesn't Match Message Points To Byte    ${result}    6

Missing keyword error message is not present when test PASS
    ${result} =    Run SUT With Helper    scat    scat-expect_keyword_in_input_and_output_after_space.omtt

    Verdict Is Set To Pass    ${result}
    Missing Keyword Message Is Not Present    ${result}

Raise missing keyword error when EXPECT keyword occours in expected output
    ${result} =    Run SUT With Helper    scat    scat-error_scenario-expect_keyword_in_output.omtt

    Verdict Is Not Present    ${result}
    Missing Keyword Message Is Present    ${result}    OUTPUT' or 'EXIT
    Exit Status Points To Fatal Error    ${result}

Output doesn't match message shouldn't be present when test PASS
    ${result} =    Run SUT With Helper    scat    scat-keywords_in_input_and_output.omtt

    Verdict Is Set To Pass    ${result}
    Output Doesn't Match Message Is Not Present    ${result}

Output doesn't match message should points to first byte when output and expected output differs on first byte
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-output_is_different_at_first_byte.omtt

    Output Doesn't Match Message Points To Byte    ${result}    0

Output doesn't match message should points to fifth byte when output and expected output differs on fifth byte
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-output_is_different_at_fifth_byte.omtt

    Output Doesn't Match Message Points To Byte    ${result}    4

Output doesn't match message should points to last output byte when output is shorten than expected output
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-output_is_shorten_than_expected_output.omtt

    Output Doesn't Match Message Points To Byte    ${result}    10

Output doesn't match message should points to last expected output byte byte when expected output is shorten than output
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-expected_output_is_shorten_than_output.omtt

    Output Doesn't Match Message Points To Byte    ${result}    10

Full match context message is present
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-output_is_different_at_fifth_byte.omtt

    Message Is Present    ${result}    ${full_match_context_message}

Full match context message should have changed special characters
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario-special_characters_at_input.omtt

    Message Is Present    ${result}    ${special_characters_message}


*** Settings ***
Variables    FullOutputMatchVariables.py
