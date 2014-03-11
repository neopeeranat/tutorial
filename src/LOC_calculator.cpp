#include "LOC_calculator.hpp"


LOC_calculator::LOC_calculator()
{
}
LOC_calculator::LOC_calculator(string filename)
{
	setFile(filename);
}
string LOC_calculator::trim(string str){
	// trim trailing spaces
	size_t endpos = str.find_last_not_of(" \t");
	if (string::npos != endpos)
	{
		str = str.substr(0, endpos + 1);
	}
	// trim leading spaces
	size_t startpos = str.find_first_not_of(" \t");
	if (string::npos != startpos)
	{
		str = str.substr(startpos);
	}
	return str;
}

//make it easy to use
bool LOC_calculator::isBlocksyntax(string line){
	return (line == "{" || line == "};" || line == "}");
}

/*
read file from filepath and return data inside file as string
*/
string LOC_calculator::read_file(string filepath)
{
	long lSize;
	FILE * pFile;
	char * buffer;
	size_t result;
	string text;

	try
	{
		pFile = fopen(filepath.c_str(), "rb");


		//check if not found file in filepath
		if (pFile == NULL)
		{
			throw runtime_error("Not found file : " + filepath);
		}
		//	"Not found file : " +filepath);

		// obtain file size:
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind(pFile);

		// allocate memory to contain the whole file:
		buffer = (char*)malloc(lSize);
		if (buffer == NULL) throw runtime_error("Memory error");

		// copy the file into the buffer:
		result = fread(buffer, 1, lSize, pFile);
		string mystring(buffer);
		if (result != lSize) throw runtime_error("Reading error");

		fclose(pFile);

		//fix size of string when coverted
		text = string(buffer);
		free(buffer);
		text = text.substr(0, lSize);
	}
	catch (exception e)
	{
		throw e;
	}
	return text;
}


/*
Calcuate entire program but not count empty line and comment
*/

