/// @file task.hpp Source file for classes Task, Tasks and everything related to them.
#include "task.hpp"

void splitString(const std::string& line, const char splitter, std::vector<std::string>& parts){
	size_t begin = 0;
	for(size_t i = 0; i < line.length(); ++i){
		if(line[i] == splitter){
			size_t length = i - begin;
			if (length > 0){
				parts.push_back(line.substr(begin,length));
			}
			begin = i+1;
		}
	}
	if(begin != line.length()){
		size_t length = line.length() - begin;
		parts.push_back(line.substr(begin, length));
	}
}

void bindStrings(const std::vector<std::string>& parts, size_t start, size_t end,std::string& bind){
	std::stringstream stream;
	bool first = true;
	for (size_t i = start; i < end; ++i){
		if(first){
			stream << parts[i];
			first = false;
		}
		else{
			stream << " " << parts[i];
		}
	}
	bind = stream.str();
}

bool isDate(const std::string& text){
	if(text.length() > 10){
		return false;
	}
	size_t lastNoneDigit = 0;
	size_t numberCounter = 0;
	size_t dashCounter = 0;
	for (size_t i = 0; i < text.length(); ++i){
		if(text[i] == '-' && lastNoneDigit < i){
			lastNoneDigit = i;
			++dashCounter;
			continue;
		}
		else if (text[i] == '-'){
			return false;
		}
		if(!isdigit(text[i])){
			return false;
		}
		if(numberCounter == dashCounter){
			++numberCounter;
		}
	}
	if(numberCounter == 3 && dashCounter == 2){
		return true;
	}
	return false;
}

Date convertDate(const std::string& writtenDate){
    std::vector<std::string> parts;
    splitString(writtenDate, '-', parts);
    int year = std::stoi(parts[0]);
    int month = std::stoi(parts[1]);
    int day = std::stoi(parts[2]);
    if (month > 12 || day > 31){
		year = month = day =0;
	}
    Date date = Date(year, month, day);
    return date;
}

void Date::currentDate(){
	std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    year = (now->tm_year + 1900);
    month = (now->tm_mon + 1);
    day = now->tm_mday;
}

bool operator<(const Date& date1, const Date& date2){
	if(date1.isEmpty() && !date2.isEmpty()){
		return false;
	}
	else if(!date1.isEmpty() && date2.isEmpty()){
		return true;
	}
	if (date1.year < date2.year){
		return true;
	}
	else if(date1.year > date2.year){
		return false;
	}
	if (date1.month < date2.month){
		return true;
	}
	else if(date1.month > date2.month){
		return false;
	}
	if (date1.day < date2.day){
		return true;
	}
	else if(date1.day > date2.day){
		return false;
	}
	return true;
}

Date::Date(int y, int m, int d){
    year = y;
    month = m;
    day = d;
}

bool Date::isEmpty() const{
    return (year == 0 && month == 0 && day == 0);
}

Task::Task(bool done, char priority){
	completion_ = done;
	if(priority == '0'){
		priority_ = -1;
	}
	else{
		priority_ = priority - 'A';
	}
}

void Task::switchCompletion(){
	completion_ = !completion_;
	if(completion_ && !(creation_date_.isEmpty())){
		Date date;
		date.currentDate();
		this->setCompletionDate(date);
	}
	else{
		Date date;
		this->setCompletionDate(date);
	}
}

void Task::setCompletionDate(std::string date){
	if (isDate(date)){
		completion_date_ = convertDate(date);
	}
}

void Task::setCreationDate(std::string date){
	if(isDate(date)){
		creation_date_ = convertDate(date);
	}
}

std::ostream& operator<<(std::ostream& os, const Date& date){
	if(date.isEmpty()){
		return os;
	}
	os << date.year << "-";
	if (date.month < 10){
		os << 0;
	}
	os << date.month << "-";
	if (date.day < 10){
			os << 0;
	}
	os << date.day;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Task& task){
	if (task.markedForDeletion()){
		os << "Delete: ";
	}
	if (task.isComplete()){
		os << "x ";
	}
	if (task.getPriority() != -1){
		os << "(" << char('A'+task.getPriority()) << ") ";
	}
	if (!task.getCompletionDate().isEmpty()){
        os << task.getCompletionDate();
        os << " ";
    }
    if (!task.getCreationDate().isEmpty()){
        os << task.getCreationDate();
        os << " ";
    }
	os << task.getText();
    if (task.getProject() != ""){
        os << " +" << task.getProject();
    }
    if (task.getContext() != ""){
        os << " @" << task.getContext();
    }
    return os;
}

