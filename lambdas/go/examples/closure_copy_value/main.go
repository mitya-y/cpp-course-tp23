package main

import "fmt"

func call(f func(bool) int) { f(true) }

func main() {
  i := 0
  // 'i' will copy by reference
  f := func(inc bool) int {
    if inc { i++ }
    return i
  }
  g := func(val int) func(bool) int {
    return func(inc bool) int {
      if inc { val++ }
      return val
    }
  }(i)

  call(f)
  fmt.Println(i, f(false), g(false)) // 1, 1, 0
  call(g)
  fmt.Println(i, f(false), g(false)) // 1, 1, 1 
}


