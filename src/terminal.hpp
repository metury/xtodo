/// @file terminal.hpp Headder file for running application in terminal.
#ifndef terminal_hpp_
#define terminal_hpp_

#include "task.hpp"
#include "reader.hpp"
#include "exception.hpp"
#include <iostream>

/// When edditing print one part of a dialog.
/// @param text What is the text to be shown on the beggining.
/// @param before What is the old text that is to be edited.
void printEditPart(const std::string& text, std::string& before);

/// When edditing print date part of a dialog.
/// @param text What is the text to be shown on the beggining.
/// @param date Which was the date beforehand.
void printEditDate(const std::string& text, const Date& date);

/// Main loop for running terminal application.
/// @param tasks What are the tasks.
/// @param reader Which reader is to be used. For saving files.
/// @param ofile Where will the tasks be saved in an output file.
void terminalRun(Tasks& tasks, Reader& reader, const std::string& ofile);

/// Edit or add task dialog.
/// @param task What is the task.
/// @param text What to write after.
void edit(Task& task, std::string& text);

/// print all possible calls in terminal app.
void printHelp();

#endif
