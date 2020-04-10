use dirs::home_dir;
use serde::{Deserialize, Serialize};
use std::fs;
use std::path::Path;
// use toml::Value::

#[derive(Deserialize, Serialize)]
pub struct Config {
    pub dir: Vec<String>,
    pub default: Default_,
}

#[derive(Deserialize, Serialize)]
pub struct Default_ {
    pub language: String,
    pub main: String,
}

pub fn read_config() -> Config {
    let config_file = &home_dir()
        .unwrap()
        .as_path()
        .join(".config")
        .join("ch")
        .join("config.toml");
    if Path::new(config_file).exists() {
        let s = fs::read_to_string(config_file).unwrap();
        toml::from_str(&s).unwrap()
    } else {
        let default_config = Config {
            dir: vec!["A", "B", "C", "D", "E", "F", "G"]
                .into_iter()
                .map(|s| s.to_string())
                .collect(),
            default: Default_ {
                language: "cpp".to_string(),
                main: "main".to_string(),
            },
        };
        fs::write(config_file, toml::to_string(&default_config).unwrap()).unwrap();
        default_config
    }
}
