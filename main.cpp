#include <iostream>
#include <stdlib.h>
#include <fstream>

typedef unsigned char byte;

using namespace std;

bool is_valid_char(char ch) {
	return (ch == '>' || ch == '<' || ch == '+' || ch == '-' || ch == '.' || ch == ',' || ch == '[' || ch == ']');
}

bool check_program(byte *program, unsigned int program_size) {
	int l=0;
	for(int i=0; i<program_size; i++) {
		if(program[i] == '[') l++;
		if(program[i] == ']') l--;
	}
	return (l == 0);
}

int main(int argc, char** argv) {

	/*
	./bfc program.bf
	argv[0] = ./bfc
	argv[1] = program.bf
	argv[2] = input_file.txt
	*/

	const int program_array_count = 100000;
	const int byte_array_count = 30000;

	// program
	unsigned int instruction_pointer = 0;
	byte program[program_array_count];
	unsigned int program_size = 0;

	// byte array
	unsigned int data_pointer = 0;
	byte byte_array[byte_array_count];
	for(int i=0; i<byte_array_count; i++) byte_array[i] = 0;

	// arguments check
	if(!(argc == 2 || argc == 3)) {
		cout << "Arguments Error\n";
		exit(-1);
	}

	// program_file
	ifstream program_file(argv[1]);
	if(!program_file.is_open()) {
		cout << "File Error\n";
		exit(-1);
	}

	// input_file
	ifstream input_file;
	if(argc == 3) {
		input_file.open(argv[2]);
		if(!input_file.is_open()) {
			cout << "File Error\n";
			exit(-1);
		}
	}

	// read program_file
	while(!program_file.eof()) {
		char ch = program_file.get();
		if(is_valid_char(ch))
			program[program_size++] = ch;
	}

	// check program for syntax errors
	if(!check_program(program, program_size)) {
		cout << "Program Syntax Error\n";
		exit(-1);
	}

	// interpret
	while(instruction_pointer < program_size) {
		switch(program[instruction_pointer]) {
			case '>':
				data_pointer = (data_pointer == byte_array_count-1) ? 0 : data_pointer+1;
				break;
			case '<':
				data_pointer = (data_pointer == 0) ? byte_array_count-1 : data_pointer-1;
				break;
			case '+':
				byte_array[data_pointer]++;
				break;
			case '-':
				byte_array[data_pointer]--;
				break;
			case '.':
				cout << byte_array[data_pointer];
				break;
			case ',':
				if(argc == 3) {
					byte_array[data_pointer] = input_file.get();
					if (input_file.eof()) {
						cout << "No bytes to read\n";
						exit(-1);
					}
				}
				else {
					cout << "Arguments Error\n";
					exit(-1);
				}
				break;
			case '[':
				if(!byte_array[data_pointer]) {
					int l=1;
					do {
						instruction_pointer++;
						if(program[instruction_pointer] == '[') l++;
						if(program[instruction_pointer] == ']') l--;
					} while(l);
				}
				break;
			case ']':
				if(byte_array[data_pointer]) {
					int l=1;
					do {
						instruction_pointer--;
						if(program[instruction_pointer] == ']') l++;
						if(program[instruction_pointer] == '[') l--;
					} while(l);
				}
				break;
		}
		instruction_pointer++;
	}

	return 0;
}