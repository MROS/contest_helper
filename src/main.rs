use clap::{App, Arg, SubCommand};
use std::fs;
use std::io::prelude::*;
use std::path::Path;

// TODO: 改用 "?"
fn get_template(lang: &str, dir: &Path) -> String {
    let path = dir.join(lang).join(format!("main.{}", lang));
    return fs::read_to_string(path).unwrap();
}

fn insert_lib(source: &str, dir: &Path, lang: &str, libs: Vec<&str>) -> String {
    let mut lines: Vec<String> = source.split("\n").map(|s| s.to_string()).collect();
    let mut pos = 0;
    for i in 0..lines.len() {
        if lines[i].contains("插入此處") {
            pos = i;
            break;
        }
    }
    for lib in libs {
        let lib_path = dir.join(lang).join(format!("{}.{}", lib, lang));
        let lib_str = fs::read_to_string(lib_path).unwrap();
        lines.insert(pos + 1, lib_str);
    }
    lines.join("\n")
}

fn main() {
    let template_dir = Path::new("/home/mros/bin/contest_helper/template/");

    let matches = App::new("contest_helper")
        .arg(Arg::with_name("language").long("lang").takes_value(true))
        .arg(
            Arg::with_name("libs")
                .long("libs")
                .multiple(true)
                .takes_value(true),
        )
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

    let libs: Vec<&str> = match matches.values_of("libs") {
        Some(libs) => libs.collect(),
        None => vec![],
    };

    let mut file = fs::File::create(file).unwrap();
    let mut source = get_template(lang, template_dir);
    source = insert_lib(&source, template_dir, lang, libs);
    file.write_all(source.as_bytes()).unwrap();
}
