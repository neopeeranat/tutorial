#include "../src/LOC_calculator.hpp"
#include <boost/test/minimal.hpp>

int test_main(int argc, char* argv[])     
{
	string input;
	LOC_calculator LOC_calculator;
	if (argc == 1)
	{
		cout << "Please enter file of program location : ";
		cin >> input;
		LOC_calculator.setFile(input);
	}
	else LOC_calculator.setFile(argv[1]);
	LOC_calculator.calculate_LOC_program();
	LOC_calculator.print_result();

	BOOST_CHECK(LOC_calculator.getLOC() == 7);
	
	BOOST_REQUIRE(LOC_calculator.getLOC() == 7);

	if (LOC_calculator.getLOC() != 7)
		BOOST_ERROR("Ouch...");         

	if (LOC_calculator.getLOC() != 7)
		BOOST_FAIL("Ouch...");        

	if (LOC_calculator.getLOC() != 7) throw "Oops...";

	return LOC_calculator.getLOC() == 7 ? 0 : 1;       
}
