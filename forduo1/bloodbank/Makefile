
all: bloodbank_tester

clean:
	rm -f *.o bloodbank_tester

bloodbank_tester: bloodbank_tester.cpp bloodbank_api_test.cpp
	g++ -O0 -g -gdwarf-2 --std=c++98 bloodbank_tester.cpp bloodbank_api_test.cpp -o bloodbank_tester
