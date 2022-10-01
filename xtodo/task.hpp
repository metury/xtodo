/// @file task.hpp Headder file for classes Task, Tasks and everything related to them.
#ifndef task_hpp_
#define task_hpp_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include "exception.hpp"

/// Put together strings from vector (start to end) to one string.
/// @param parts String parts in vector.
/// @param start First index of string from vector that will be used.
/// @param end First index of string that won't be used.
/// @param bind New constructed string.
void bindStrings(const std::vector<std::string>& parts, size_t start, size_t end,std::string& bind);

/// Split spring based on given splitter.
/// @param line Which line is going to be split.
/// @param splitter By which character I am going to split.
/// @param parts Where to put all the parts.
void splitString(const std::string& line, const char splitter, std::vector<std::string>& parts);

/// Struct for holding data about given date.
struct Date{ 
	public:
	
		/// Default constructor.
		/// @param y Year.
		/// @param m Month.
		/// @param d Day.
		Date(int y = 0, int m = 0, int d = 0);
        
        /// Whether the date is undefined (all values are equal to 0).
        /// @return True if it is undefined.
        bool isEmpty() const;
		
		/// Number of day.
		int day;
		
		/// Number of month.
		int month;
		
		/// Number of year.
		int year;
};

/// To use default << operator to print date.
/// @param os Which stream to use.
/// @param date Which date wil be putted to the stream.
/// @return Reference to the stream.
std::ostream& operator<<(std::ostream& os, const Date& date);

/// To compare dates between each other.
/// @param date1 First date.
/// @param date2 Second date.
/// @return If the first was earlier.
bool operator<(const Date& date1, const Date& date2);

/// Convert date from string in format y-m-d.
/// @param writtenDate String with this format.
/// @return Newly constructed Date.
Date convertDate(const std::string& writtenDate);

/// Class for holding all properties of task.
class Task{
	public:
	
		/// Default constructor.
		/// @param done If the task was marked as done or not.
		/// @param priority If the task has any given priority.
		Task(bool done = false, char priority = '0');
		
		/// Get reference to the text.
		/// @return Reference to the text.
		inline std::string& text(){
			return text_;
		}
		
		/// Getter for constant reference to the text.
		/// @return Const reference to the text.
		inline const std::string& getText() const{
			return text_;
		}
		
		/// Get the reference to the project tag.
		/// @return Reference to the project tag.
		inline std::string& project(){
			return project_tag_;
		}
		
		/// Getter for constant reference to the project tag.
		/// @return Const reference to the project tag.
		inline const std::string& getProject() const{
			return project_tag_;
		}
		
		/// Get the reference to the context tag.
		/// @return Reference to the context tag.
		inline std::string& context(){
			return context_tag_;
		}
		
		/// Getter for constant reference to the context tag.
		/// @return Const reference to the context tag.
		inline const std::string& getContext() const{
			return context_tag_;
		}
		
		/// Set the deletion for the task.
		/// @param del If the task is to be deleted or not.
		inline void setDeletion(bool del){
			markedForDeletion_ = del;
		}
		
		/// Getter for deletion flag.
		/// @return If the task is set to be deleted.
		inline bool markedForDeletion() const{
			return markedForDeletion_;
		}
		
		/// Switch deletion to the negation of the current state.
		inline void switchDeletion(){
			markedForDeletion_ = !markedForDeletion_;
		}
		
		/// Set the completion of the task.
		/// @param completion Whether the task is going to be completed or not.
		inline void setCompletion(bool completion){
			completion_ = completion;
		}
		
		/// Getter for completion flag.
		/// @return If the task is complete or not.
		inline bool isComplete() const{
			return completion_;
		}
		
		/// Switch completion to its negation of the current state.
		void switchCompletion();
		
		/// Set completion date with date in string format.
		/// @param date The string with writte date.
		void setCompletionDate(std::string date);
		
		/// Set comletion date with already existing date.
		/// @param date Given date.
		inline void setCompletionDate(Date date){
			completion_date_ = date;
		}
		
		/// Getter for constant reference to the completion date.
		/// @return Const reference to the completion date.
		inline const Date& getCompletionDate() const{
			return completion_date_;
		}
		
		/// Set creation date with date in string format.
		/// @param date The string with writte date.
		void setCreationDate(std::string date);
		
		/// Set comletion date with already existing date.
		/// @param date Given date.
		inline void setCreationDate(Date date){
			creation_date_ = date;
		}	

		/// Getter for constant reference to the completion date.
		/// @return Const reference to the completion date.
		inline const Date& getCreationDate() const{
			return creation_date_;
		}
		
		/// Getter for priority in size_t format.
		/// @return Priority.
		inline size_t getPriority() const{
			return priority_;
		}

		/// Setter for priority with given character.
		/// @param pr Priority in char format (A-Z).
		void setPriority(char pr);
		
		/// If the task is matching with given string.
		/// @param match Matching string.
		/// @return If it matches.
		bool match(const std::string& match);
		
		/// If the task is empty.
		/// @return True if it is empty.
		bool isEmpty();
	private:
		
		/// If the task is set to be deleted (not saved).
		bool markedForDeletion_ = false;
		
		/// What is the content of the task.
		std::string text_;
		
		/// What priority does the task have (from A to Z, but in size_t format).
		size_t priority_;
		
		/// If the task is marked as completed or not.
		bool completion_;
		
