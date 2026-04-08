fn greeting() -> &'static str {
    #[cfg(feature = "verbose")]
    {
        #[cfg(target_os = "linux")] { return "linux"; }
        #[cfg(target_os = "windows")] { return "windows"; }
        #[cfg(not(any(target_os = "linux", target_os = "windows")))] { return "unknown"; }
    }
    #[cfg(not(feature = "verbose"))]
    { return ""; }
}
 
fn main() {
    println!("hello {}", greeting());
}
 
