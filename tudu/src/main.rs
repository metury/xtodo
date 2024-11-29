use std::collections::HashMap;
use regex::Regex;

struct Task {
    completion: bool,
    priority: u16,
    creation_date: i64,
    completion_date: i64,
    text: String,
    project: String,
    context: String,
    other_values: HashMap<String, String>,
    deletion: bool,
}

impl Task {
    fn new(text: String) -> Task {
    }
}


fn parse(line: &String) -> Option<Task> {
    None
}


fn main() {
    println!("Hello, world!");
}
