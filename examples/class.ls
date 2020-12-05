class HelloWorld {
  static Hello() {
    return "Hello, world!";
  }

  Hey() {
    print this;
    print "hey";
  }
}

print HelloWorld;

var hello = HelloWorld();

hello.message = "Hello, world";

print HelloWorld.Hello();

//
class Student {
    constructor(name, age, concentration) {
        this.name = name;
        this.age = age;
        this.concentration = concentration;
    }

    GetDetails() {
        print  "Student name: " + this.name + " age: " + this.age
            + " concentration:" + this.concentration;
    }
}
var ben = Student("Ben", "20", "BEW");

ben.GetDetails();
