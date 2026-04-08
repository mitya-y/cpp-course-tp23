trait Animal { fn speak(&self); }

impl<T> Animal for T {
    fn speak(&self) { println!("Hello"); }
}

struct Cat;

fn announce<T: Animal>(animal: T) { animal.speak(); }

fn main() {
  announce(Cat);
}
 