bool operator<(const Task& task1, const Task& task2){
	if(task1.markedForDeletion() && !task2.markedForDeletion()){
		return false;
	}
	else if (!task1.markedForDeletion() && task2.markedForDeletion()){
		return true;
	}
	if (task1.isComplete() && !task2.isComplete()){
		return false;
	}
	else if (!task1.isComplete() && task2.isComplete()){
		return true;
	}
	if (task1.getPriority() < task2.getPriority()){
		return true;
	}
	else if (task1.getPriority() > task2.getPriority()){
		return false;
	}
	if(task1.getCompletionDate() < task2.getCompletionDate()){
		return true;
	}
	else if(task2.getCompletionDate() < task1.getCompletionDate()){
		return false;
	}
	if (task1.getCreationDate() < task2.getCreationDate()){
		return true;
	}
	else if(task2.getCreationDate() < task1.getCreationDate()){
		return false;
	}
	return true;
}

void Tasks::addTask(const std::string& line){
	std::vector<std::string> parts;
	splitString(line, ' ', parts);
	size_t begin = 0;
	bool done = false;
	char priority = '0';
	if(parts.size() == 0){
		return;
	}
	if(parts[begin] == "x"){
		done = true;
		++begin;
	}
	if(parts[begin].length() == 3 && parts[begin][0] == '(' && parts[begin][1] >= 'A' && parts[begin][1] <= 'Z' && parts[begin][2] == ')'){
		priority = parts[begin][1];
		++begin;
	}
	Task* temp = new Task(done, priority);
	if(isDate(parts[begin])){
		if (isDate(parts[begin+1])){
			temp->setCreationDate(parts[begin]);
			temp->setCompletionDate(parts[begin+1]);
			begin += 2;
		}
		else{
			temp->setCreationDate(parts[begin]);
			++begin;
		}
	}
	size_t textBeg = begin;
	for(; begin < parts.size(); ++begin){
		if(parts[begin][0] == '@'){
			temp->context() = parts[begin].substr(1, parts[begin].length()-1);
			if(begin == textBeg){
				++textBeg;
			}
			else if(temp->text() == ""){
				bindStrings(parts, textBeg, begin, temp->text());
			}
		}
		else if(parts[begin][0] == '+'){
			temp->project() = parts[begin].substr(1, parts[begin].length()-1);
			if(begin == textBeg){
				++textBeg;
			}
			else if(temp->text() == ""){
				bindStrings(parts, textBeg, begin, temp->text());
			}
		}
	}
	if(temp->text() == ""){
		bindStrings(parts, textBeg, begin, temp->text());
	}
	tasks_.push_back(temp);
}

void Tasks::printTasks(std::ostream& os){
	size_t index = 0;
	for(auto&& task : tasks_){
		if (!task->isComplete() && !task->markedForDeletion()){
			if(index < 10){
				os << "0";
			}
			os << index << ": " << (*task) << std::endl;
		}
		++index;
	}
}

void Tasks::printAllTasks(std::ostream& os){
	size_t index = 0;
	for(auto&& task : tasks_){
		if(index < 10){
			os << "0";
		}
		os << index << ": " << (*task) << std::endl;
		++index;
	}
}

void Tasks::print(std::ostream& os) const{
	for(auto&& task : tasks_){
		if(!task->markedForDeletion()){
			os << (*task) << std::endl;
		}
	}
}

Tasks::~Tasks(){
	for(auto&& task : tasks_){
		delete(task);
	}
}

void Tasks::sort(){
	std::sort(tasks_.begin(), tasks_.end(), predTask());
}

Task& Tasks::at(size_t position){
	if(position < tasks_.size()){
		ChangeTask CT = ChangeTask(tasks_[position]);
		undo_.push(CT);
		return *(tasks_[position]);
	}
	else{
		throw NonExistingItemException(position, tasks_.size() - 1);
	}
}

