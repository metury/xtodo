/// @file reader.hpp Header file for reading files, arguments and configuration file.
#ifndef reader_hpp_
#define reader_hpp_

#include <string>
#include <set>
#include <fstream>
#include <vector>
#include <filesystem>
#include "task.hpp"
#include "exception.hpp"

/// File parser and argumetn parser.
class file_parser{
    public:
		/// Default constructor.
		file_parser();
		/// Parse input arguments.
        /// @param args Vector of arguments.
        /// @return If help was found, then do not run application and print helpline.
        bool parse_arguments(const std::vector<std::string>& args);
		/// Read config file.
        void read_config();



		/// Read all files in given vector and add all tasks.
		/// @param tasks Where to store all tasks.
        void read_files(tasks& tasks);
		/// Save current state of tasks to output file.
		/// @param tasks From where to get tasks.
        void save_file(const tasks& tasks);

        /// read only one file.
        /// @param file Whhich file to read.
        /// @param tasks Where to store all tasks.
        void readFile(const std::string& file, tasks& tasks);
		/// Save configuration while running GUI client.
        /// @param file Used fiel for tasks.
        void saveConfigGUI(std::string& file);

    private:
		/// Save config file with current settings.
		/// @param settings Current stettings.
		/// @param files Vector of given files.
		/// @param ofile Output file.
        void save_config();
		/// Filepath to config file.
		std::filesystem::path config_;
		bool save_config_;
		bool use_config_;
		std::vector<std::filesystem::path> files_;
		std::filesystem::path ofile_;
};

#endif
