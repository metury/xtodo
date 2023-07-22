/// @file Source file for classes Task, Tasks and everything related to them.
#include "task.hpp"

void bind_strings(const std::vector<std::string>& parts, size_t start, size_t end, std::string& bind){
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

void split_string(const std::string& line, const char splitter, std::vector<std::string>& parts){
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

// ++ Date ++

bool is_date(const std::string& text){
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

date convert_date(const std::string& writtenDate){
    std::vector<std::string> parts;
    split_string(writtenDate, '-', parts);
    int year = std::stoi(parts[0]);
    int month = std::stoi(parts[1]);
    int day = std::stoi(parts[2]);
    if (month > 12 || day > 31){
		year = month = day =0;
	}
    return {year, month, day};
}

void date::current_date(){
	std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    year = (now->tm_year + 1900);
    month = (now->tm_mon + 1);
    day = now->tm_mday;
}

bool operator<(const date& date1, const date& date2){
	if(date1.is_empty() && !date2.is_empty()) return false;
	else if(!date1.is_empty() && date2.is_empty()) return true;

	if (date1.year < date2.year) return true;
	else if(date1.year > date2.year) return false;

	if (date1.month < date2.month) return true;
    else if(date1.month > date2.month) return false;

	if (date1.day < date2.day) return true;
	else if(date1.day > date2.day) return false;
	return true;
}

date::date(int y, int m, int d) : day(d), month(m), year(y){}

bool date::is_empty() const{
    return (year == 0 && month == 0 && day == 0);
}

task::task(bool done, char priority) : completion_(done){
	if(priority == '0')
		priority_ = -1;
	else
		priority_ = priority - 'A';
}

void task::switch_completion(){
	completion_ = !completion_;
	date date;
	if(completion_ && !(creation_date_.is_empty()))
		date.current_date();
    set_completion_date(date);
}

void task::set_completion_date(std::string& date){
	if (is_date(date)){
		completion_date_ = convert_date(date);
	}
}

void task::set_creation_date(std::string& date){
	if(is_date(date)){
		creation_date_ = convert_date(date);
	}
}

std::ostream& operator<<(std::ostream& os, const date& date){
	if(date.is_empty()){
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

std::ostream& operator<<(std::ostream& os, const task& task){
	if (task.marked_for_deletion()){
		os << "Delete: ";
	}
	if (task.is_complete()){
		os << "x ";
	}
	if (task.get_priority() != -1){
		os << "(" << char('A'+task.get_priority()) << ") ";
	}
	if (!task.get_completion_date().is_empty()){
        os << task.get_completion_date();
        os << " ";
    }
    if (!task.get_creation_date().is_empty()){
        os << task.get_creation_date();
        os << " ";
    }
	os << task.get_text();
    if (task.get_project() != ""){
        os << " +" << task.get_project();
    }
    if (task.get_context() != ""){
        os << " @" << task.get_context();
    }
    return os;
}

bool operator<(const task& task1, const task& task2){
	if(task1.marked_for_deletion() && !task2.marked_for_deletion()){
		return false;
	}
	else if (!task1.marked_for_deletion() && task2.marked_for_deletion()){
		return true;
	}
	if (task1.is_complete() && !task2.is_complete()){
		return false;
	}
	else if (!task1.is_complete() && task2.is_complete()){
		return true;
	}
	if (task1.get_priority() < task2.get_priority()){
		return true;
	}
	else if (task1.get_priority() > task2.get_priority()){
		return false;
	}
	if(task1.get_completion_date() < task2.get_completion_date()){
		return true;
	}
	else if(task2.get_completion_date() < task1.get_completion_date()){
		return false;
	}
	if (task1.get_creation_date() < task2.get_creation_date()){
		return true;
	}
	else if(task2.get_creation_date() < task1.get_creation_date()){
		return false;
	}
	return true;
}

void tasks::add_task(const std::string& line){
	std::vector<std::string> parts;
	split_string(line, ' ', parts);
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
	task* temp = new task(done, priority);
	if(is_date(parts[begin])){
		if (is_date(parts[begin+1])){
			temp->set_creation_date(parts[begin]);
			temp->set_completion_date(parts[begin+1]);
			begin += 2;
		}
		else{
			temp->set_creation_date(parts[begin]);
			++begin;
		}
	}
	size_t textBeg = begin;
	for(; begin < parts.size(); ++begin){
		if(parts[begin][0] == '@'){
			temp->set_context() = parts[begin].substr(1, parts[begin].length()-1);
			if(begin == textBeg){
				++textBeg;
			}
			else if(temp->set_text() == ""){
				bind_strings(parts, textBeg, begin, temp->set_text());
			}
		}
		else if(parts[begin][0] == '+'){
			temp->set_project() = parts[begin].substr(1, parts[begin].length()-1);
			if(begin == textBeg){
				++textBeg;
			}
			else if(temp->set_text() == ""){
				bind_strings(parts, textBeg, begin, temp->set_text());
			}
		}
	}
	if(temp->set_text() == ""){
		bind_strings(parts, textBeg, begin, temp->set_text());
	}
	tasks_.push_back(temp);
}

void tasks::print_tasks(std::ostream& os){
	size_t index = 0;
	for(auto&& task : tasks_){
		if (!task->is_complete() && !task->marked_for_deletion()){
			if(index < 10){
				os << "0";
			}
			os << index << ": " << (*task) << std::endl;
		}
		++index;
	}
}

void tasks::print_all_Tasks(std::ostream& os){
	size_t index = 0;
	for(auto&& task : tasks_){
		if(index < 10){
			os << "0";
		}
		os << index << ": " << (*task) << std::endl;
		++index;
	}
}

void tasks::print(std::ostream& os) const{
	for(auto&& task : tasks_){
		if(!task->marked_for_deletion()){
			os << (*task) << std::endl;
		}
	}
}

tasks::~tasks(){
	for(auto&& task : tasks_){
		delete(task);
	}
}

void tasks::sort(){
	std::sort(tasks_.begin(), tasks_.end(), pred_task());
}

task& tasks::at(size_t position){
	if(position < tasks_.size()){
		change_task CT = change_task(tasks_[position]);
		undo_.push(CT);
		return *(tasks_[position]);
	}
	else{
		throw NonExistingItemException(position, tasks_.size() - 1);
	}
}

task& tasks::add_empty(){
	task* temp = new task();
	temp->set_text() = "Newly created task";
	date date;
	date.current_date();
	temp->set_creation_date(date);
	tasks_.push_back(temp);
	return *(tasks_[tasks_.size() - 1]);
}

void tasks::undo(bool print){
	if(undo_.size() > 0){
		if (print){
			std::cout << "From: "<< *(undo_.top().get_task()) << std::endl;
		}
		change_task CT = change_task(undo_.top().get_task());
		redo_.push(CT);
		undo_.top().undo();
		undo_.pop();
		if (print){
			std::cout << "To: "<< *(redo_.top().get_task()) << std::endl;
		}
	}
}

void tasks::redo(bool print){
	if(redo_.size() > 0){
		if (print){
			std::cout << "From: "<< *(redo_.top().get_task()) << std::endl;
		}
		change_task CT = change_task(redo_.top().get_task());
		undo_.push(CT);
		redo_.top().undo();
		redo_.pop();
		if (print){
			std::cout << "To: "<< *(undo_.top().get_task()) << std::endl;
		}
	}
}

change_task::change_task(task* t) : task_(t){
	marked_for_deletion_ = t->marked_for_deletion();
	text_ = t->get_text();
	priority_ = t->get_priority();
	completion_ = t->is_complete();
	completion_date_ = t->get_completion_date();
	creation_date_ = t->get_creation_date();
	project_tag_ = t->get_project();
	context_tag_ = t->get_context();
}

void change_task::undo(){
	task_->set_deletion(marked_for_deletion_);
	task_->set_text() = text_;
	task_->set_priority(priority_ == '0' ? -1 : 'A' + priority_);
	task_->set_completion(completion_);
	task_->set_completion_date(completion_date_ );
	task_->set_creation_date(creation_date_);
	task_->set_project() = project_tag_;
	task_->set_context() = context_tag_;
}

void task::set_priority(char pr){
	if (pr == '0'){
		priority_ = -1;
	}
	else{
		priority_ = pr - 'A';
	}
}

task& tasks::operator [](size_t index){
	if(index <= tasks_.size()){
		return *tasks_[index];
	}
    else{
        throw NonExistingItemException(index, tasks_.size());
    }
}

tasks::iterator tasks::begin(){
		return tasks::iterator(this, 0);
}

tasks::iterator tasks::end(){
		return tasks::iterator(this, tasks_.size());
}

tasks::iterator::iterator(tasks* tasks, std::size_t position){
		tasks_ = tasks;
		position_ = position;
}

task& tasks::iterator::operator*() const{
		return (*tasks_)[position_];
}
bool tasks::iterator::operator!=(const tasks::iterator& other) const{
		return position_ != other.position_;
}
tasks::iterator& tasks::iterator::operator++(){
		if(position_ != tasks_->size()){
			position_++;
		}
		return *this;
}

void tasks::clear(){
	for (auto&& task : tasks_){
		delete task;
	}
	tasks_ = std::vector<task*>();
	while (!undo_.empty()){
		undo_.pop();
	}
	while (!redo_.empty()){
		redo_.pop();
	}
}

bool task::match(const std::string& match){
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
		bool set_context = (context_tag_.find(match) != std::string::npos);
		bool set_project = (project_tag_.find(match) != std::string::npos);
		bool set_text = (text_.find(match) != std::string::npos);
		return (set_context || set_project || set_text);
	}
	return true;
}

bool task::is_empty(){
	if(text_ == ""){
		return true;
	}
	return false;
}
