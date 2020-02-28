def has_only_two_output_doesnt_match_messages(result):
    assert 2 == len([x for x in result.stdout.splitlines() if "Output doesn't match." in x])
