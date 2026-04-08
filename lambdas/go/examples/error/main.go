package main
import (
	"fmt"
	"sync"
)

func main() {
	var wg sync.WaitGroup
	for i := 0; i < 15; i++ {
		wg.Add(1)
		go func() {
			fmt.Print(i)
      wg.Done()
		}()
	}
	wg.Wait()
}
