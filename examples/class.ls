class HelloWorld {
  Hello() {
    return this.message;
  }
}

print HelloWorld;

var hello = HelloWorld();

hello.message = "Hello, world";

print hello.Hello();
