#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <unistd.h>
#define USAGE "rpf [-1][-2][-3] [FILE]"

void PrintFile(const std::filesystem::path &path) {

	std::ifstream thisfile;
	thisfile.open(path);
	if(!thisfile.is_open()) {
		std::cout << "Cannot open file" << std::endl;
		return;
	}
	
	thisfile.seekg(0, std::ios::end); // seek to end
	auto size = thisfile.tellg(); // get size
	std::string data(size, '\0'); // make string of appropriate size
	thisfile.seekg(0, std::ios::beg); // seek to beginning of file
	thisfile.read(&data[0], size); // read entire file in to string
	std::cout << data;
	std::cout << "File size: " << size << std::endl;
	thisfile.close();
}

void PrintFileBUFFERED_SINGLE_CHAR(const std::filesystem::path &path) {

	std::ifstream thisfile;
	thisfile.open(path);
	if(!thisfile.is_open()) {
		std::cout << "Cannot open file" << std::endl;
		return;
	}

	thisfile.seekg(0, std::ios::end); // seek to end
	auto size = thisfile.tellg(); // get size
	thisfile.seekg(0, std::ios::beg); // seek to beginning of file
	while (!thisfile.eof()) {
		std::vector<char> vecbuf(1024);
		for(auto a = vecbuf.begin() ; a != vecbuf.end() ; ++a) {
			*a = thisfile.get();
		}
		
		for(const auto &a : vecbuf) {
			std::cout << a;
		}		
	}
	
	std::cout << "File size: " << size << std::endl;
	thisfile.close();
}

void PrintFileBUFFERED_READ(const std::filesystem::path &path) {

	std::ifstream thisfile;
	thisfile.open(path);
	if(!thisfile.is_open()) {
		std::cout << "Cannot open file" << std::endl;
		return;
	}

	thisfile.seekg(0, std::ios::end); // seek to end
	auto size = thisfile.tellg(); // get size
	thisfile.seekg(0, std::ios::beg); // seek to beginning of file
	while (!thisfile.eof()) {
		std::vector<char> vecbuf(1024);
		thisfile.read(vecbuf.data(), vecbuf.size());
		std::cout.write(vecbuf.data(), thisfile.gcount());
	}
	
	std::cout << "File size: " << size << std::endl;
	thisfile.close();
}

inline void usageOUT() {
	std::cerr << USAGE << std::endl;
}

int main (int argc, char **argv) {

	if(argc < 3 || argc > 3) {
		usageOUT();
		return EXIT_FAILURE;
	}

	int opt;
	int one = 0;
	int two = 0;
	int three = 0;
    while ((opt = getopt(argc, argv, "123")) != -1) {
        switch (opt) {
        case '1':

			one = 1;
			
			break;
        case '2':

			two = 1;
			
			break;
		case '3':

			three = 1;
			
			break;
		}
	}

	if((one + two + three) > 1) {
		usageOUT();
		return EXIT_FAILURE;
	}
	
	int index;
	std::filesystem::path path;
	for (index = optind ; index < argc ; index++) {
		path = argv[index];
	}

	if(one == 1) {
		PrintFile(path);
		std::cout << "PrintFile" << std::endl;
	}

	if(two == 1) {
		PrintFileBUFFERED_SINGLE_CHAR(path);
		std::cout << "PrintFileBSC" << std::endl;
	}
	
	if(three == 1) {
		PrintFileBUFFERED_READ(path);
		std::cout << "PrintFileBR" << std::endl;
	}
	
	return EXIT_SUCCESS;
}
