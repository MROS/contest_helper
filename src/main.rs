use clap::{App, Arg, SubCommand};
use dirs::home_dir;
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

fn main() -> std::io::Result<()> {
    let template_dir = &home_dir()
        .unwrap()
        .as_path()
        .join(".config")
        .join("ch")
        .join("template");

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

    let filename = matches.value_of("FILE").unwrap();

    let lang = match matches.value_of("language") {
        None => {
            let file_split: Vec<&str> = filename.split(".").collect();
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

    if Path::new(filename).exists() {
        if libs.len() > 0 {
            let mut source = fs::read_to_string(Path::new(filename))?;
            source = insert_lib(&source, template_dir, lang, libs);
            let mut file = fs::File::create(filename)?;
            file.write_all(source.as_bytes())?;
        }
    } else {
        let mut file = fs::File::create(filename)?;
        let mut source = get_template(lang, template_dir);
        source = insert_lib(&source, template_dir, lang, libs);
        file.write_all(source.as_bytes())?;
    }

    Ok(())
}
