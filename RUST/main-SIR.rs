use std::process;
use std::env;
use std::fs::File;
use std::io::Write;
//rustc  main-SIR.rs -o SIR
fn main() {
    let args: Vec<String> = env::args().collect();
    let beta:f64 = args[1].parse().unwrap(); //1.1;
    let mi:f64 = args[2].parse().unwrap(); //0.1;
    let h:f64 = args[1].parse().unwrap(); //0.5;
    let mut s1:f64 ;
    let mut s0:f64 = 1.0;
    let mut i1:f64 ;
    let mut i0:f64 = args[1].parse().unwrap(); //1E-2;
    let mut r1:f64 ;
    let mut r0:f64 = 0.0;
    let timestep:i64 = 100;
    let mut log = File::create("Log-RUST.txt").expect("Unable to create");

    println!("SIR model - network information spreading");

    //let arg1 = &args[1];
    //println!("{arg1}");
    let v:f64 =
    println!("{v}");
/*
    for _ts in 1..timestep{
        s1 = s0 + h * (-beta * s0 * i0);
        i1 = i0 + h * ((beta * s0 * i0) - (mi * i0));
        r1 = r0 + h * (mi * i0);
        //let txt = _ts.to_string() + " " + s1.to_string()  + " " + i1.to_string() + " " + r1.to_string();
        let txt = format!("{} {:.20} {:.20} {:.20} \n", _ts, s1, i1, r1);
        //"{_ts} {s1} {i1} {r1} ";
        log.write_all(txt.as_bytes()).expect("Unable to write");
        //println!("{_ts} {s1} {i1} {r1} ");
        s0 = s1; i0 = i1; r0 = r1;
    }

*/


    process::exit(0);
}
