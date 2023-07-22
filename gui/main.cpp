/// @file main.cpp is the main entrypoint.
#include <iostream>
#include <string>
#include <vector>
#include "../core/reader.hpp"
#include "../core/task.hpp"
#include "../core/exception.hpp"

#include <QApplication>
#include "gui.hpp"


/// Main function of the application.
/// @param argc Argument count.
/// @param argv List of arguments.
/// @return Application exit code.
int main(int argc, char * * argv){
	try{
        tasks tasks;
        file_parser reader;
        reader.read_config();
        QApplication a(argc, argv);
        MainWindow w (&tasks, &reader);
        w.show();
        return a.exec();
	}
	catch(Exception& e){
		std::cout << e.what() << std::endl;
		std::cout << "Run with flag --help to see possible arguments." << std::endl;
		return 1;
	}
}
