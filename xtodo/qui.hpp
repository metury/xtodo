/// @file qui.hpp Header file for handling GUI.
#ifndef qui_hpp
#define qui_hpp

#include <QDialog>
#include <QMainWindow>
#include <QErrorMessage>
#include <QFileDialog>
#include <QListWidgetItem>
#include <string>
#include <typeinfo>
#include "task.hpp"
#include "reader.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; class editwindow; class findwindow; class openfilewindow;}
QT_END_NAMESPACE

/// Main window for showing tasks.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

	/// Default constructor.
    /// @param tasks What tasks it is operating with.
    /// @param reader The reader that is used.
    /// @param ofile What is the filepath to the output file.
    /// @param *parent qt constructor.
    MainWindow( Tasks* tasks, Reader* reader, std::string* ofile, QWidget *parent = nullptr);
    
    /// Default destructor.
    ~MainWindow();
    
    /// Reload list with tasks.
    void refresh();
    
    /// Reload list with matching ones.
    void refreshMatch(const std::string& match);
    
    /// Opening new instance of window without given textfile.
    void openNew();
    
    /// Add new line to list with taks.
    /// @param text What is to be inserted.
    void addPart(QString text);
	
	/// Change color of item based on its priority or if it is incorrect.
	/// @param task Which task.
	/// @param index What is the index in list widget.
    void changeColor(Task* task, size_t index);
    
    /// Return index of the task that is selected.
    /// @return Its index.
    size_t selectedItem(QListWidgetItem* item);
    
    /// For showing found tasks.
    void showFound(std::string match);

public slots:

	/// Slot for pressing undo button.
    void undo();
	
	/// Slot for pressing redo button.
    void redo();
    
    /// Slot for pressing edit button.
    void edit();
    
    /// Slot for pressing add button.
    void add();
    
    /// Slot for pressing done button.
    void done();
    
    /// Slot for pressing delete button.
    void myDelete();
    
    /// Slot for sorting tasks.
    void sort();
    
    /// Slot for saving document.
    void save();
    
    /// Opening new file.
    void open();
    
    /// Importing from another file.
    void import();
    
    /// Show or not show deleted items.
    void checkDel();
    
    /// Show or not show done items.
    void checkDone();
    
    /// Open find window.
    void openFind();
    
    /// Reload file.
    void reload();
    
    /// Quit the application.
    void quit();
    
    /// Save configuration to its file.
    void saveConfig();

private:
	
	/// Qt ui.
    Ui::MainWindow *ui;
    
    /// Which tasks it is handling.
    Tasks* tasks_;
    
    /// Which reader to use when writing to files or reading them.
    Reader* reader_;
    
    /// What is the path to output file.
    std::string* ofile_;
    
    /// Whether to show done items.
    bool done_ = false;
    
    /// Whether to show deleted items.
    bool deleted_ = false;
    
    /// Colors for tasks.
    const std::vector<QColor> colors = {
		QColor(0xFF0000),
		QColor(0xFF5900),
		QColor(0xFFA300),
		QColor(0xBAB400),
		QColor(0x7AC700),
		QColor(0x3CD100),
		QColor(0x00D159),
		QColor(0x00D1B5),
		QColor(0x00BBD1),
		QColor(0x0069D1),
		QColor(0x001FFF),
		QColor(0x6D00FF),
		QColor(0xB900FF),
		QColor(0xC900BB),
		QColor(0xC90085),
		QColor(0xC9006D),
		QColor(0x8E000D),
		QColor(0x8E3C00),
		QColor(0x717302),
		QColor(0x1E7302),
		QColor(0x027344),
		QColor(0x027371),
		QColor(0x023C73),
		QColor(0x020373),
		QColor(0x4A0273),
		QColor(0x730263)};
};


/// Window for editing and adding tasks.
class editwindow : public QDialog
{
    Q_OBJECT

public:

	/// Default constructor.
	/// @param task With which task it is operating.
	/// @param mainParent Which mainWindow called this editwindow.
	/// @param *parent qt constructor
	/// @param emptyText If the last editwindow ended with emptytext.
	/// @param emptyDate If the last editwindow ended with wrong dates.
    explicit editwindow(Task* task, MainWindow* mainParent, bool emptyText = false, bool emptyDate = false,  QWidget *parent = nullptr);
    
    /// Default destructor.
    ~editwindow();
    
    /// Write everything about the task into boxes.
    void write();
    
    /// If there was mistake show error when loading new window.
    void showError();

public slots:

	/// Slot for pressing save button.
    void mysave();

private:

	/// Qt ui.
    Ui::editwindow *ui;
    
    /// Which task it is handling.
    Task* task_;
    
    /// Which mainwindow called this window. For reloading the parent.
    MainWindow* parent_;
    
    /// When date is not defined and should be.
    bool emptyText_;
    
    /// When there was an empty text before.
    bool emptyDate_;
};

/// Derivative of list widget item to store index of the task.
class MyItem : public QListWidgetItem{
	public:
	
		/// Default constructor.
		/// @param index Index of the task.
		MyItem(size_t index);
		
		/// Get the index of the task.
		/// @return Its index.
		size_t getIndex();
	private:
	
		/// Tasks index.
		size_t task_index_;
};


/// Window for editing and adding tasks.
class findwindow : public QDialog
{
    Q_OBJECT

public:

	/// Default constructor.
	/// @param mainParent Which mainWindow called this editwindow.
	/// @param *parent qt constructor
    explicit findwindow(MainWindow* mainParent, QWidget *parent = nullptr);
    
    /// Default destructor.
    ~findwindow();

public slots:

	/// Slot for pressing ok button.
    void find();

private:

	/// Qt ui.
    Ui::findwindow *ui;
    
    /// Which mainwindow called this window. For reloading the parent.
    MainWindow* parent_;
};

#endif
