*** Comments ***
Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Settings ***
Resource    common/SutExecution.resource
Resource    common/VerdictMatchers.resource
Resource    common/OmttExitStatusMatchers.resource


*** Test Cases ***
Match CR LF (Windows style) test file with CR LF (Windows style) process output
    ${result} =    Run SUT With Helper    scat    scat-crlf_file-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match CR (old Apple style) test file with CR (old Apple style) process output
    ${result} =    Run SUT With Helper    scat    scat-cr_file-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match LF test file with CR LF (Windows style) process output
    ${result} =    Run SUT With Helper    stdout_crlf    stdout-lf_test_file-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match CR LF (Windows style) test file with LF process output
    ${result} =    Run SUT With Helper    stdout_lf    stdout-crlf_test_file-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match LF test file with CR (old Apple style) process output
    ${result} =    Run SUT With Helper    stdout_cr    stdout-lf_test_file-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match CR (old Apple style) test file with LF process output
    ${result} =    Run SUT With Helper    stdout_lf    stdout-cr_test_file-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match CR LF (Windows style) input with LF expected output
    ${result} =    Run SUT With Helper    scat    scat-crlf_input_lf_output-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match LF input with CR LF (Windows style) expected output
    ${result} =    Run SUT With Helper    scat    scat-lf_input_crlf_output-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match CR (old Apple style) input with LF expected output
    ${result} =    Run SUT With Helper    scat    scat-cr_input_lf_output-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}

Match LF input with CR (old Apple style) expected output
    ${result} =    Run SUT With Helper    scat    scat-lf_input_cr_output-match_exit_code_and_full_output.omtt

    Verdict Is Set To Pass    ${result}
    Exit Status Points To All Tests Passed    ${result}
