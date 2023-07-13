tests: clean
	$(info tests)
	./tests/unit_tests.sh

tests_with_coverage: clean
	$(info tests_with_coverage)
	./tests/unit_tests.sh --coverage

clean:
	rm -rf build/
