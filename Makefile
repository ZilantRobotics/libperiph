
tests: clean
	$(info tests)
	./tests/unit_tests_with_coverage.sh

clean:
	rm -rf build/