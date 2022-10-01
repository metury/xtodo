/// @file reader.cpp Cpp file for implementation of reader.hpp.
#include "reader.hpp"
#include "task.hpp"

void ArgumentReader::readConfig(Settings& settings, std::vector<std::string>& files, std::string& ofile){
	if (!settings.useConfig){
		if(ofile == "" && files.size() == 1){
			ofile = files[0];
		}
		else if(ofile == ""){
			throw NonGivenSetting(OFILE);
		}
		if (files.size() == 0){
			throw NonGivenSetting(IFILE);
		}
		if (settings.client == NONE){
			throw NonGivenSetting(INTERFACE);
		}
		if(settings.save){
			saveConfig(settings,files,ofile);
		}
		
		return;
	}
	std::ifstream stream;
	stream.open(config);
	if(!stream.is_open()){
		throw FileProcessingException(config, INPUT);
	}
	std::string line;
	while(getline(stream,line)){
		if(line[0] == '#'){
			continue;
		}
		if(line.length() >= 5 && line.substr(0,5) == "FILE="){
			files.push_back(line.substr(5,line.length()));
		}
		else if(line.length() >= 7 && line.substr(0,7) == "CLIENT=" && settings.client == NONE){
			if(line.substr(7,line.length()) == "CLI"){
				settings.client = CLI;
			}
			else{
				settings.client = GUI;
			}
		}
		else if(line.length() >= 6 && line.substr(0,6) == "OFILE=" && ofile == ""){
			if (ofile == ""){
				ofile = line.substr(6,line.length());
			}
		}
	}
	stream.close();
	if(ofile == "" && files.size() == 1){
		ofile = files[0];
	}
	else if (ofile == ""){
		throw NonGivenSetting(OFILE);
	}
	if(settings.save){
		saveConfig(settings, files, ofile);
	}
}

void ArgumentReader::saveConfig(const Settings& settings, const std::vector<std::string>& files, const std::string ofile){
	std::ofstream ostream;
	ostream.open(config);
	if(!ostream.is_open()){
		throw FileProcessingException(config, OUTPUT);
	}
	ostream << "# Saved configuration from last time." << std::endl;
	ostream << "# User interface." << std::endl;
	if(settings.client == CLI){
		ostream << "CLIENT=CLI" << std::endl;
	}
	else{
		ostream << "CLIENT=GUI" << std::endl;
	}
	ostream << "# Used files." << std::endl;
	for(auto&& file : files){
		ostream << "FILE=" << file << std::endl;
	}
	ostream << "# Output file." << std::endl;
	ostream << "OFILE=" << ofile << std::endl;
	ostream.close();
}

bool ArgumentReader::parseArguments(const std::vector<std::string>& args, Settings& settings,std::vector<std::string>& files, std::string& ofile){
	bool output = false;
	for(auto&& arg : args){
		if(arg[0] == '-'){
			if(arg == "-cli"){
				settings.client = CLI;
			}
			else if (arg == "-gui"){
				settings.client = GUI;
			}
			else if (arg == "-s"){
				settings.save = true;
			}
			else if (arg == "-o"){
				output = true;
			}
			else if(arg == "-noconf"){
				settings.useConfig = false;
			}
			else if(arg == "-h" || arg == "--help"){
				return false;
			}
		}
		else{
			if(output){
				ofile = arg;
				output = false;
			}
			else{
				files.push_back(arg);
			}
		}
	}
	return true;
}

void Reader::readFiles(const std::vector<std::string>& files, Tasks& tasks){
	std::ifstream stream;
	for(auto&& file : files){
		stream.open(file);
		if(!stream.is_open()){
			throw FileProcessingException(file, INPUT);
		}
		std::string line;
		while(getline(stream,line)){
			tasks.addTask(line);
		}
		stream.close();
	}
}

void Reader::readFile(const std::string& file, Tasks& tasks){
	std::ifstream stream;
	stream.open(file);
	if(!stream.is_open()){
		throw FileProcessingException(file, INPUT);
	}
	std::string line;
	while(getline(stream,line)){
		tasks.addTask(line);
	}
	stream.close();
}

void Reader::saveFile(const Tasks& tasks, const std::string& ofile){
	std::ofstream stream;
	stream.open(ofile);
	if(!stream.is_open()){
		throw FileProcessingException(ofile, OUTPUT);
	}
	tasks.print(stream);
	stream.close();
}
