package main

import "fmt"
import "slices"

func main() {
  words := []string{"aa", "aaa", "a"}
  slices.SortFunc(words, func(a, b string) int {
      return len(a) - len(b)
  })
  fmt.Println(words) // [a aa aaa]
}

