/// @file exception.hpp Headder file for handling all exceptions.
#ifndef exception_hpp_
#define exception_hpp_

#include <string>
#include <sstream>
#include <exception>

/// if the file is for output or input.
enum FILETYPE {OUTPUT, INPUT};

/// What settings is missing, output file, input file or user interface.
enum SETTINGTYPE {OFILE, IFILE, INTERFACE};

/// Default class for exceptions.
class Exception : public std::exception{
	public:
	
		/// Get the error message.
		/// @return Reference to the error message.
		inline std::string& what(){
			return message_;
		}
		
		/// Error message.
		std::string message_;
};

/// When someone is trying to reach nonexisting task.
class NonExistingItemException : public Exception{
	public:
	
		/// Default constructor.
		/// @param given Which task is nonreachable.
		/// @param max What is the maximal position that cold be reached.
		NonExistingItemException(size_t given, size_t max);
};

/// When the givven file couldn't be accesed.
class FileProcessingException : public Exception{
	public:
	
		/// Default constructor.
		/// @param filename What is the filename / filepath.
		/// @param type If it is output or input.
		FileProcessingException(const std::string& filename, FILETYPE type);
};

/// When there is missing settings to proper start the application.
class NonGivenSetting : public Exception{
	public:
	
		/// Default constructor.
		/// @param type What settings is missing.
		NonGivenSetting(SETTINGTYPE type);
};


#endif
