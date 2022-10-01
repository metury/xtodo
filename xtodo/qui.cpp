/// @file qui.hpp Handling graphical interface.
#include "qui.hpp"
#include "ui_mainwindow.h"
#include "ui_editwindow.h"
#include "ui_findwindow.h"

MainWindow::MainWindow( Tasks* tasks, Reader* reader, std::string* ofile, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    tasks_ = tasks;
    reader_ = reader;
    ofile_ = ofile;
    ui->setupUi(this);
    
    // Connect all buttons / key shortcuts to its function.
    connect(ui->actionEdit, SIGNAL (triggered()), this, SLOT(edit()));
    connect(ui->actionDone, SIGNAL (triggered()), this, SLOT(done()));
    connect(ui->actionAdd, SIGNAL (triggered()), this, SLOT(add()));
    connect(ui->actionDelete, SIGNAL (triggered()), this, SLOT(myDelete()));
    connect(ui->actionSort, SIGNAL(triggered()), this, SLOT(sort()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(redo()));
    connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(import()));
    connect(ui->actionShow_done, SIGNAL (triggered()), this, SLOT(checkDone()));
    connect(ui->actionShow_deleted, SIGNAL (triggered()), this, SLOT(checkDel()));
    connect(ui->actionFind, SIGNAL (triggered()), this, SLOT(openFind()));
    connect(ui->actionReload, SIGNAL (triggered()), this, SLOT(reload()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeColor(Task* task, size_t index){
    if(task->text() == "" || (!task->getCompletionDate().isEmpty() &&  task->getCreationDate().isEmpty())){
		ui->tasks->item(index)->setBackground(QColor(QColor::fromRgb(200,0,0)));
		ui->tasks->item(index)->setForeground(QBrush(QColor::fromRgb(255,255,255)));
	}
	else if(task->getPriority() == -1){
        return;
    }
	else{
		size_t max = 157;
		int red = 255 - std::min(max, task->getPriority()*21);
		int green = std::min(max, task->getPriority()*12);
		int blue = std::min(max, task->getPriority()*7);
		ui->tasks->item(index)->setForeground(QBrush(QColor::fromRgb(red,green,blue)));
	}
}

void MainWindow::refresh(){
    ui->tasks->clear();
    size_t index = 0;
    size_t colorIndex = 0;
    for(auto&& task : *tasks_){
        if (task.isComplete() && !done_){}
        else if (task.markedForDeletion() && !deleted_){}
        else {
            std::stringstream stream;
            stream << task;
            QString qstring = QString::fromStdString(stream.str());
            MyItem* item = new MyItem(index);
            item->setText(qstring);
            ui->tasks->addItem(item);
            changeColor(&task,colorIndex);
            ++colorIndex;
        }
        ++index;
    }
}

void MainWindow::refreshMatch(const std::string& match){
	ui->tasks->clear();
    size_t index = 0;
    size_t colorIndex = 0;
    for(auto&& task : *tasks_){
        if (task.isComplete() && !done_){}
        else if (task.markedForDeletion() && !deleted_){}
        else if (task.match(match)){
            std::stringstream stream;
            stream << task;
            QString qstring = QString::fromStdString(stream.str());
            MyItem* item = new MyItem(index);
            item->setText(qstring);
            ui->tasks->addItem(item);
            changeColor(&task,colorIndex);
            ++colorIndex;
        }
        ++index;
    }
}

void MainWindow::undo(){
    int index = ui->tasks->currentRow();
    tasks_->undo();
    refresh();
    ui->tasks->setCurrentRow(index);
}

void MainWindow::redo(){
    int index = ui->tasks->currentRow();
    tasks_->redo();
    refresh();
    ui->tasks->setCurrentRow(index);
}

size_t MainWindow::selectedItem(QListWidgetItem* item){
	if (typeid(*item) == typeid(MyItem)){
		auto temp = dynamic_cast<MyItem&>(*item);
		return temp.getIndex();
	}
	else{
		QErrorMessage* em = new QErrorMessage();
        em->showMessage(QString::fromStdString("This shouldn't have happened."));
        throw NonExistingItemException(0,0);
	}
}

void MainWindow::edit(){
	if(ui->tasks->selectedItems().size() > 0){
		size_t index = selectedItem(ui->tasks->selectedItems()[0]);
		try{
			editwindow* ew = new editwindow(&tasks_->at(index), this);
			ew->show();
		} catch (Exception& e){
			QErrorMessage* em = new QErrorMessage();
			em->showMessage(QString::fromStdString(e.what()));
		}
	}
}

void MainWindow::add(){
    editwindow* ew = new editwindow(&tasks_->addEmpty(), this);
    ew->show();
    // tasks_-> checkEmpty(); // Called after deletion.
}

void MainWindow::myDelete(){
	if(ui->tasks->selectedItems().size() > 0){
		size_t index = selectedItem(ui->tasks->selectedItems()[0]);
		int rowIndex = ui->tasks->currentRow();
		try{
			tasks_ -> at(index).switchDeletion();
			refresh();
		} catch (Exception& e){
			QErrorMessage* em = new QErrorMessage();
			em->showMessage(QString::fromStdString(e.what()));
		}
		if(deleted_){
			ui->tasks->setCurrentRow(rowIndex);
		}
	}
}

void MainWindow::done(){
	if(ui->tasks->selectedItems().size() > 0){
		size_t index = selectedItem(ui->tasks->selectedItems()[0]);
		int rowIndex = ui->tasks->currentRow();
		try{
			tasks_ -> at(index).switchCompletion ();
			refresh();
		} catch (Exception& e){
			QErrorMessage* em = new QErrorMessage();
			em->showMessage(QString::fromStdString(e.what()));
		}
		if(done_){
			ui->tasks->setCurrentRow(rowIndex);
		}
	}
}

void MainWindow::sort(){
    tasks_->sort();
    refresh();
}

void MainWindow::save(){
    reader_->saveFile((*tasks_), (*ofile_));
}

void MainWindow::open(){
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open Document"),QDir::currentPath(),tr("text files (*.txt)") );
    std::string file = filepath.toStdString();
    try{
        reader_->saveFile((*tasks_), (*ofile_));
        tasks_->clear();
        (*ofile_) = file;
        reader_->readFile(file, (*tasks_));
    } catch (Exception& e){
        QErrorMessage* em = new QErrorMessage();
        em->showMessage(QString::fromStdString(e.what()));
    }
    refresh();
}

void MainWindow::import(){
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open Document"),QDir::currentPath(),tr("text files (*.txt)"));
    std::string file = filepath.toStdString();
    try{
        reader_->saveFile((*tasks_), (*ofile_));
        (*ofile_) = file;
        reader_->readFile(file, (*tasks_));
    } catch (Exception& e){
        QErrorMessage* em = new QErrorMessage();
        em->showMessage(QString::fromStdString(e.what()));
    }
    refresh();
}

void MainWindow::checkDel(){
    deleted_ = !deleted_;
    refresh();
}

void MainWindow::checkDone(){
    done_ = !done_;
    refresh();
}

void MainWindow::openFind(){
	findwindow* fw = new findwindow(this);
	fw->show();
}

void MainWindow::showFound(std::string match){
	refreshMatch(match);
}

void MainWindow::reload(){
	reader_->saveFile(*tasks_, *ofile_);
	tasks_->clear();
	reader_->readFile(*ofile_, *tasks_);
	refresh();
}

editwindow::editwindow(Task* task, MainWindow* mainParent, bool emptyText, bool emptyDate, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editwindow)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(mysave()));
    task_ = task;
    parent_ = mainParent;
    
    emptyDate_ = emptyDate;
    emptyText_ = emptyText;
    write();
}

editwindow::~editwindow()
{
    delete ui;
}

void editwindow::write(){
    if (emptyDate_){
        ui->complDateEdit->setStyleSheet("QLineEdit { background-color : red; color : white; }");
        ui->createDateEdit->setStyleSheet("QLineEdit { background-color : red; color : white; }");
    }
    if(emptyText_){
        ui->textEdit->setStyleSheet("QLineEdit { background-color : red; color : white; }");
    }
    if(task_->getPriority() != -1){
        char chr = 'A' + task_->getPriority();
        QChar qchr(chr);
        ui->priorEdit->insert(qchr);
    }
    QString temp = QString::fromStdString(task_->getText());
    ui->textEdit->insert(temp);
    temp = QString::fromStdString(task_->getContext());
    ui->contextEdit->insert(temp);
    temp = QString::fromStdString(task_->getProject());
    ui->projectEdit->insert(temp);
    if(!task_->getCompletionDate().isEmpty()){
        std::stringstream stream;
        stream << task_-> getCompletionDate();
        temp = QString::fromStdString(stream.str());
        ui->complDateEdit->insert(temp);
    }
    if(!task_->getCreationDate().isEmpty()){
        std::stringstream stream;
        stream << task_-> getCreationDate();
        temp = QString::fromStdString(stream.str());
        ui->createDateEdit->insert(temp);
    }
}

void editwindow::mysave(){
    std::string line = (ui->priorEdit->text()).toStdString();
    if(line.length() == 1 && line[0] <= 'Z' && line[0] >= 'A'){
        task_->setPriority(line[0]);
    }
    else{
        task_->setPriority('0');
    }
    line = (ui->complDateEdit->text()).toStdString();
    if(line == ""){
        task_->setCompletionDate(Date());
    }
    else if(line.length() >= 1){
        task_->setCompletionDate(line);
    }
    line = (ui->createDateEdit->text()).toStdString();
    if(line == ""){
        task_->setCreationDate(Date());
    }
    else if(line.length() >= 1){
        task_->setCreationDate(line);
    }
    line = (ui->textEdit->text()).toStdString();
    task_->text() = line;
    line = (ui->contextEdit->text()).toStdString();
    task_->context() = line;
    line = (ui->projectEdit->text()).toStdString();
    task_->project() = line;

    bool date = (!task_->getCompletionDate().isEmpty() && task_->getCreationDate().isEmpty());
    bool text = (task_->text() == "");
    if(date || text){
        editwindow* ew = new editwindow(task_, parent_, text, date);
        ew->show();
        ew->showError();
    }
    parent_->refresh();
}

void editwindow::showError(){
	if (emptyDate_ && emptyText_){
		QErrorMessage* em = new QErrorMessage();
        em->showMessage("1) Text cannot be empty. 2) If completion date is set, creation date need to be defined as well.");
	}
	else if (emptyDate_){
		QErrorMessage* em = new QErrorMessage();
        em->showMessage("1) If completion date is set, creation date need to be defined as well.");
	}
	else if (emptyText_){
		QErrorMessage* em = new QErrorMessage();
        em->showMessage("1) Text cannot be empty.");
	}
}

MyItem::MyItem(size_t index){
	task_index_ = index;
}

size_t MyItem::getIndex(){
	return task_index_;
}

findwindow::findwindow(MainWindow* mainParent, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findwindow)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(find()));
    parent_ = mainParent;
}

findwindow::~findwindow()
{
    delete ui;
}

void findwindow::find(){
	parent_->showFound((ui->findEdit->text()).toStdString());
}
