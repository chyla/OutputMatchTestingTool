SUBDIRS = apps
SYSTEMTESTS_SRC_DIR = $(top_srcdir)/systemtests
SYSTEMTESTS_BUILD_DIR = $(top_builddir)/systemtests
EXTRA_DIST = $(SYSTEMTESTS_SRC_DIR)/*.robot \
             $(SYSTEMTESTS_SRC_DIR)/*.py \
             $(SYSTEMTESTS_SRC_DIR)/common/*.py \
             $(SYSTEMTESTS_SRC_DIR)/common/*.resource \
             $(SYSTEMTESTS_SRC_DIR)/omtt/*
SYSTEMTESTS_LOGS_DIR = output

clean-local:
	rm -rf $(SYSTEMTESTS_LOGS_DIR)

check:
	@if [ "$(PYTHON)" = ":" ] ; then                                                      \
                echo "Python not found, not able to execute system tests." ;                  \
                exit 1 ;                                                                      \
        else                                                                                  \
                if [ "x$(HAS_ROBOT)" = "xno" ] ; then                                         \
                        echo "Robot Framework not found, not able to execute system tests." ; \
                        exit 1 ;                                                              \
                else                                                                          \
                        echo "Checking systemtests UT's..." ;                                 \
                        $(PYTHON) -m unittest discover -v $(SYSTEMTESTS_SRC_DIR) ;            \
                                                                                              \
                        echo "Running systemtests..." ;                                       \
                        $(ROBOT) -d $(SYSTEMTESTS_LOGS_DIR)                                   \
                                 --variable SUT_PATH:$(top_builddir)/src/omtt                 \
                                 --variable OMTT_TESTS_DIR:$(SYSTEMTESTS_SRC_DIR)/omtt        \
                                 --variable HELPER_APPS_DIR:$(SYSTEMTESTS_BUILD_DIR)/apps     \
                                 -L DEBUG                                                     \
                                 $(SYSTEMTESTS_SRC_DIR) ;                                     \
                fi                                                                            \
        fi

.PHONY: check
