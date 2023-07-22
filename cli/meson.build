project('xtodo-cli', 'cpp',
  version : '1.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

source = files('main.cpp', '../core/exception.cpp', '../core/reader.cpp', '../core/task.cpp', 'terminal.cpp')

executable('xtodo-cli',
	source,
	install : true)
