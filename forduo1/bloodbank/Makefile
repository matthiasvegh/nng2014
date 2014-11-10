
all: bloodbank_tester test_creator run

clean:
	rm -f *.o bloodbank_tester testcreator

run: bloodbank_tester blood_001.txt blood_002.txt blood_003.txt
	./bloodbank_tester blood_001.txt
	./bloodbank_tester blood_002.txt
	./bloodbank_tester blood_003.txt

test_creator: testcreator.cpp
	g++ -O3 --std=c++11 testcreator.cpp -o testcreator

bloodbank_tester: bloodbank_tester.cpp bloodbank_api_test.cpp
	g++ -O0 -g -gdwarf-2 --std=c++98 bloodbank_tester.cpp bloodbank_api_test.cpp -o bloodbank_tester
