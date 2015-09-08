use std::io;
use std::str::Split;

struct URI {
    raw_uri: String,
    original_uri: String,
}

impl URI {
    pub fn new(uri: String) -> URI {
        URI {
            raw_uri: uri.clone(),
            original_uri: String::new(),
        }
    }

    pub fn get_original_uri(&self) -> String {
        let mut original_uri: Vec<u8> = Vec::new();

        let mut tokens = self.raw_uri.split("%");

        for tokenbytes in tokens.next().unwrap().as_bytes() {
            original_uri.push(*tokenbytes);
        }

        //percent-encoded tokens.
        for token in tokens {
            let trimmed = token.trim();

            //Sanitize before slice. Prepare for unsafe world!
            if(trimmed.len() < 2) {
                panic!("Malformed URI!");
            }

            //Slice first 2 byte.
            //WARN: UNSAFE ROUTINE CAN CRASH APPLICATION.
            let sliced = unsafe {
                trimmed.slice_unchecked(0, 2)
            };

            let leftover = unsafe {
                trimmed.slice_unchecked(2, trimmed.len())
            };

            let byte = u8::from_str_radix(sliced, 16).unwrap();
            original_uri.push(byte);

            for leftbyte in leftover.as_bytes() {
                original_uri.push(*leftbyte);
            }
        }

        unsafe {
            String::from_utf8_unchecked(original_uri)
        }.to_string()
    }
}

fn main() {
    let mut reader = io::stdin();
    //Read first line. It contains Line numbers to read.

    let mut line = String::new();
    reader.read_line(&mut line).ok().expect("Failed to read stdin!");

    let line_count = match line.trim().parse::<i32>().ok() {
        Some(input_int) => input_int,
        None => {
            panic!("Input is not a number!");
        }
    };

    //Now, allocate new vector to store URI data.
    let mut raw_uris: Vec<URI> =
        Vec::with_capacity(line_count as usize);

    for i in 0..line_count {
        let mut uri_string = String::new();
        reader.read_line(&mut uri_string).ok()
            .expect("Failed to read stdin!");

        let mut uri = URI::new(uri_string.trim().to_string());
        raw_uris.push(uri);
    }

    //Iterate and convert and print it.
    for uri in raw_uris.iter() {
        println!("{}", uri.get_original_uri());
    }
}
