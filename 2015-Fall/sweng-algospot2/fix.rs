use std::io;

struct Counter {
    raw_data: Vec<i32>,
}

impl Counter {
    pub fn new(data: Vec<i32>) -> Counter {
        Counter {
            raw_data: data.clone(),
        }
    }

    pub fn do_count(&self) -> i32 {
        let mut sorted_data = self.raw_data.clone();

        sorted_data.sort();

        let mut idx: i32 = 0;
        let mut count = 0;
        for i in sorted_data.iter() {
            let raw_data_val = match self.raw_data.get(idx as usize) {
                Some(input_int) => input_int,
                None => {
                    panic!("Something goes wrong!");
                }
            };

            if(i == raw_data_val) {
                count += 1;
            }

            idx += 1;
        }

        count
    }
}

fn main() {
    let mut reader = io::stdin();

    let mut line = String::new();
    reader.read_line(&mut line).ok().expect("Failed to read stdin!");

    let data_count = match line.trim().parse::<i32>().ok() {
        Some(input_int) => input_int,
        None => {
            panic!("Input is not a number!");
        }
    };

    let mut counters: Vec<Counter> =
        Vec::with_capacity(data_count as usize);

    for i in 0..data_count {
        let mut count_string = String::new();
        let mut data_string = String::new();

        reader.read_line(&mut count_string).ok()
            .expect("Failed to read stdin!");

        reader.read_line(&mut data_string).ok()
            .expect("Failed to read stdin!");

        let count = match count_string.trim().parse::<i32>().ok() {
            Some(input_int) => input_int,
            None => {
                panic!("Input is not a number!");
            }
        };

        data_string = data_string.trim().to_string();

        let mut tokens = data_string.split(" ");

        let mut datavec: Vec<i32> = Vec::with_capacity(count as usize);
        for token in tokens {
            let seq_elem = match token.parse::<i32>().ok() {
                Some(input_int) => input_int,
                None => {
                    panic!("Sequence is not a number!");
                }
            };

            datavec.push(seq_elem);
        }

        if(datavec.len() != count as usize) {
            panic!("Data length does not match with given data size number!");
        }

        let mut counter = Counter::new(datavec);
        counters.push(counter);
    }

    for counter in counters.iter() {
        println!("{}", counter.do_count());
    }
}

