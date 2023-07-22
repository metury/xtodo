/// @file task.hpp Header file for classes Task, Tasks and everything related to them.
#ifndef task_hpp_
#define task_hpp_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <ctime>
#include "exception.hpp"

/// Put together strings from vector (start to end) to one string.
/// @param parts String parts in vector.
/// @param start First index of string from vector that will be used.
/// @param end First index of string that won't be used.
/// @param bind New constructed string.
void bind_strings(const std::vector<std::string>& parts, size_t start, size_t end, std::string& bind);

/// Split spring based on given splitter.
/// @param line Which line is going to be split.
/// @param splitter By which character I am going to split.
/// @param parts Where to put all the parts.
void split_string(const std::string& line, const char splitter, std::vector<std::string>& parts);

/// Holding data about a single date.
struct date{
		/// Default constructor.
		/// @param y Year.
		/// @param m Month.
		/// @param d Day.
		date(int y = 0, int m = 0, int d = 0);
		/// Make this date current.
		void current_date();
        /// Whether the date is undefined (all values are equal to 0).
        /// @return True if it is undefined.
        bool is_empty() const;
		/// Number of day.
		int day;
		/// Number of month.
		int month;
		/// Number of year.
		int year;
};

/// To use default << operator to print date.
/// @param os Which stream to use.
/// @param date Which date will be putted to the stream.
/// @return Reference to the stream.
std::ostream& operator<<(std::ostream& os, const date& date);

/// To compare dates between each other.
/// @param date1 First date.
/// @param date2 Second date.
/// @return If the first was earlier.
bool operator<(const date& date1, const date& date2);

/// If the string represents a day.
/// @param text Written day.
/// @return If it is indeed a date.
bool is_date(const std::string& text);

/// Convert date from string in format y-m-d.
/// @param writtenDate String with this format.
/// @return Newly constructed Date.
date convert_date(const std::string& writtenDate);

/// Class for holding all properties of task.
class task{
	public:
		/// Default constructor.
		/// @param done If the task was marked as done or not.
		/// @param priority If the task has any given priority.
		task(bool done = false, char priority = '0');
		/// Get reference to the text.
		/// @return Reference to the text.
		inline std::string& set_text(){return text_;}
		/// Getter for constant reference to the text.
		/// @return Constant reference to the text.
		inline const std::string& get_text() const{return text_;}
		/// Get the reference to the project tag.
		/// @return Reference to the project tag.
		inline std::string& set_project(){return project_tag_;}
		/// Getter for constant reference to the project tag.
		/// @return Const reference to the project tag.
		inline const std::string& get_project() const{return project_tag_;}
		/// Get the reference to the context tag.
		/// @return Reference to the context tag.
		inline std::string& set_context(){return context_tag_;}
		/// Getter for constant reference to the context tag.
		/// @return Const reference to the context tag.
		inline const std::string& get_context() const{return context_tag_;}
		/// Set the deletion for the task.
		/// @param del If the task is to be deleted or not.
		inline void set_deletion(bool del){marked_for_deletion_ = del;}
		/// Getter for deletion flag.
		/// @return If the task is set to be deleted.
		inline bool marked_for_deletion() const{return marked_for_deletion_;}
		/// Switch deletion to the negation of the current state.
		inline void switch_deletion(){marked_for_deletion_ = !marked_for_deletion_;}
		/// Set the completion of the task.
		/// @param completion Whether the task is going to be completed or not.
		inline void set_completion(bool completion){completion_ = completion;}
		/// Getter for completion flag.
		/// @return If the task is complete or not.
		inline bool is_complete() const{return completion_;}
		/// Switch completion to its negation of the current state.
		void switch_completion();
		/// Set completion date with date in string format.
		/// @param date The string with writte date.
		void set_completion_date(std::string& date);
		/// Set comletion date with already existing date.
		/// @param date Given date.
		inline void set_completion_date(date date){completion_date_ = date;}
		/// Getter for constant reference to the completion date.
		/// @return Const reference to the completion date.
		inline const date& get_completion_date() const{return completion_date_;}
		/// Set creation date with date in string format.
		/// @param date The string with writte date.
		void set_creation_date(std::string& date);
		/// Set comletion date with already existing date.
		/// @param date Given date.
		inline void set_creation_date(date date){creation_date_ = date;}
		/// Getter for constant reference to the completion date.
		/// @return Const reference to the completion date.
		inline const date& get_creation_date() const{return creation_date_;}
		/// Getter for priority in size_t format.
		/// @return Priority.
		inline int get_priority() const{return priority_;}
		/// Setter for priority with given character.
		/// @param pr Priority in char format (A-Z).
		void set_priority(char pr);
		/// If the task is matching with given string.
		/// @param match Matching string.
		/// @return If it matches.
		bool match(const std::string& match);
		/// If the task is empty.
		/// @return True if it is empty.
		bool is_empty();
	private:
		/// If the task is set to be deleted (not saved).
		bool marked_for_deletion_ = false;
		/// What is the content of the task.
		std::string text_;
		/// What priority does the task have (from A to Z).
		int priority_;
		/// If the task is marked as completed or not.
		bool completion_;
		/// When the task is set to be done.
		date completion_date_;
		/// When the task was created.
		date creation_date_;
		/// What is the project tag.
		std::string project_tag_;
		/// What is the context tag.
		std::string context_tag_;
};

