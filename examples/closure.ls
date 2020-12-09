// closure wraps function and keeps one variable named counter for internal
// state.
func Closure() {
    var counter = 0;

    func Increment() {
        counter = counter + 1;
        return counter;
    }

    return Increment;
}

// Gets a function from the closure back.
var counter = Closure();

// Calling counter() increments the counter contained within the closure.
print counter();
print counter();
