use clap::{App, Arg, SubCommand};
use std::fs;
use std::io::prelude::*;
use std::path::Path;

// TODO: 改用 "?"
fn get_template(lang: &str, dir: &Path) -> String {
    for entry in fs::read_dir(dir).unwrap() {
        let path = entry.unwrap().path();
        if path.to_str().unwrap().ends_with(lang) {
            return fs::read_to_string(path).unwrap();
        }
    }
    return "".to_owned();
}

fn main() {
    let template_dir = Path::new("/home/mros/bin/contest_helper/template/");

    let matches = App::new("contest_helper")
        .arg(Arg::with_name("language").short("l").takes_value(true))
        .arg(Arg::with_name("FILE").required(true).index(1))
        .subcommand(SubCommand::with_name("dir"))
        .get_matches();

    let lang = match matches.value_of("language") {
        None => "cpp",
        Some(l) => l,
    };

    let file = matches.value_of("FILE").unwrap();

    let mut file = fs::File::create(file).unwrap();
    file.write_all(get_template(lang, template_dir).as_bytes())
        .unwrap();
}
