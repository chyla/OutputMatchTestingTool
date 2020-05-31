SUBDIRS = apps
EXTRA_DIST = $(top_srcdir)/systemtests/*.robot \
             $(top_srcdir)/systemtests/*.py \
             $(top_srcdir)/systemtests/common/*.py \
             $(top_srcdir)/systemtests/common/*.resource \
             $(top_srcdir)/systemtests/omtt/*
SYSTEMTESTS_LOGS_DIR = output
SYSTEMTESTS_DIR = $(top_srcdir)/systemtests

clean-local:
	rm -rf $(SYSTEMTESTS_LOGS_DIR)

check:
	@echo "PWD: " `pwd`
	@echo "Checking systemtests UT's..."
	python3 -m unittest discover -v $(SYSTEMTESTS_DIR)

	@echo "Running systemtests..."
	robot -d $(SYSTEMTESTS_LOGS_DIR) --variable SUT_PATH:$(top_builddir)/src/omtt --variable OMTT_TESTS_DIR:$(top_srcdir)/systemtests/omtt -L DEBUG $(SYSTEMTESTS_DIR)

.PHONY: check
