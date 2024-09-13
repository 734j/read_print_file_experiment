#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#define USAGE "rpf [-1][-2][-3][-4][-5][-6] [FILE]"
#define VEC_BUF_SIZE 1048576 // 1 MiB

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
	std::cout.write(&data[0], size);
	std::cout << "File size: " << size << std::endl;
	thisfile.close();
}

void PrintFileBUFFERED_SINGLE_CHAR(const std::filesystem::path &path) { // extremely slow

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
		std::vector<char> vecbuf(VEC_BUF_SIZE);
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
		std::vector<char> vecbuf(VEC_BUF_SIZE);
		thisfile.read(vecbuf.data(), vecbuf.size());
		std::cout.write(vecbuf.data(), thisfile.gcount());
	}

	std::cout << "File size: " << size << std::endl;
	thisfile.close();
}

void PrintFileBUFFERED_READ_ARRAY(const std::filesystem::path &path) {

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
		char arrbuf[VEC_BUF_SIZE];
		thisfile.read(arrbuf, VEC_BUF_SIZE);
		std::cout.write(arrbuf, thisfile.gcount());
	}

	std::cout << "File size: " << size << std::endl;
	thisfile.close();
}

void PrintFileBUFFERED_READ_STRING(const std::filesystem::path &path) {

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
		std::string strbuf(VEC_BUF_SIZE, '\0');
		thisfile.read(&strbuf[0], VEC_BUF_SIZE);
		std::cout.write(&strbuf[0], thisfile.gcount());
	}

	std::cout << "File size: " << size << std::endl;
	thisfile.close();
}

void ncat(const std::filesystem::path &path) {

    FILE *file;
	const char *filename = path.c_str();
    file = fopen(filename,"r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file.\n");
        return;
    }

	fseek(file, 0, SEEK_END);
	auto size = ftell(file);
	fseek(file, 0, SEEK_SET);
    char buffer[VEC_BUF_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, VEC_BUF_SIZE, file)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }

	std::cout << "File size: " << size << std::endl;
    fclose(file);
}

void ncat_write(std::filesystem::path &path) {

    int file;
	const char *filename = path.c_str();
    file = open(filename, O_RDONLY);
    if (file == -1) {
        fprintf(stderr, "Cannot open file.\n");
        return;
    }

	auto size = lseek(file, 0, SEEK_END);
	lseek(file, 0, SEEK_SET);
    char buffer[VEC_BUF_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(file, buffer, VEC_BUF_SIZE)) > 0) {
        write(0, buffer, bytes_read);
    }

	std::cout << "File size: " << size << std::endl;
    close(file);
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
	int four = 0;
	int five = 0;
	int six = 0;
	int seven = 0;
    while ((opt = getopt(argc, argv, "1234567")) != -1) {
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
		case '4':

			four = 1;

			break;
		case '5':

			five = 1;

			break;
		case '6':

			six = 1;

			break;
		case '7':

			seven = 1;

			break;
		}
	}

	if((one + two + three + four + five + six + seven) > 1) {
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

	if(four == 1) {
		PrintFileBUFFERED_READ_ARRAY(path);
		std::cout << "PrintFileBRA" << std::endl;
	}

	if(five == 1) {
		ncat(path);
		std::cout << "NCAT" << std::endl;
	}

	if(six == 1) {
		PrintFileBUFFERED_READ_STRING(path);
		std::cout << "PrintFileBRS" << std::endl;
	}

	if(seven == 1) {
		ncat_write(path);
		std::cout << "NCAT_WRITE" << std::endl;
	}

	return EXIT_SUCCESS;
}
