*** Comments ***
Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.


*** Keywords ***
Verify Status Line
    [Arguments]    ${result}    ${total}    ${pass}    ${fail}
    Should Contain    ${result.stdout}    ${total} tests total, ${pass} passed, ${fail} failed
