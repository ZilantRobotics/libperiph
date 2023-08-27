unit_tests: clean
	$(info tests)
	./tests/unit_tests.sh

coverage: clean
	$(info coverage)
	./tests/unit_tests.sh --coverage

clean:
	rm -rf build/
