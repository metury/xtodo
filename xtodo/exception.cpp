/// @file exception.cpp Source file for handling all exceptions.
#include "exception.hpp"

FileProcessingException::FileProcessingException(const std::string& filename, FILETYPE type){
	std::stringstream stream;
	if(type == OUTPUT){
		stream << "Output ";
	}
	else{
		stream << "Input ";
	}
	stream << "file " << filename << " couldn't be used.";
	message_ = stream.str();
}

NonExistingItemException::NonExistingItemException(size_t given, size_t max){
	std::stringstream stream;
	stream << "Task with given number " << given << " does not exists. Maximum is " << max << ".";
	message_ = stream.str();
}

NonGivenSetting::NonGivenSetting(SETTINGTYPE type){
	std::stringstream stream;
	stream << "There was not specified which ";
	if (type == OFILE){
		stream << "output file will be used.";
	}
	else if (type == IFILE){
		stream << "input file will be used.";
	}
	else if (type == INTERFACE){
		stream << "interface will be used.";
	}
	message_ = stream.str();
}
