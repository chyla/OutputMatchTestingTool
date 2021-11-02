# Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
# All rights reserved.
#
# Distributed under the terms of the BSD 3-Clause License.


def tests_were_executed_in_order(result, order):
    lines_with_tests_names = __extract_lines_with_tests_names(result)

    if len(lines_with_tests_names) != len(order):
        raise Exception(
            f"Can't check order, unequal lengths.\nExpected: {order}\nGot: {lines_with_tests_names}"
        )

    for line, test in zip(lines_with_tests_names, order):
        if test not in line:
            raise Exception(f"Wrong order, expected: '{test}'; got line with: '{line}'")


def test_was_executed_with_specified_number(result, number, of, test_file_name):
    lines_with_tests_names = __extract_lines_with_tests_names(result)

    for line in lines_with_tests_names:
        if test_file_name in line:
            if f"{number}/{of}" in line:
                return
            else:
                raise Exception(
                    f"Wrong (or missing) number ({number}/{of}) in line: '{line}'"
                )

    raise Exception(f"Line with test not found: {test_file_name}")


def test_was_executed_with_pass(result, test_file_name):
    __check_test_execution("PASS", result, test_file_name)


def test_was_executed_with_fail(result, test_file_name):
    __check_test_execution("FAIL", result, test_file_name)


def __extract_lines_with_tests_names(result):
    return [x for x in result.stdout.splitlines() if "Running test" in x]


def __check_test_execution(verdict, result, test_file_name):
    tests_names_with_verdicts = [
        x for x in result.stdout.splitlines() if "Running test" in x or "Verdict:" in x
    ]

    index_of_line_with_test = __index_of_test_in_lines(
        tests_names_with_verdicts, test_file_name
    )

    __check_test_vertdict_exists(
        tests_names_with_verdicts, index_of_line_with_test, test_file_name
    )

    assert (
        f"Verdict: {verdict}" in tests_names_with_verdicts[index_of_line_with_test + 1]
    )


def __index_of_test_in_lines(lines, test_name):
    for index, line in enumerate(lines):
        if test_name in line:
            return index

    raise Exception(f"Test {test_name} not found.")


def __check_test_vertdict_exists(
    tests_names_with_verdicts, line_with_test, test_file_name
):
    if (
        len(tests_names_with_verdicts) <= line_with_test + 1
        or "Verdict:" not in tests_names_with_verdicts[line_with_test + 1]
    ):
        raise Exception(f"Test verdict (for {test_file_name}) not found.")
