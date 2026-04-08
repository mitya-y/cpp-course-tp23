trait Base<T: Base<T>> {
    fn interface(&self) {
        self._impl_method();
    }
    fn _impl_method(&self);
}

struct Derived;

impl Base<Derived> for Derived {
    fn _impl_method(&self) {
        println!("Derived implementation");
    }
}

fn main() {
    let d = Derived;
    d.interface(); 
}