Task& Tasks::addEmpty(){
	Task* temp = new Task();
	temp->text() = "Newly created task";
	Date date;
	date.currentDate();
	temp->setCreationDate(date);
	tasks_.push_back(temp);
	return *(tasks_[tasks_.size() - 1]);
}

void Tasks::undo(bool print){
	if(undo_.size() > 0){
		if (print){
			std::cout << "From: "<< *(undo_.top().task()) << std::endl;
		}
		ChangeTask CT = ChangeTask(undo_.top().task());
		redo_.push(CT);
		undo_.top().undo();
		undo_.pop();
		if (print){
			std::cout << "To: "<< *(redo_.top().task()) << std::endl;
		}
	}
}

void Tasks::redo(bool print){
	if(redo_.size() > 0){
		if (print){
			std::cout << "From: "<< *(redo_.top().task()) << std::endl;
		}
		ChangeTask CT = ChangeTask(redo_.top().task());
		undo_.push(CT);
		redo_.top().undo();
		redo_.pop();
		if (print){
			std::cout << "To: "<< *(undo_.top().task()) << std::endl;
		}
	}
}

ChangeTask::ChangeTask(Task* task){
	task_ = task;
	markedForDeletion_ = task->markedForDeletion();
	text_ = task->text();
	priority_ = task->getPriority();
	completion_ = task->isComplete();
	completion_date_ = task->getCompletionDate();
	creation_date_ = task->getCreationDate();
	project_tag_ = task->project();
	context_tag_ = task->context();
}

void ChangeTask::undo(){
	task_->setDeletion(markedForDeletion_);
	task_->text() = text_;
	task_->setPriority(priority_ == '0' ? -1 : 'A' + priority_);
	task_->setCompletion(completion_);
	task_->setCompletionDate(completion_date_ );
	task_->setCreationDate(creation_date_);
	task_->project() = project_tag_;
	task_->context() = context_tag_;
}

void Task::setPriority(char pr){
	if (pr == '0'){
		priority_ = -1;
	}
	else{
		priority_ = pr - 'A';
	}
}

Task& Tasks::operator [](size_t index){
	if(index <= tasks_.size()){
		return *tasks_[index];
	}
    else{
        throw NonExistingItemException(index, tasks_.size());
    }
}

Tasks::iterator Tasks::begin(){
		return Tasks::iterator(this, 0);
}
		
Tasks::iterator Tasks::end(){
		return Tasks::iterator(this, tasks_.size());
}

Tasks::iterator::iterator(Tasks* tasks, std::size_t position){
		tasks_ = tasks;
		position_ = position;
}

Task& Tasks::iterator::operator*() const{
		return (*tasks_)[position_];
}
bool Tasks::iterator::operator!=(const Tasks::iterator& other) const{
		return position_ != other.position_;
}
Tasks::iterator& Tasks::iterator::operator++(){
		if(position_ != tasks_->size()){
			position_++;
		}
		return *this;
}

void Tasks::clear(){
	for (auto&& task : tasks_){
		delete task;
	}
	tasks_ = std::vector<Task*>();
	while (!undo_.empty()){
		undo_.pop();
	}
	while (!redo_.empty()){
		redo_.pop();
	}
}

bool Task::match(const std::string& match){
	if (match.length() > 1 && match[0] == '!'){
		return match[1] == ('A' + priority_);
	}
	if (match.length() > 0 && match[0] == '@'){
		return (context_tag_.find(match.substr(1,match.length()-1)) != std::string::npos);
	}
	else if (match.length() > 0 && match[0] == '+'){
		return (project_tag_.find(match.substr(1,match.length()-1)) != std::string::npos);
	}
	else if (match.length() > 0){
		bool context = (context_tag_.find(match) != std::string::npos);
		bool project = (project_tag_.find(match) != std::string::npos);
		bool text = (text_.find(match) != std::string::npos);
		return (context || project || text);
	}
	return true;
}

bool Task::isEmpty(){
	if(text_ == ""){
		return true;
	}
	return false;
}
