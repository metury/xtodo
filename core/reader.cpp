/// @file reader.cpp Cpp file for implementation of reader.hpp.
#include "reader.hpp"
#include "task.hpp"

file_parser::file_parser(){
	#ifdef _WIN32
        config_ = std::filesystem::path(getenv("APPDATA"));
    #else
        const char* xdgConfigHome = getenv("XDG_CONFIG_HOME");
        if(xdgConfigHome != nullptr){
            config_ = std::filesystem::path(getenv("XDG_CONFIG_HOME"));
        }
        if (configPath_.empty()) {
            config_ = std::filesystem::path(getenv("HOME")) / ".config";
        }
    #endif
	config_ = config_ / "xtodo";
	if(!std::filesystem::exists(config_)){
        std::filesystem::create_directories(config_);
    }
	config_ = config_ / "config";
	if(!std::filesystem::exists(config_)){
		std::ofstream ostream;
		ostream.open(config_);
		if(!ostream.is_open()){
			throw FileProcessingException(config_, OUTPUT);
		}
		ostream << "";
	}
}

void file_parser::read_config(){
	if (!use_config_){
		if(ofile == "" && files.size() == 1)
			ofile = files[0];
		else if(ofile == "")
			throw NonGivenSetting(OFILE);
		if (files.size() == 0)
			throw NonGivenSetting(IFILE);
		if(save_config_)
			saveConfig(settings,files,ofile);
		return;
	}
	std::ifstream stream;
	stream.open(config);
	if(!stream.is_open())
		throw FileProcessingException(config, INPUT);
	std::string line;
	while(getline(stream,line)){
		if(line[0] == '#') continue;
		if(line.length() >= 5 && line.substr(0,5) == "FILE=")
			files.push_back(line.substr(5,line.length()));
		else if(line.length() >= 6 && line.substr(0,6) == "OFILE=" && ofile == "")
			if (ofile == "") ofile = line.substr(6,line.length());
	}
	stream.close();
	if(ofile == "" && files.size() == 1)
		ofile = files[0];
	else if (ofile == "")
		throw NonGivenSetting(OFILE);
	if(save_config_){
		save_config();
	}
}

void ArgumentReader::save_config(){
	std::ofstream ostream;
	ostream.open(config);
	if(!ostream.is_open()){
		throw FileProcessingException(config, OUTPUT);
	}
	ostream << "# Saved configuration from last time." << std::endl;
	ostream << "# Used files." << std::endl;
	for(auto&& file : files_){
		ostream << "FILE=" << file << std::endl;
	}
	ostream << "# Output file." << std::endl;
	ostream << "OFILE=" << ofile_ << std::endl;
	ostream.close();
}

bool file_parser::parse_arguments(const std::vector<std::string>& args){
	bool output = false;
	save_config_ = false;
	use_config_ = true;
	for(auto&& arg : args){
		if(arg[0] == '-'){
			if (arg == "-s"){
				save_config_ = true;
			}
			else if (arg == "-o"){
				output = true;
			}
			else if(arg == "--noconf"){
				use_config_ = false;
			}
			else if(arg == "-h" || arg == "--help"){
				return false;
			}
		}
		else{
			if(output){
				ofile_ = arg;
				output = false;
			}
			else{
				files_.push_back(arg);
			}
		}
	}
	return true;
}

void file_parser::read_files(Tasks& tasks){
	std::ifstream stream;
	for(auto&& file : files_){
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

void file_parser::readFile(const std::string& file, Tasks& tasks){
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

void file_parser::save_file(const Tasks& tasks){
	std::ofstream stream;
	stream.open(ofile);
	if(!stream.is_open()){
		throw FileProcessingException(ofile, OUTPUT);
	}
	tasks.print(stream);
	stream.close();
}

void ArgumentReader::saveConfigGUI(std::string& file){
	std::ofstream ostream;
	ostream.open(config);
	if(!ostream.is_open()){
		throw FileProcessingException(config, OUTPUT);
	}
	ostream << "# File." << std::endl;
	ostream << "FILE=" << file << std::endl;
	ostream.close();
}