void LOC_calculator::calculate_LOC_program()
{
	string string_data;
	bool count_LOC_structure = false;
	bool count_LOC_function = false;
	bool count_LOC_class = false;
	bool found_begin = false;
	int number_of_block = 0;

	if (LOC_calculator::filename == "") return;

	cout << "Start calculate LOC from " << LOC_calculator::filename << endl << endl;
	try
	{

		string_data = read_file(LOC_calculator::filename);
		if (string_data.empty()) throw runtime_error("Source file is empty");;

		for (int i = 0; i < string_data.length(); i += 2)
		{
			int line_lenght = string_data.find("\r\n", i) - i;

			// in case for last line of file
			if (line_lenght == (-1 - i))
				line_lenght = string_data.length() - i;

			string curent_line = trim(string_data.substr(i, line_lenght));
			if (curent_line == "class MyClass{"){
				curent_line += "";
			}
			//check if it's comment line
			if (curent_line.find("/*") != -1 && !(curent_line.find("curent_line.find(\"/*\")") != -1))
			{
				i = string_data.find("*/\r\n", i) + 2;
				continue;
			}
			if (curent_line.find("//") == 0)
			{
				i += line_lenght;
				continue;
			}
			if (curent_line.empty()){
				i += line_lenght;
				continue;
			}

			//ignore include
			if (curent_line.find("#include") != -1 && !(curent_line.find("curent_line.find(\"#include\")") != -1))
			{
				i += line_lenght;
				continue;
			}

			//ignore public: and private:
			if (curent_line == "public:" || curent_line == "private:")
			{
				i += line_lenght;
				continue;
			}

			if (regex_match(curent_line, regex("^using namespace.*")))
			{
				program.number_of_namespace++;
				program.total_LOC_of_namespace++;
			}
			if (curent_line.find("class ") != -1)
			{
				count_LOC_class = true;
				program.number_of_class++;
				//case { same line as struct
				if (curent_line.find("{") == -1)
				{
					program.total_LOC_of_class++;
				}
			}
			if (regex_match(curent_line, regex("^struct [A-Za-z0-9_]+[ \t\r\n\v\f]*[{]*$", std::regex_constants::extended)))
			{
				count_LOC_structure = true;
				program.number_of_structure++;
				//case { same line as struct
				if (curent_line.find("{") == -1)
				{
					program.total_LOC_of_structure++;
				}

			}
			//I'm not sure while this comment regex not work on g++ -std=c++11 while perfectly fine on VS2013 compiler. if you could give me suggestion on , please tell me.
			//if (regex_match(curent_line, regex("^[A-Za-z0-9_<>:]*[ \t\r\n\v\f]*[A-Za-z0-9_<>:]*[ \t\r\n\v\f]*[A-Za-z0-9_<>:]+[ \t\r\n\v\f]+[A-Za-z0-9_&]+[\(].*[)][ \t\r\n\v\f]*[\{]?$", std::regex_constants::extended)))
			if (regex_match(curent_line, regex("(public:|protected:|private:)* *.+ +.*\\(.*\\)")))
			{
				count_LOC_function = true;
				program.number_of_function++;
				//case { same line as struct
				if (curent_line.find("{") == -1){
					program.total_LOC_of_function++;
				}

			}

			if (count_LOC_structure || count_LOC_function || count_LOC_class){

				if (!found_begin)
				{
					if (curent_line.find("{") != -1){
						found_begin = true;
						number_of_block++;
					}
				}
				else
				{
					if (curent_line.find("{") != -1)
						number_of_block++;
					if (curent_line.find("}") != -1)
						number_of_block--;
				}

				if (number_of_block > 0){
					if (count_LOC_class && !isBlocksyntax(curent_line))
					{
						program.total_LOC_of_class++;
					}
					if (count_LOC_structure && !isBlocksyntax(curent_line)) program.total_LOC_of_structure++;
					if (count_LOC_function && !isBlocksyntax(curent_line)) program.total_LOC_of_function++;
				}
				else if (found_begin)
				{
					//end block
					if (count_LOC_class)
					{
						if (!isBlocksyntax(curent_line)) program.total_LOC_of_class++;
						count_LOC_class = false;
					}
					if (count_LOC_structure)
					{
						if (!isBlocksyntax(curent_line)) program.total_LOC_of_structure++;
						count_LOC_structure = false;
					}
					if (count_LOC_function)
					{
						if (!isBlocksyntax(curent_line)) program.total_LOC_of_function++;
						count_LOC_function = false;
					}

					//reset counter flag
					found_begin = false;
					number_of_block = 0;
				}

			}

			if (!isBlocksyntax(curent_line))
				program.LOC++;
			i += line_lenght;
		}
	}
	catch (exception e)
	{
		cerr << e.what() << endl;
	}
}

int LOC_calculator::getLOC()
{
	return program.LOC;
}
void LOC_calculator::setFile(string filename)
{
	path p(filename);
	try
	{

		if (exists(p))
		{
			if (is_regular_file(p))
			{
				string test = extension(p);
				if (extension(p) == ".cpp")
				{

				}
				else
				{
					throw runtime_error(filename + " is not a cpp file");
				}
			}

			else
			{
				throw runtime_error(filename + " is not a regular file");
			}

		}
		else
		{
			throw runtime_error(filename + " does not exist");
		}

		LOC_calculator::filename = filename;
	}
	catch (runtime_error e)
	{
		cerr << e.what() << endl;
	}
	catch (exception e)
	{
		cerr << "Unexpected error : " << e.what() << endl;
	}

	
}

void LOC_calculator::print_result()
{
	if (program.LOC > 0)
	{
		for (int i = 0; i < 1; i++)
		{
			//cout << "Program " << i + 1 << endl;
			if (program.number_of_namespace>0)
			{
				cout << "number of using namespace : " << program.number_of_namespace << endl;
				cout << "total line of using namespace : " << program.total_LOC_of_namespace << endl;
			}
			if (program.number_of_structure > 0)
			{
				cout << "number of structure : " << program.number_of_structure << endl;
				cout << "total line of structure : " << program.total_LOC_of_structure << endl;
			}
			cout << "number of class : " << program.number_of_class << endl;
			cout << "total line of class : " << program.total_LOC_of_class << endl;
			cout << "number of function : " << program.number_of_function << endl;
			cout << "total line of function : " << program.total_LOC_of_function << endl;
			cout << "Total size : " << program.LOC << endl;
			cout << endl;
		}
	}
}
