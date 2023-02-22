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

void printEditDate(const std::string& text, const Date& date){
	std::cout << text << " ";
	if(date.isEmpty()){
		std::cout << "/";
	}
	else{
		std::cout << date;
	}
	std::cout << " -> ";
}

void edit(Task& task, std::string& text){
	std::string line;
	std::cout << "Priority: " << (task.getPriority() == -1 ? '/' : char('A'+task.getPriority())) << " -> ";
	getline(std::cin, line);
	if(line.length() == 1 && line[0] <= 'Z' && line[0] >= 'A'){
		task.setPriority(line[0]);
	}
	else if(line[0] == '/'){
		task.setPriority('0');
	}
	printEditDate("Completion date:", task.getCompletionDate());
	getline(std::cin, line);
	if(line[0] == '/'){
		task.setCompletionDate(Date());
	}
	else if(line.length() >= 1){
		task.setCompletionDate(line);
	}
	do {
		printEditDate("Creation date:", task.getCreationDate());
		getline(std::cin, line);
		if(line[0] == '/'){
			task.setCreationDate(Date());
		}
		else if(line.length() >= 1){
			task.setCreationDate(line);
		}
		if(!task.getCompletionDate().isEmpty() && task.getCreationDate().isEmpty()){
			std::cout << "If completion date is set, creation date need to be specified as well." << std::endl;
		}
	} while(!task.getCompletionDate().isEmpty() && task.getCreationDate().isEmpty());
	do {
		printEditPart("Text:", task.text());
		getline(std::cin, line);
		if(line.length() >= 1){
			task.text() = line;
			break;
		}
		if(task.text() == ""){
			std::cout << "Text cannot be empty!" << std::endl;
		}
	} while(task.text() == "");
	printEditPart("Context tag:", task.context());
	getline(std::cin, line);
	if(line[0] == '/'){
		task.context() = "";
	}
	else if(line.length() >= 1){
		task.context() = line;
	}
	printEditPart("Project tag:", task.project());
	getline(std::cin, line);
	if(line[0] == '/'){
		task.project() = "";
	}
	else if(line.length() >= 1){
		task.project() = line;
	} 
	
	std::cout << text << task << std::endl;
}

void printHelp(){
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

void terminalRun(Tasks& tasks, Reader& reader, const std::string& ofile){
	std::string line;
	std::cout << "x++: ";
	while (getline(std::cin, line)){
		std::vector<std::string> parts(0);
		splitString(line, ' ', parts);
		if(parts.size() == 0){
			std::cout << "x++: ";
			continue;
		}
		if(line == "show"){
			tasks.printTasks();
		}
		else if(line == "showall"){
			tasks.printAllTasks();
		}
		else if(line == "save"){
			reader.saveFile(tasks,ofile);
		}
		else if(line == "bye" || line == "exit"){
			reader.saveFile(tasks, ofile);
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
			printHelp();
		}
		else if (parts[0] == "add"){
			if(parts.size() > 1){
				std::string writtentask;
				bindStrings(parts,1,parts.size(),writtentask);
				tasks.addTask(writtentask);
			}
			else{
				std::string text = "Added task: ";
				edit(tasks.addEmpty(), text);
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
						std::cout << "x++: ";
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
							if (i == parts.size() - 1){ std::cout << "x++: ";}
							continue;
						}
						tasks.at(position).switchDeletion();
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
							if (i == parts.size() - 1){ std::cout << "x++: ";}
							continue;
						}
						tasks.at(position).switchCompletion();
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
			reader.saveFile(tasks, ofile);
			tasks.clear();
			reader.readFile(ofile, tasks);
		}
		std::cout << "x++: ";
	} 
	std::cout << "Program terminated with EOF." << std::endl;
}
