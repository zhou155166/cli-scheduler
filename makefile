test: main.cpp project.cpp
	g++ -o test main.cpp project.cpp -lssl -lcrypto
