trait Animal { fn speak(&self); }

struct Dog;
struct Cat;

impl Animal for Dog { fn speak(&self) { println!("Woof!"); } }
impl Animal for Cat { fn speak(&self) { println!("Meow!"); } }

// Wlll be generated 2 functions
fn announce<T: Animal>(animal: T) { animal.speak(); }

fn main() {
  announce(Dog);
  announce(Cat);
}
 

