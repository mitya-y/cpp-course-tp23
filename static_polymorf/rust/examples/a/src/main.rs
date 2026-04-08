trait Animal { fn speak(&self); }
trait SpacialSpeak {}

impl<T: !SpacialSpeak> Animal for T {
    fn speak(&self) { println!("Hello"); }
}

struct Cat;

impl Animal for Cat { fn speak(&self) { println!("Meow!"); } }

fn announce<T: Animal>(animal: T) { animal.speak(); }

fn main() {
  announce(Cat);
}
 

