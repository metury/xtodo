/// @file main.cpp is the main entrypoint.
#include <iostream>
#include <string>
#include <vector>
#include "../core/reader.hpp"
#include "terminal.hpp"
#include "../core/exception.hpp"

/// Print all possible arguments when calling program.
void print_help_args(){
	std::cout << "xtodo [files] [optional arguments]" << std::endl;
	std::cout << "  -s" << std::endl;
	std::cout << "    To save current settings to config file" << std::endl;
	std::cout << "  -o" << std::endl;
	std::cout << "    Next file (argument without - beforehand) will be as output file" << std::endl;
	std::cout << "  --noconf" << std::endl;
	std::cout << "    Not to read config file" << std::endl;
	std::cout << "  -h | --help" << std::endl;
	std::cout << "    To see this help" << std::endl;
}

/// Main function of the application.
/// @param argc Argument count.
/// @param argv List of arguments.
/// @return Application exit code.
int main(int argc, char * * argv){
	try{
		// Initialization of program with its settings.
		std::vector<std::string> args (argv + 1, argc+argv);
		file_parser parser;
		bool run = parser.parse_arguments(args);
		if(!run){
			print_help_args();
			return 0;
		}
		try{
			parser.read_config();
		}
		catch(Exception& e){
			std::cout << e.what() << std::endl;
		}

		// Load tasks.
		tasks tasks;
		parser.read_files(tasks);
		terminal_run(tasks, parser);
		return 0;
	}
	catch(Exception& e){
		std::cout << e.what() << std::endl;
		std::cout << "Run with flag --help to see possible arguments." << std::endl;
		return 1;
	}
}
