
all: bloodbank_tester test_creator run

clean:
	rm -f *.o bloodbank_tester testcreator .runresults

run: bloodbank_tester blood_001.txt blood_002.txt blood_003.txt hundredthousand none five small
	rm -rf .runresults
	@echo 'none' >> .runresults
	./bloodbank_tester none >> .runresults
	@echo 'five' >> .runresults
	./bloodbank_tester five >> .runresults
	@echo 'small' >> .runresults
	./bloodbank_tester small >> .runresults
	@echo 'blood_001.txt' >> .runresults
	./bloodbank_tester blood_001.txt >> .runresults
	@echo 'blood_002.txt' >> .runresults
	./bloodbank_tester blood_002.txt >> .runresults
	@echo 'blood_003.txt' >> .runresults
	./bloodbank_tester blood_003.txt >> .runresults
	@echo 'hundredthousand' >> .runresults
	./bloodbank_tester hundredthousand >> .runresults

	@git diff --word-diff --inter-hunk-context=10 --no-index --minimal --color ./baseline_results ./.runresults | cat

test_creator: testcreator.cpp
	g++ -O3 --std=c++11 testcreator.cpp -o testcreator

bloodbank_tester: bloodbank_tester.cpp bloodbank_api_test.cpp
	@clang++ -O0 -g -gdwarf-2 -Wall -Wextra --std=c++11 bloodbank_tester.cpp bloodbank_api_test.cpp -o /dev/stdout >/dev/null
	g++ -O0 -g -gdwarf-2 -Wall -Wextra --std=c++11 bloodbank_tester.cpp bloodbank_api_test.cpp -o bloodbank_tester
