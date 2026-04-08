// go mod init example/escape
// go test -gcflags="-m" -bench .

package main

func EscapingClosure() func() int {
  x := 42
  return func() int { // возвращаем замыкание наружу → x убегает в кучу
   x++
   return x
  }
}

func NoEscapingClosure() int {
  y := 42
  return func() int { // возвращаем замыкание наружу → x убегает в кучу
    return y
  }()
}


