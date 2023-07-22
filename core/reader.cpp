/// @file reader.cpp Cpp file for implementation of reader.hpp.
#include "reader.hpp"

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
		save_config();
	}
}

void file_parser::read_config(){
	if (!use_config_){
		if(ofile_ == "" && files_.size() == 1)
			ofile_ = files_[0];
		else if(ofile_ == "")
			throw NonGivenSetting(OFILE);
		if (files_.size() == 0)
			throw NonGivenSetting(IFILE);
		if(save_config_)
			save_config();
		return;
	}
	try{
        std::ifstream stream;
        stream.open(config_);
        std::string line;
        while(getline(stream, line)){
            if(line[0] == '#') continue;
            if(line.length() >= 5 && line.substr(0,5) == "FILE=")
                files_.push_back(line.substr(5,line.length()));
            else if(line.length() >= 6 && line.substr(0,6) == "OFILE=" && ofile_ == "")
                if (ofile_ == "") ofile_ = line.substr(6, line.length());
        }
	} catch(std::exception& e){
        std::cout << e.what() << std::endl;
	}
	if(ofile_ == "" && files_.size() == 1)
		ofile_ = files_[0];
	else if (ofile_ == "")
		throw NonGivenSetting(OFILE);
	if(save_config_){
		save_config();
	}

}

void file_parser::save_config(){
    try{
        std::ofstream ostream;
        ostream.open(config_);
        ostream << "# Saved configuration from last time." << std::endl;
        ostream << "# Used files." << std::endl;
        for(auto&& file : files_){
            ostream << "FILE=" << file << std::endl;
        }
        ostream << "# Output file." << std::endl;
        ostream << "OFILE=" << ofile_ << std::endl;
    } catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
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

void file_parser::read_files(tasks& tasks){
	std::ifstream stream;
	for(auto&& file : files_){
        try{
            stream.open(file);
            std::string line;
            while(getline(stream,line)){
                tasks.add_task(line);
            }
            stream.close();
        } catch(std::exception& e){
            std::cout << e.what() << std::endl;
        }
	}
}

void file_parser::readFile(const std::string& file, tasks& tasks){
    try{
        std::ifstream stream;
        stream.open(file);
        std::string line;
        while(getline(stream,line)){
            tasks.add_task(line);
        }
    } catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

void file_parser::save_file(const tasks& tasks){
	try{
        std::ofstream stream;
        stream.open(ofile_);
        tasks.print(stream);
    } catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

void file_parser::saveConfigGUI(std::string& file){
    try{
        std::ofstream ostream;
        ostream.open(config_);
        ostream << "# File." << std::endl;
        ostream << "FILE=" << file << std::endl;
	} catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
}
