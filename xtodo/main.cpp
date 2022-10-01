/// @file main.cpp is the main entrypoint.
#include <iostream>
#include <string>
#include <vector>
#include "reader.hpp"
#include "task.hpp"
#include "terminal.hpp"
#include "exception.hpp"

#include <QApplication>
#include "qui.hpp"

/// Print all possible arguments when calling program.
void printHelpArgs(){
	std::cout << "List of arguments that can be used:" << std::endl;
	std::cout << "-cli .. to use terminal application" << std::endl;
	std::cout << "-gui .. to use graphical user interface" << std::endl;
	std::cout << "-s .. to save current settings to config file" << std::endl;
	std::cout << "-o .. next file (argument without - beforehand) will be as output file" << std::endl;
	std::cout << "-noconf .. not to read config file" << std::endl;
	std::cout << "-h [or] --help .. to see this help" << std::endl;
	std::cout << ".. or else it is considered to be a file (input or output)" << std::endl;
}

/// Main function of the application.
/// @param argc Argument count.
/// @param argv List of arguments.
/// @return Application exit code.
int main(int argc, char * * argv){
	try{
		// Initialization of program with its settings.
		std::vector<std::string> args (argv + 1, argc+argv); 
		ArgumentReader ar;
		Settings settings;
		std::vector<std::string> files;
		std::string ofile;
		bool run = ar.parseArguments(args, settings, files, ofile);
		if (!run){
			printHelpArgs();
			return 0;
		}
		ar.readConfig(settings, files, ofile);
		
		// Load tasks.
		Tasks tasks;
		Reader reader;
		reader.readFiles(files, tasks);
		
		// Run.
		if (settings.client == CLI){
			terminalRun(tasks, reader, ofile);
		}
		else{
			QApplication a(argc, argv);
            MainWindow w (&tasks, &reader, &ofile);
			w.show();
            w.refresh();
			return a.exec();
		}
		return 0;
	}
	catch(Exception& e){
		std::cout << e.what() << std::endl;
		std::cout << "Run with flag --help to see possible arguments." << std::endl;
		return 1;
	}
}