		/// When the task is set to be done.
		Date completion_date_;
		
		/// When the task was created.
		Date creation_date_;
		
		/// What is the project tag.
		std::string project_tag_;
		
		/// What is the context tag.
		std::string context_tag_;
};

/// << operator for task.
/// @param os Reference to the used stream.
/// @param task Which task will be used.
/// @return Reference to the stream.
std::ostream& operator<<(std::ostream& os, const Task& task);

/// To compare tasks between eachother.
/// @param task1 First tasks.
/// @param task2 Second task.
/// @return If the first task has less "priority" (deletion - done - priority - completion date - creation date).
bool operator<(const Task& task1, const Task& task2);


/// When changing the task make this class to preserve the old task for undo and redo purposes.
class ChangeTask{
	public:
		
		/// Default constructor.
		/// @param task Pointer to the changed task.
		ChangeTask(Task* task);
		
		/// Undo all changes made to the task.
		void undo();
		
		/// Getter for the pointer to the changed task.
		/// @return Pointer to the task.
		inline Task* task(){
			return task_;
		}
	private:
		
		/// Pointer to the changed task.
		Task* task_;
		
		/// If it was marked for deletion.
		bool markedForDeletion_ = false;
		
		/// What was the old content.
		std::string text_;
		
		/// What was its priority.
		size_t priority_;
		
		/// If it was set for completion or not.
		bool completion_;
		
		/// What was its completion date.
		Date completion_date_;
		
		/// What was its creation date.
		Date creation_date_;
		
		/// What was its project tag.
		std::string project_tag_;
		
		/// What was its context tag.
		std::string context_tag_;
};

/// Functor for pointer to task to sort vector with task pointer.
struct predTask{
	
	/// Operator () of the functor.
	/// @param task1 Pointer to first task.
	/// @param task2 Pointer to second task.
	/// @return If the first task has bigger priority.
	bool operator()(Task* task1, Task* task2) const { 
		return *task1 < *task2;
	}
};


/// Class holding all tasks and to work with them.
class Tasks{
	public:
	
		/// Adding task in string format.
		/// @param line Task in string format.
		void addTask(const std::string& line);
		
		/// Print tasks that are not done and not marked for deletion with their index.
		/// @param os Which stream to use.
		void printTasks(std::ostream& os = std::cout);
		
		/// Print all tasks with ther index.
		/// @param os Which stream to use.
		void printAllTasks(std::ostream& os = std::cout);
		
		/// Print all tasks in its base string format.
		/// @param os Which stream to use.
		void print(std::ostream& os) const;
		
		/// Sort all tasks based on their priority.
		void sort();
		
		/// Getter for reference of the task at the given index, also make copy for undo.
		/// @param position What is the index of the task.
		/// @return Reference to the task.
		Task& at(size_t position);
		
		/// Adding new (empty) task.
		/// @return Reference to the newly constructed task.
		Task& addEmpty();
		
		/// Undo last change.
		/// @param print Wheter to print the change to cout or not.
		void undo(bool print = false);
		
		/// Redo last undo.
		/// @param print Whether to print the change to cout or not.
		void redo(bool print = false);
		
		/// How many tasks it has.
		/// @return The size.
		inline size_t size() const{
			return tasks_.size();
		}
		
		/// Get Task on the postion through brackets.
		/// @param index which position.
		/// @return Reference to thatt task if it exists.
		Task& operator[](size_t index);
		
		/// Class for iterator in Tasks.
		class iterator;
		
		/// Begining iterator.
		/// @return Newly constructed iterator pointing to the begining.
		Tasks::iterator begin();
		
		/// Ending iterator.
		/// @return Newly constructed iterator pointing to the end.
		Tasks::iterator end();
		
		/// Default destructor for destructing all tasks.
		~Tasks();
		
		/// Destroy all tasks and change tasks.
		void clear();
		
		/// Check if the last task is empty, if so delete it.
		void checkEmpty();
	private:
		
		/// Where all task are stored. As pointers to easily make new pointers to them.
		std::vector<Task*> tasks_;
		
		/// Stack of last changes.
		std::stack<ChangeTask> undo_;
		
		/// Stack of last undos.
		std::stack<ChangeTask> redo_;
};

/// Class for iterator in Tasks.
class Tasks::iterator{
	friend class Tasks;
	public:
	
		/// What category is this iterator.
		using iterator_category = std::forward_iterator_tag;
		
		/// How to solve difference of these iterators.
		using difference_type = std::ptrdiff_t;
		
		/// What is the type it is iterating.
		using value_type = Task;
		
		/// What is the pointer.
		using pointer = Task*;
		
		/// What is the reference.
		using reference = Task&;
		
		/// Basic constructor.
		/// @param tasks Which tasks it is bounded to.
		/// @param position At which position it is looking.
		iterator(Tasks* tasks, std::size_t position);
		
		/// To use * operator with the iterator.
		/// @return reference to that Task.
		Task& operator*() const;
		
		/// How to compare two iterators.
		/// @param other Second iterator.
		/// @return If they are different.
		bool operator!=(const iterator& other) const;
		
		/// Default operator for incrementation.
		/// @return Reference to changed iterator.
		iterator& operator++();
	private:
	
		/// Pointer to parent Tasks.
		Tasks* tasks_;
		
		/// At which position is the iterator looking.
		size_t position_;
};

#endif
