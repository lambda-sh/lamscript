class Math {

  constructor(example) {
    this.example = example;
  }

  static PI {
    return 3.141569;
  }

  DoSomething() {
    return Math.PI;
  }

  test {
    return this.example;
  }
}

// Instance getter
var math = Math("First instance");
print math.test;

// Getters bind to their original instance/static base type.
var math2 = Math("Second instance");
math2.test = math.test;
print math2.test;

// Static getter
print Math.PI;

// Calling static var from instance.
print math.DoSomething();
