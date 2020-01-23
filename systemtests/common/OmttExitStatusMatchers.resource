*** Comments ***
Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
All rights reserved.

Distributed under the terms of the BSD 3-Clause License.


*** Keywords ***
Exit Status Points To All Tests Passed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    0

Exit Status Points To One Test Failed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    1

Exit Status Points To Help Or Version Information Printed
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    251

Exit Status Points To Invalid Command Line Options
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    252

Exit Status Points To Fatal Error
    [Arguments]    ${result}
    Should Be Equal As Integers    ${result.rc}    255
