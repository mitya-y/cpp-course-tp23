trait Container {
    type Item;
    fn get(&self) -> Option<&Self::Item>;
}

struct VecWrapper<T> {
    data: Vec<T>,
}
impl<T> Container for VecWrapper<T> {
    type Item = T;
    fn get(&self) -> Option<&Self::Item> {
        self.data.first()
    }
}
 
fn main() {
}
 
