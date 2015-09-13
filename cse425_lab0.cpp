// cse425_lab0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cse425_lab0.h"
#include "Parser.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 3){
		cout << "usage: " << argv[programName] << " <input_file_name> <output_file_name>"
			<< endl;
		return mainError::Argc;
	}
	
	ifstream input(argv[input_file_name]);
	if (!input) return mainError::InfileNotFound;
	
	ofstream output(argv[output_file_name]);

	try{
		Parser p(input, output);
	}
	catch (exception& e){
		cout << "exceptino in Parse: " << e.what();
	}

	output.close();

	return mainError::None;
}

// Error handling:
// hornc -> head
// need to handle
// what about body -> predicate {AND predicate}
// do we need to recover of {} parse fails?