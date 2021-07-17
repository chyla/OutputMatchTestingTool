*** Comments ***
Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource
Resource    common/StatusLineMatchers.resource
Library    common/TestExecutionMatchers.py


*** Test Cases ***
Mark all tests as PASS when executing two successfull tests
    ${result} =    Run SUT With Helper    scat    scat-return_input_without_checking_output.omtt    will_return_empty_output_on_empty_input.omtt

    Test Was Executed With Pass      ${result}    scat-return_input_without_checking_output.omtt
    Test Was Executed With Pass      ${result}    will_return_empty_output_on_empty_input.omtt

    Verify Status Line    ${result}    total=2    pass=2    fail=0
    Exit Status Points To All Tests Passed    ${result}

Mark one test as PASS and one as FAIL when executing one passing and one failing test
    ${result} =    Run SUT With Helper    scat    scat-return_input_without_checking_output.omtt    scat-failing_scenario-output_is_shorten_than_expected_output.omtt

    Test Was Executed With Pass      ${result}    scat-return_input_without_checking_output.omtt
    Test Was Executed With Fail      ${result}    scat-failing_scenario-output_is_shorten_than_expected_output.omtt

    Verify Status Line    ${result}    total=2    pass=1    fail=1
    Exit Status Points To One Test Failed    ${result}

Mark one test as PASS and two tests as FAIL when first is failing, second is passing, third is failing
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario_when_will_expect_text_on_empty_output.omtt    scat-return_input_without_checking_output.omtt    scat-failing_scenario-output_is_shorten_than_expected_output.omtt

    Test Was Executed With Fail      ${result}    scat-failing_scenario_when_will_expect_text_on_empty_output.omtt
    Test Was Executed With Pass      ${result}    scat-return_input_without_checking_output.omtt
    Test Was Executed With Fail      ${result}    scat-failing_scenario-output_is_shorten_than_expected_output.omtt

    Verify Status Line    ${result}    total=3    pass=1    fail=2
    Exit Status Points To Two Tests Failed    ${result}

Exit status should point to maximum tests failed when 51 tests fail
    @{tests} =    Create Same Tests Names List    number=51    test_name=scat-failing_scenario-output_is_shorten_than_expected_output.omtt
    ${result} =    Run SUT With Helper    scat    @{tests}

    Exit Status Points To Maximum Tests Failed    ${result}

Status line should point to number of tests failed even when more than maximum tests fail
    @{tests} =    Create Same Tests Names List    number=51    test_name=scat-failing_scenario-output_is_shorten_than_expected_output.omtt
    ${result} =    Run SUT With Helper    scat    @{tests}

    Verify Status Line    ${result}    total=51    pass=0    fail=51

Tests are executed in command line order
    @{tests_order} =    Create List    scat-failing_scenario_when_will_expect_text_on_empty_output.omtt    scat-return_input_without_checking_output.omtt    scat-failing_scenario-output_is_shorten_than_expected_output.omtt
    ${result} =    Run SUT With Helper    scat    @{tests_order}

    Tests Were Executed In Order    ${result}      ${tests_order}

Tests are enumerated in ascending order
    ${result} =    Run SUT With Helper    scat    scat-failing_scenario_when_will_expect_text_on_empty_output.omtt    scat-return_input_without_checking_output.omtt    scat-failing_scenario-output_is_shorten_than_expected_output.omtt

    Test Was Executed With Specified Number    ${result}    number=1    of=3    test_file_name=scat-failing_scenario_when_will_expect_text_on_empty_output.omtt
    Test Was Executed With Specified Number    ${result}    number=2    of=3    test_file_name=scat-return_input_without_checking_output.omtt
    Test Was Executed With Specified Number    ${result}    number=3    of=3    test_file_name=scat-failing_scenario-output_is_shorten_than_expected_output.omtt


*** Keywords ***
Create Same Tests Names List
    [Arguments]    ${number}    ${test_name}
    @{tests} =   Create List
    FOR    ${i}    IN RANGE    ${number}
        Append To List    ${tests}    ${test_name}
    END
    [Return]    @{tests}
