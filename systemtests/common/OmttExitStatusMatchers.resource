*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Keywords ***
Exit Status Points To All Tests Passed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    0

Exit Status Points To One Test Failed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    1

Exit Status Points To Two Tests Failed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    2

Exit Status Points To Maximum Tests Failed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    50

Exit Status Points To Information Printed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    61

Exit Status Points To Invalid Command Line Options
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    64

Exit Status Points To Fatal Error
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    60
