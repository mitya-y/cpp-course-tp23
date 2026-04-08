package main

import "fmt"


func get_generator() func() int {
  i := 0
  // 'i' will copy by reference
  return func() int {
    i++
    return i
  }
}

func main() {
  a := get_generator()
  fmt.Println(a()) // 1
  fmt.Println(a()) // 2

  b := get_generator()
  fmt.Println(b()) // 1
}

