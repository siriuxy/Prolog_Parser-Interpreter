/* 
	cse425_lab2.h: contain some useful enums that are later used in main fucntion to handle file IO etc.

	Likai Yan
	Jianfeng Zeng
*/
#pragma once
enum argIndex{
	programName = 0,
	input_file_name = 1,
	output_file_name = 2,
	expectedArgNum = 3,
};

enum mainError{
	None,
	Argc,
	InfileNotFound,
	OutfileUnavailable,
};