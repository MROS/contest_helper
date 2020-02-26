use clap::{App, Arg, SubCommand};
use std::fs;
use std::io::prelude::*;
use std::path::Path;

// TODO: 改用 "?"
fn get_template(lang: &str, dir: &Path) -> String {
    let path = dir.join(lang).join(format!("main.{}", lang));
    println!("{:?}", path);
    return fs::read_to_string(path).unwrap();
}

fn main() {
    let template_dir = Path::new("/home/mros/bin/contest_helper/template/");

    let matches = App::new("contest_helper")
        .arg(Arg::with_name("language").short("l").takes_value(true))
        .arg(Arg::with_name("FILE").required(true).index(1))
        .subcommand(SubCommand::with_name("dir"))
        .get_matches();

    let file = matches.value_of("FILE").unwrap();

    let lang = match matches.value_of("language") {
        None => {
            let file_split: Vec<&str> = file.split(".").collect();
            if file_split.len() > 1 {
                file_split[file_split.len() - 1]
            } else {
                "cpp" // 沒有副檔名，就當 c++ 了
            }
        }
        Some(l) => l,
    };

    let mut file = fs::File::create(file).unwrap();
    file.write_all(get_template(lang, template_dir).as_bytes())
        .unwrap();
}
