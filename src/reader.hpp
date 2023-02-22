/// @file reader.hpp Header file for reading files, arguments and configuration file.
#ifndef reader_hpp_
#define reader_hpp_

#include <string>
#include <set>
#include <fstream>
#include <vector>
#include "task.hpp"
#include "exception.hpp"

/// User interface.
enum CLIENT {GUI, CLI, NONE};

/// What is the settings of current program.
struct Settings{
	public:
		
		/// Chosen client.
		CLIENT client = GUI;
		
		/// Whether to save this settings to config file.
		bool save = false;
		
		/// Whether to read config file or not.
		bool useConfig = true;
};

/// Read config file and arguments, also save config if needed.
class ArgumentReader{
    public:
		
		/// Read config file.
		/// @param settings Which setting to change or load.
		/// @param files Which are the files that are being read from.
		/// @param ofile What is the output file for storing changes.
        void readConfig(Settings& settings, std::vector<std::string>& files, std::string& ofile);
        
        /// Parse input arguments.
        /// @param args Vector of arguments.
        /// @param settings Current settings.
        /// @param files Vector of given files.
        /// @param ofile Output file.
        /// @return If help was found, then do not run application and print helpline.
        bool parseArguments(const std::vector<std::string>& args, Settings& settings, std::vector<std::string>& files, std::string& ofile);
        
        /// Save configuration while running GUI client.
        /// @param file Used fiel for tasks.
        void saveConfigGUI(std::string& file);
    private:
		
		/// Save config file with current settings.
		/// @param settings Current stettings.
		/// @param files Vector of given files.
		/// @param ofile Output file.
        void saveConfig(const Settings& settings, const std::vector<std::string>& files, const std::string ofile);
		
		/// Filepath to config file.
		const std::string config = ".xconfig";
};

/// Reading and saving files with todo.txt syntax in it.
class Reader{
    public:

		/// Read all files in given vector and add all tasks.
		/// @param files Vector of given files with tasks.
		/// @param tasks Where to store all tasks.
        void readFiles(const std::vector<std::string>& files, Tasks& tasks);
        
        /// read only one file.
        /// @param file Whhich file to read.
        /// @param tasks Where to store all tasks.
        void readFile(const std::string& file, Tasks& tasks);
		
		/// Save current state of tasks to output file.
		/// @param tasks From where to get tasks.
		/// @param ofile Which file to use as an output file.
        void saveFile(const Tasks& tasks, const std::string& ofile);
};

#endif