/// << operator for task.
/// @param os Reference to the used stream.
/// @param task Which task will be used.
/// @return Reference to the stream.
std::ostream& operator<<(std::ostream& os, const task& task);

/// To compare tasks between eachother.
/// @param task1 First tasks.
/// @param task2 Second task.
/// @return If the first task has less "priority" (deletion - done - priority - completion date - creation date).
bool operator<(const task& task1, const task& task2);


/// When changing the task make this class to preserve the old task for undo and redo purposes.
class change_task{
	public:
		/// Default constructor.
		/// @param task Pointer to the changed task.
		change_task(task* t);
		/// Undo all changes made to the task.
		void undo();
		/// Getter for the pointer to the changed task.
		/// @return Pointer to the task.
		inline task* get_task(){return task_;}
	private:
		/// Pointer to the changed task.
		task* task_;
		/// If it was marked for deletion.
		bool marked_for_deletion_ = false;
		/// What was the old content.
		std::string text_;
		/// What was its priority.
		int priority_;
		/// If it was set for completion or not.
		bool completion_;
		/// What was its completion date.
		date completion_date_;
		/// What was its creation date.
		date creation_date_;
		/// What was its project tag.
		std::string project_tag_;
		/// What was its context tag.
		std::string context_tag_;
};

/// Functor for pointer to task to sort vector with task pointer.
struct pred_task{

	/// Operator () of the functor.
	/// @param task1 Pointer to first task.
	/// @param task2 Pointer to second task.
	/// @return If the first task has bigger priority.
	bool operator()(task* task1, task* task2) const {
		return *task1 < *task2;
	}
};


/// Class holding all tasks and to work with them.
class tasks{
	public:
		/// Adding task in string format.
		/// @param line Task in string format.
		void add_task(const std::string& line);
		/// Print tasks that are not done and not marked for deletion with their index.
		/// @param os Which stream to use.
		void print_tasks(std::ostream& os = std::cout);
		/// Print all tasks with ther index.
		/// @param os Which stream to use.
		void print_all_Tasks(std::ostream& os = std::cout);
		/// Print all tasks in its base string format.
		/// @param os Which stream to use.
		void print(std::ostream& os) const;
		/// Sort all tasks based on their priority.
		void sort();
		/// Getter for reference of the task at the given index, also make copy for undo.
		/// @param position What is the index of the task.
		/// @return Reference to the task.
		task& at(size_t position);
		/// Adding new (empty) task.
		/// @return Reference to the newly constructed task.
		task& add_empty();
		/// Undo last change.
		/// @param print Wheter to print the change to cout or not.
		void undo(bool print = false);
		/// Redo last undo.
		/// @param print Whether to print the change to cout or not.
		void redo(bool print = false);
		/// How many tasks it has.
		/// @return The size.
		inline size_t size() const{return tasks_.size();}
		/// Get Task on the postion through brackets.
		/// @param index which position.
		/// @return Reference to thatt task if it exists.
		task& operator[](size_t index);
		/// Class for iterator in Tasks.
		class iterator;
		/// Begining iterator.
		/// @return Newly constructed iterator pointing to the begining.
		tasks::iterator begin();
		/// Ending iterator.
		/// @return Newly constructed iterator pointing to the end.
		tasks::iterator end();
		/// Default destructor for destructing all tasks.
		~tasks();
		/// Destroy all tasks and change tasks.
		void clear();
	private:
		/// Where all task are stored. As pointers to easily make new pointers to them.
		std::vector<task*> tasks_;
		/// Stack of last changes.
		std::stack<change_task> undo_;
		/// Stack of last undos.
		std::stack<change_task> redo_;
};

/// Class for iterator in Tasks.
class tasks::iterator{
	friend class tasks;
	public:
		/// What category is this iterator.
		using iterator_category = std::forward_iterator_tag;
		/// How to solve difference of these iterators.
		using difference_type = std::ptrdiff_t;
		/// What is the type it is iterating.
		using value_type = task;
		/// What is the pointer.
		using pointer = task*;
		/// What is the reference.
		using reference = task&;
		/// Basic constructor.
		/// @param tasks Which tasks it is bounded to.
		/// @param position At which position it is looking.
		iterator(tasks* tasks, std::size_t position);
		/// To use * operator with the iterator.
		/// @return reference to that Task.
		task& operator*() const;
		/// How to compare two iterators.
		/// @param other Second iterator.
		/// @return If they are different.
		bool operator!=(const iterator& other) const;
		/// Default operator for incrementation.
		/// @return Reference to changed iterator.
		iterator& operator++();
	private:
		/// Pointer to parent Tasks.
		tasks* tasks_;
		/// At which position is the iterator looking.
		size_t position_;
};

#endif
