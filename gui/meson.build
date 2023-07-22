project('xtodo', 'cpp',
  version : '1.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

qt6 = import('qt6')
qt6_dep = dependency('qt6', modules: ['Core', 'Gui', 'Widgets'])

source = files('main.cpp', '../core/exception.cpp', 'gui.cpp', '../core/reader.cpp', '../core/task.cpp')

resources = qt6.compile_resources(sources: ['resources/resources.qrc'])
ui_files = qt6.compile_ui(sources: ['mainwindow.ui', 'editwindow.ui', 'findwindow.ui'])
moc_files = qt6.compile_moc(headers : ['gui.hpp'])

executable('xtodo',
	source, moc_files, ui_files, resources,
	dependencies : [qt6_dep],
	install : true)
