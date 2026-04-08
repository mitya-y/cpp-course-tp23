// rustup override set nightly
#![feature(specialization)]

trait Animal {
    fn speak(&self);
}

// Default specialization
impl<T> Animal for T {
    default fn speak(&self) {
        println!("Hello");
    }
}

struct Cat;
struct Dog;

// specialization for Cat
impl Animal for Cat {
    fn speak(&self) {
        println!("Meow!");
    }
}

fn announce<T: Animal>(animal: T) {
    animal.speak();
}

fn main() {
    announce(Cat); // Meow!
    announce(Dog); // Hello
}

