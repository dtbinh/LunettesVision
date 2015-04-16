#ifdef UNIT_TEST_GOOGLE

#define VERBOSE_MODE
#ifndef DEBUG
	#define DEBUG
#endif // !DEBUG

#include "Opengltest.hpp"	///< use to test Display class functionnalities
//~ #include "Shadertest.hpp"	///< use to test Shader class functionnalities


int main( int argc, char** argv )
{
	COLOR_PRINT_ON( COLOR_GREEN );
	std::cout << "<<<  Running tests  >>>" << std::endl;
	COLOR_PRINT_OFF();
	
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif
