#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <regex>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;
struct Program
{
	long LOC = 0;
	int number_of_namespace = 0;
	int total_LOC_of_namespace = 0;
	int number_of_structure = 0;
	int total_LOC_of_structure = 0;
	int number_of_class = 0;
	int total_LOC_of_class = 0;
	int number_of_function = 0;
	int total_LOC_of_function = 0;
};
class LOC_calculator
{
private:
	Program program;
	string filename;
	

public:
	LOC_calculator();
	LOC_calculator(string filename);
public:
	void calculate_LOC_program();
	int getLOC();
	void setFile(string filename);
	void print_result();
private:
	string trim(string str);
	bool isBlocksyntax(string line);
	string read_file(string filepath);



};

