/// @file qui.hpp Handling graphical interface.
#include "gui.hpp"


MainWindow::MainWindow(tasks* tasks, file_parser* reader, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), tasks_(tasks), reader_(reader){
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
    connect(ui->actionQuit, SIGNAL (triggered()), this, SLOT(quit()));
    connect(ui->actionConfig, SIGNAL (triggered()), this, SLOT(saveConfig()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::changeColor(task* task, size_t index){
    if(task->set_text() == "" || (!task->get_completion_date().is_empty() &&  task->get_creation_date().is_empty())){
		ui->tasks->topLevelItem(index)->setBackground(0, QColor(QColor(0x972323)));
		ui->tasks->topLevelItem(index)->setForeground(0, QBrush(QColor(0xFFFFFF)));
	}
	else if(task->is_complete()){
		ui->tasks->topLevelItem(index)->setBackground(0, QColor(QColor(0xFFFFFF)));
		ui->tasks->topLevelItem(index)->setForeground(0, QBrush(QColor(0x888888)));
	}
	else if(task->get_priority() == -1){
        return;
    }
	else{
		ui->tasks->topLevelItem(index)->setForeground(0, QBrush(colors[task->get_priority()]));
	}
}

void MainWindow::refresh(){
    ui->tasks->clear();
    size_t index = 0;
    size_t colorIndex = 0;
    for(auto&& task : *tasks_){
        if (task.is_complete() && !done_){}
        else if (task.marked_for_deletion() && !deleted_){}
        else if (task.match(matchingString_)){
            std::stringstream stream;
            stream << task;
            QString qstring = QString::fromStdString(stream.str());
            task_item* item = new task_item(&task);
            ui->tasks->addTopLevelItem(item);
            changeColor(&task,colorIndex);
            ++colorIndex;
        }
        ++index;
    }
}

void MainWindow::refreshMatch(const std::string& match){
	matchingString_ = match;
	refresh();
}

void MainWindow::undo(){
    tasks_->undo();
    refresh();
}

void MainWindow::redo(){
    tasks_->redo();
    refresh();
}

void MainWindow::quit(){
	try{
		reader_->save_file((*tasks_));
	}
	catch(Exception& e){
		QErrorMessage* em = new QErrorMessage();
        em->showMessage(QString::fromStdString(e.what()));
	}
	this->close();
}

void MainWindow::edit(){
    auto item = ui->tasks->currentItem();
    if(item == nullptr) return;
    auto task_it = dynamic_cast<task_item*>(item);
    editwindow* ew = new editwindow(task_it->get_task(), this);
    ew->show();
}

void MainWindow::add(){
    editwindow* ew = new editwindow(&tasks_->add_empty(), this);
    ew->show();
}

void MainWindow::myDelete(){
    auto item = ui->tasks->currentItem();
    if(item == nullptr) return;
    auto task_it = dynamic_cast<task_item*>(item);
    task_it->get_task()->switch_deletion();
    refresh();
}

void MainWindow::done(){
    auto item = ui->tasks->currentItem();
    if(item == nullptr) return;
    auto task_it = dynamic_cast<task_item*>(item);
	task_it->get_task()->switch_completion();
    refresh();
}

void MainWindow::sort(){
    tasks_->sort();
    refresh();
}

void MainWindow::save(){
	try{
		reader_->save_file((*tasks_));
	}
	catch(Exception& e){
		QErrorMessage* em = new QErrorMessage();
        em->showMessage(QString::fromStdString(e.what()));
	}
}

void MainWindow::open(){
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open Document"),QDir::currentPath(),tr("text files (*.txt)") );
    std::string file = filepath.toStdString();
    try{
        reader_->save_file((*tasks_));
        tasks_->clear();
        reader_->set_file(file);
        reader_->read_files(*tasks_);
    } catch (Exception& e){
        QErrorMessage* em = new QErrorMessage();
        em->showMessage(QString::fromStdString(e.what()));
    }
    refresh();
}

void MainWindow::openNew(){
	QString filepath = QFileDialog::getOpenFileName(this,tr("Open Document"),QDir::currentPath(),tr("text files (*.txt)") );
    std::string file = filepath.toStdString();
	reader_->set_file(file);
	reader_->read_files(*tasks_);
    refresh();
}

void MainWindow::import(){
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open Document"),QDir::currentPath(),tr("text files (*.txt)"));
    std::string file = filepath.toStdString();
    try{
        reader_->save_file((*tasks_));
        reader_->readFile(file, (*tasks_));
    } catch (Exception& e){
        QErrorMessage* em = new QErrorMessage();
        em->showMessage(QString::fromStdString(e.what()));
    }
    refresh();
}

void MainWindow::saveConfig(){
	try{
		reader_->saveConfigGUI(*ofile_);
	}
	catch(Exception& e){
		QErrorMessage* em = new QErrorMessage();
        em->showMessage(QString::fromStdString(e.what()));
	}
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
	reader_->save_file(*tasks_);
	tasks_->clear();
	reader_->readFile(*ofile_, *tasks_);
	refresh();
}

editwindow::editwindow(task* task, MainWindow* mainParent, bool emptyText, bool emptyDate, QWidget *parent) :
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
        ui->complDateEdit->setStyleSheet("QLineEdit { background-color : darkRed; color : white; }");
        ui->createDateEdit->setStyleSheet("QLineEdit { background-color : darkRed; color : white; }");
    }
    if(emptyText_){
        ui->textEdit->setStyleSheet("QLineEdit { background-color : darkRed; color : white; }");
    }
    if(task_->get_priority() != -1){
        char chr = 'A' + task_->get_priority();
        QChar qchr(chr);
        ui->priorEdit->insert(qchr);
    }
    QString temp = QString::fromStdString(task_->get_text());
    ui->textEdit->insert(temp);
    temp = QString::fromStdString(task_->get_context());
    ui->contextEdit->insert(temp);
    temp = QString::fromStdString(task_->get_project());
    ui->projectEdit->insert(temp);
    if(!task_->get_completion_date().is_empty()){
        std::stringstream stream;
        stream << task_-> get_completion_date();
        temp = QString::fromStdString(stream.str());
        ui->complDateEdit->insert(temp);
    }
    if(!task_->get_creation_date().is_empty()){
        std::stringstream stream;
        stream << task_-> get_creation_date();
        temp = QString::fromStdString(stream.str());
        ui->createDateEdit->insert(temp);
    }
}

void editwindow::mysave(){
    std::string line = (ui->priorEdit->text()).toStdString();
    if(line.length() == 1 && line[0] <= 'Z' && line[0] >= 'A'){
        task_->set_priority(line[0]);
    }
    else{
        task_->set_priority('0');
    }
    line = (ui->complDateEdit->text()).toStdString();
    if(line == ""){
        task_->set_completion_date(date());
    }
    else if(line.length() >= 1){
        task_->set_completion_date(line);
    }
    line = (ui->createDateEdit->text()).toStdString();
    if(line == ""){
        task_->set_creation_date(date());
    }
    else if(line.length() >= 1){
        task_->set_creation_date(line);
    }
    line = (ui->textEdit->text()).toStdString();
    task_->set_text() = line;
    line = (ui->contextEdit->text()).toStdString();
    task_->set_context() = line;
    line = (ui->projectEdit->text()).toStdString();
    task_->set_project() = line;

    bool date = (!task_->get_completion_date().is_empty() && task_->get_creation_date().is_empty());
    bool text = (task_->get_text() == "");
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

task_item::task_item(task* t) : task_(t){
    setText(0, QString::fromStdString(std::to_string(task_->get_priority())));
    setText(1, QString::fromStdString(task_->get_text()));
    setText(2, QString::fromStdString(task_->get_creation_date().str()));
    setText(3, QString::fromStdString(task_->get_completion_date().str()));
    setText(4, QString::fromStdString(task_->get_project()));
    setText(5, QString::fromStdString(task_->get_context()));
}

task* task_item::get_task(){
	return task_;
}

findwindow::findwindow(MainWindow* mainParent, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findwindow)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(find()));
    parent_ = mainParent;
}

findwindow::~findwindow(){
    delete ui;
}

void findwindow::find(){
	parent_->showFound((ui->findEdit->text()).toStdString());
}
