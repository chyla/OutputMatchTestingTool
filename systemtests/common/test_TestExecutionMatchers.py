# Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
# All rights reserved.
#
# Distributed under the terms of the BSD 3-Clause License.

from . import TestExecutionMatchers

import unittest
import unittest.mock


first_test_name = "cat-will-exit-with-zero.omtt"
second_test_name = "cat-will_match_part_of_output.omtt"
PASS = "PASS"
FAIL = "FAIL"


def create_result(first_test_result, second_test_result):
    result = unittest.mock.Mock()
    result.stdout = f"""
Testing: /bin/cat
====================
Running test (1/2): examples/{first_test_name}
Verdict: {first_test_result}
====================
Running test (2/2): examples/{second_test_name}
Verdict: {second_test_result}
"""
    return result


class TestsWereExecutedInOrderTestSuite(unittest.TestCase):

    def test_does_nothing_when_order_match(self):
        result = create_result(PASS, PASS)

        TestExecutionMatchers.tests_were_executed_in_order(result,
                                                           [first_test_name, second_test_name])

    def test_raises_exception_when_order_is_wrong_on_first_position(self):
        result = create_result(PASS, PASS)

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.tests_were_executed_in_order(result,
                                                               [first_test_name, first_test_name])

        self.assertEqual(f"Wrong order, expected: '{first_test_name}'; got line with: 'Running test (2/2): examples/{second_test_name}'", str(cm.exception))

    def test_raises_exception_when_order_is_wrong_on_later_position(self):
        result = create_result(PASS, PASS)

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.tests_were_executed_in_order(result,
                                                               [second_test_name, first_test_name])

        self.assertEqual(f"Wrong order, expected: '{second_test_name}'; got line with: 'Running test (1/2): examples/{first_test_name}'", str(cm.exception))

    def test_raises_exception_when_expected_order_list_is_longer_than_output(self):
        result = create_result(PASS, PASS)

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.tests_were_executed_in_order(result,
                                                               [first_test_name, second_test_name, second_test_name])

        self.assertIn("Can't check order, unequal lengths.", str(cm.exception))

    def test_raises_exception_when_expected_order_list_is_shorter_than_output(self):
        result = create_result(PASS, PASS)

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.tests_were_executed_in_order(result,
                                                               [first_test_name])

        self.assertIn("Can't check order, unequal lengths.", str(cm.exception))


class TestsWasExecutedWithSpecifiedNumber(unittest.TestCase):

    def test_does_nothing_when_number_match(self):
        result = create_result(PASS, PASS)

        TestExecutionMatchers.test_was_executed_with_specified_number(result, number=2, of=2, test_file_name=second_test_name)

    def test_raises_exception_when_number_is_wrong(self):
        result = create_result(PASS, PASS)

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.test_was_executed_with_specified_number(result, number=1, of=2, test_file_name=second_test_name)

        self.assertIn(f"Wrong (or missing) number (1/2) in line: 'Running test (2/2): examples/{second_test_name}'", str(cm.exception))

    def test_raises_exception_when_line_with_test_not_found(self):
        result = create_result(PASS, PASS)
        not_existing_test = "not_existing_test.omtt"

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.test_was_executed_with_specified_number(result, number=1, of=2, test_file_name=not_existing_test)

        self.assertEqual(f"Line with test not found: {not_existing_test}", str(cm.exception))


class TestWasExecutedWithPassTestSuite(unittest.TestCase):

    def test_assert_exception_is_not_thrown_when_test_pass(self):
        result = create_result(PASS, PASS)

        TestExecutionMatchers.test_was_executed_with_pass(result,
                                                          first_test_name)

    def test_assert_exception_is_thrown_when_first_test_fail(self):
        result = create_result(FAIL, PASS)

        with self.assertRaises(AssertionError):
            TestExecutionMatchers.test_was_executed_with_pass(result,
                                                              first_test_name)

    def test_assert_exception_is_thrown_when_second_test_fail(self):
        result = create_result(PASS, FAIL)

        with self.assertRaises(AssertionError):
            TestExecutionMatchers.test_was_executed_with_pass(result,
                                                              second_test_name)

    def test_exception_is_thrown_when_test_name_is_not_found_in_output(self):
        result = create_result(PASS, PASS)
        non_existing_test_name = "some_test.omtt"

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.test_was_executed_with_pass(result,
                                                              non_existing_test_name)

        self.assertEqual(f"Test {non_existing_test_name} not found.", str(cm.exception))

    def test_exception_is_thrown_when_test_verdict_is_missing(self):
        result = unittest.mock.Mock()
        result.stdout = f"""
Testing: /bin/cat
====================
Running test (1/2): examples/{first_test_name}
====================
Running test (2/2): examples/{second_test_name}
Verdict: PASS
"""

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.test_was_executed_with_pass(result,
                                                              first_test_name)

        self.assertEqual(f"Test verdict (for {first_test_name}) not found.", str(cm.exception))

    def test_exception_is_thrown_when_test_verdict_is_missing_due_to_cutted_output(self):
        result = unittest.mock.Mock()
        result.stdout = f"""
Testing: /bin/cat
====================
Running test (1/2): examples/{first_test_name}
Verdict: PASS
====================
Running test (2/2): examples/{second_test_name}
"""

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.test_was_executed_with_pass(result,
                                                              second_test_name)

        self.assertEqual(f"Test verdict (for {second_test_name}) not found.", str(cm.exception))


class TestWasExecutedWithFailTestSuite(unittest.TestCase):

    def test_assert_exception_is_not_thrown_when_test_fail(self):
        result = create_result(FAIL, FAIL)

        TestExecutionMatchers.test_was_executed_with_fail(result,
                                                          first_test_name)

    def test_assert_exception_is_thrown_when_first_test_pass(self):
        result = create_result(PASS, FAIL)

        with self.assertRaises(AssertionError):
            TestExecutionMatchers.test_was_executed_with_fail(result,
                                                              first_test_name)

    def test_assert_exception_is_thrown_when_second_test_pass(self):
        result = create_result(FAIL, PASS)

        with self.assertRaises(AssertionError):
            TestExecutionMatchers.test_was_executed_with_fail(result,
                                                              second_test_name)

    def test_exception_is_thrown_when_test_name_is_not_found_in_output(self):
        result = create_result(FAIL, FAIL)
        non_existing_test_name = "some_test.omtt"

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.test_was_executed_with_fail(result,
                                                              non_existing_test_name)

        self.assertEqual(f"Test {non_existing_test_name} not found.", str(cm.exception))

    def test_exception_is_thrown_when_test_verdict_is_missing(self):
        result = unittest.mock.Mock()
        result.stdout = f"""
Testing: /bin/cat
====================
Running test (1/2): examples/{first_test_name}
====================
Running test (2/2): examples/{second_test_name}
Verdict: FAIL
"""

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.test_was_executed_with_fail(result,
                                                              first_test_name)

        self.assertEqual(f"Test verdict (for {first_test_name}) not found.", str(cm.exception))

    def test_exception_is_thrown_when_test_verdict_is_missing_due_to_cutted_output(self):
        result = unittest.mock.Mock()
        result.stdout = f"""
Testing: /bin/cat
====================
Running test (1/2): examples/{first_test_name}
Verdict: FAIL
====================
Running test (2/2): examples/{second_test_name}
"""

        with self.assertRaises(Exception) as cm:
            TestExecutionMatchers.test_was_executed_with_fail(result,
                                                              second_test_name)

        self.assertEqual(f"Test verdict (for {second_test_name}) not found.", str(cm.exception))
