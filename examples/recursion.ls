// Typical function declaration that solves for fib of n
func fib(n) {
  if (n <= 1) {
    return n;
  }

  return fib(n - 2) + fib(n - 1);
}

// Starts parsing fib of 10.
print fib(10);
