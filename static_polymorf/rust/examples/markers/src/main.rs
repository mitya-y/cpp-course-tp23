use std::rc::Rc;

fn is_send<T: Send>() {}

fn main() {
  is_send::<i32>();            // OK
  is_send::<String>();         // OK
  // is_send::<Rc<i32>>();     // Error: Rc is not Send
}
