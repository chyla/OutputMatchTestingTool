SUBDIRS = apps

check:
	@echo "Checking systemtests UT's..."
	cd .. && python3 -m unittest discover -v systemtests/

	@echo "Running systemtests..."
	cd .. && robot -d systemtests/output -L DEBUG systemtests/

.PHONY: check
