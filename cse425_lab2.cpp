/* cse425_lab2.cpp : This is the entry point of lab 2 program. It accommodates 
	file IO and its exception handling.

	Likai Yan
	Jianfeng Zeng
*/

#include "stdafx.h"
#include "cse425_lab2.h"
#include "Parser.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != expectedArgNum){
		cout << "usage: " << argv[programName] << " <input_file_name> <output_file_name>"
			<< endl;
		return mainError::Argc;
	}
	
	ifstream input(argv[input_file_name]);
	if (!input) return mainError::InfileNotFound;
	
	ofstream output(argv[output_file_name]);
	if (!output) return mainError::OutfileUnavailable;

	try{
		Parser p(input, output);
	}
	catch (exception& e){
		cout << "exceptino in Parse: " << e.what();
	}
		
	output.close();

	return mainError::None;
}

