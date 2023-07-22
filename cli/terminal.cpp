/// @file terminal.cpp Source file for running application in terminal.
#include "terminal.hpp"

void printEditPart(const std::string& text, std::string& before){
	std::cout << text << " ";
	if(before == ""){
		std::cout << "/";
	}
	else{
		std::cout << before;
	}
	std::cout << " -> ";
}

void printEditDate(const std::string& text, const date& date){
	std::cout << text << " ";
	if(date.is_empty()){
		std::cout << "/";
	}
	else{
		std::cout << date;
	}
	std::cout << " -> ";
}

void edit(task& task, std::string& text){
	std::string line;
	std::cout << "Priority: " << (task.get_priority() == -1 ? '/' : char('A'+task.get_priority())) << " -> ";
	getline(std::cin, line);
	if(line.length() == 1 && line[0] <= 'Z' && line[0] >= 'A'){
		task.set_priority(line[0]);
	}
	else if(line[0] == '/'){
		task.set_priority('0');
	}
	printEditDate("Completion date:", task.get_completion_date());
	getline(std::cin, line);
	if(line[0] == '/'){
		task.set_completion_date(date());
	}
	else if(line.length() >= 1){
		task.set_completion_date(line);
	}
	do {
		printEditDate("Creation date:", task.get_creation_date());
		getline(std::cin, line);
		if(line[0] == '/'){
			task.set_creation_date(date());
		}
		else if(line.length() >= 1){
			task.set_creation_date(line);
		}
		if(!task.get_completion_date().is_empty() && task.get_creation_date().is_empty()){
			std::cout << "If completion date is set, creation date need to be specified as well." << std::endl;
		}
	} while(!task.get_completion_date().is_empty() && task.get_creation_date().is_empty());
	do {
		printEditPart("Text:", task.set_text());
		getline(std::cin, line);
		if(line.length() >= 1){
			task.set_text() = line;
			break;
		}
		if(task.set_text() == ""){
			std::cout << "Text cannot be empty!" << std::endl;
		}
	} while(task.set_text() == "");
	printEditPart("Context tag:", task.set_context());
	getline(std::cin, line);
	if(line[0] == '/'){
		task.set_context() = "";
	}
	else if(line.length() >= 1){
		task.set_context() = line;
	}
	printEditPart("Project tag:", task.set_project());
	getline(std::cin, line);
	if(line[0] == '/'){
		task.set_project() = "";
	}
	else if(line.length() >= 1){
		task.set_project() = line;
	}

	std::cout << text << task << std::endl;
}

void print_help(){
	std::cout << "What can be called in terminal application:" << std::endl;
	std::cout << "show .. show tasks taht are not done and not marked for deletion" << std::endl;
	std::cout << "showall .. show all tasks" << std::endl;
	std::cout << "save .. save current tasks to output file" << std::endl;
	std::cout << "bye [or] exit .. to save and exit application" << std::endl;
	std::cout << "quit .. to force exit witout saving the file" << std::endl;
	std::cout << "undo .. undo last change" << std::endl;
	std::cout << "redo .. redo last undo" << std::endl;
	std::cout << "sort .. sort all tasks" << std::endl;
	std::cout << "add task-in-text-format .. to add new task from the text behind" << std::endl;
	std::cout << "edit x .. to edit task on x position, x needs to be a number" << std::endl;
	std::cout << "del x y z [...] .. mark task on x y z positions for deletion" << std::endl;
	std::cout << "done x y z [...] .. mark/unmark task on x y z positions as complete" << std::endl;
	std::cout << "find string .. to look for tasks having this string" << std::endl;
	std::cout << "	- also can be used as @string or +string, to look only for context or project" << std::endl;
	std::cout << std::endl;
	std::cout << "When editing or adding new task:" << std::endl;
	std::cout << "This will prompt a new dialogue and everytime if you press only enter it will not change, or if you paste / it will be read as replace for nothing." << std::endl;
}

void terminal_run(tasks& tasks, file_parser& reader){
	std::string line;
	std::cout << prefix;
	while (getline(std::cin, line)){
		std::vector<std::string> parts(0);
		split_string(line, ' ', parts);
		if(parts.size() == 0){
			std::cout << prefix;
			continue;
		}
		if(line == "show"){
			tasks.print_tasks();
		}
		else if(line == "showall"){
			tasks.print_all_Tasks();
		}
		else if(line == "save"){
			reader.save_file(tasks);
		}
		else if(line == "bye" || line == "exit"){
			reader.save_file(tasks);
			return;
		}
		else if (line == "quit"){
			return;
		}
		else if (line == "undo"){
			tasks.undo(true);
		}
		else if (line == "redo"){
			tasks.redo(true);
		}
		else if (line == "sort"){
			tasks.sort();
		}
		else if (line == "help"){
			print_help();
		}
		else if (parts[0] == "add"){
			if(parts.size() > 1){
				std::string writtentask;
				bind_strings(parts,1,parts.size(),writtentask);
				tasks.add_task(writtentask);
			}
			else{
				std::string text = "Added task: ";
				edit(tasks.add_empty(), text);
			}
		}
		else if (parts.size() > 1 && parts[0] == "edit"){
			if(isdigit(parts[1][0])){
				size_t position;
				try{
					try{
						position = stoi(parts[1]);
					} catch(std::out_of_range& e){
						std::cout << "This is way too large number" << std::endl;
						std::cout << prefix;
						continue;
					}
					std::string text = "Chnaged to: ";
					edit(tasks.at(position),text);
				} catch(Exception& e){
					std::cout << e.what() << std::endl;
				}
			}
		}
		else if (parts.size() > 1 && parts[0] == "del"){
			for (size_t i = 1; i < parts.size(); ++i){
				if(isdigit(parts[i][0])){
					size_t position;
					try{
						try{
							position = stoi(parts[i]);
						} catch(std::out_of_range& e){
							std::cout << "This is way too large number" << std::endl;
							if (i == parts.size() - 1){ std::cout << prefix;}
							continue;
						}
						tasks.at(position).switch_deletion();
					} catch(Exception& e){
						std::cout << e.what() << std::endl;
					}
				}
			}
		}
		else if(parts.size() > 1 && parts[0] == "done"){
			for (size_t i = 1; i < parts.size(); ++i){
				if(isdigit(parts[i][0])){
					size_t position;
					try{
						try{
							position = stoi(parts[i]);
						} catch(std::out_of_range& e){
							std::cout << "This is way too large number" << std::endl;
							if (i == parts.size() - 1){ std::cout << prefix;}
							continue;
						}
						tasks.at(position).switch_completion();
					} catch(Exception& e){
						std::cout << e.what() << std::endl;
					}
				}
			}
		}
		else if(parts.size() == 2 && parts[0] == "find"){
			size_t index = 0;
			for (auto&& task : tasks){
				if (task.match(parts[1])){
					if (index < 10){
						std::cout << "0";
					}
					std::cout << index << ": " << task << std::endl;
					++index;
				}
			}
		}
		else if(parts.size() == 1 && parts[0] == "reload"){
			reader.save_file(tasks);
			tasks.clear();
			reader.read_files(tasks);
		}
		std::cout << prefix;
	}
	std::cout << "Program terminated with EOF." << std::endl;
}
