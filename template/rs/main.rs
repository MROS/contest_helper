#![allow(unused_imports)]
#![allow(unused_macros)]
#![allow(dead_code)]
use std::cmp;
use std::collections::{BTreeMap, BTreeSet, BinaryHeap, HashMap, HashSet, VecDeque};
use std::io::{self, stdin, BufRead};
use std::str::FromStr;

fn read_one<T>() -> T
where
    T: FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug,
{
    let stdin = stdin();
    let mut handle = stdin.lock();
    let mut buf = String::new();
    handle.read_line(&mut buf).unwrap();

    buf.trim().parse::<T>().unwrap()
}

macro_rules! read_tuple {
    ( $( $t:ty ), * ) => {
        {
            let stdin = stdin();
            let mut handle = stdin.lock();
            let mut buf = String::new();
            handle.read_line(&mut buf).unwrap();
            let mut v = buf.trim().split_whitespace()
                .collect::<VecDeque<&str>>();
            (
                $({
                    v.pop_front().unwrap().parse::<$t>().unwrap()
                },)*
            )
        }
    }
}

fn read_vec<T>() -> Vec<T>
where
    T: FromStr,
    <T as std::str::FromStr>::Err: std::fmt::Debug,
{
    let stdin = stdin();
    let mut handle = stdin.lock();
    let mut buf = String::new();
    handle.read_line(&mut buf).unwrap();

    buf.trim()
        .split_whitespace()
        .map(|s| s.parse::<T>().unwrap())
        .collect::<Vec<T>>()
}

fn main() -> io::Result<()> {
    let n = read_one::<String>();
    println!("{}", n);

    let (x, y) = read_tuple![i32, u8];
    println!("{} {}", x, y);

    let xs = read_vec::<i32>();
    println!("{:?}", xs);
    Ok(())
}
