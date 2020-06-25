SUBDIRS = src unittests systemtests
dist_doc_DATA = README.md LICENSE VERSIONING.md
EXTRA_DIST = headers examples

unittests:
	cd unittests && $(MAKE) $(AM_MAKEFLAGS) check

systemtests: all
	cd systemtests && $(MAKE) $(AM_MAKEFLAGS) check

.PHONY: unittests systemtests
