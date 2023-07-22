/// @file terminal.hpp Headder file for running application in terminal.
#ifndef terminal_hpp_
#define terminal_hpp_

#include "../core/task.hpp"
#include "../core/reader.hpp"
#include "../core/exception.hpp"
#include <iostream>

const std::string prefix = "xtodo: ";

/// When edditing print one part of a dialog.
/// @param text What is the text to be shown on the beggining.
/// @param before What is the old text that is to be edited.
void printEditPart(const std::string& text, std::string& before);

/// When edditing print date part of a dialog.
/// @param text What is the text to be shown on the beggining.
/// @param date Which was the date beforehand.
void printEditDate(const std::string& text, const date& date);

/// Main loop for running terminal application.
/// @param tasks What are the tasks.
/// @param reader Which reader is to be used. For saving files.
void terminal_run(tasks& tasks, file_parser& parser);

/// Edit or add task dialog.
/// @param task What is the task.
/// @param text What to write after.
void edit(task& task, std::string& text);

/// print all possible calls in terminal app.
void print_help();

#endif
