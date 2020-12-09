// Base class declared with single method
class Base {
    Method() {
        print "hello";
    }
}

// Child class extends from base, inheriting Method().
class Child extends Base {}

// Creates a child instance that calls the super classes method.
var x = Child();
x.Method();
