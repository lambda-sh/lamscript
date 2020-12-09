// Creates a class
class Student {

    // Classes can be setup with explicit constructors.
    constructor(name, age, concentration) {
        this.name = name;
        this.age = age;
        this.concentration = concentration;
    }

    // Class methods are expressed the same as functions except without the
    // prefix `func`
    GetDetails() {
        print "Student name: " + this.name + " age: " + this.age
            + " concentration: " + this.concentration;
    }
}

// Creates an object and stores it into ben.
var ben = Student("Ben", "20", "BEW");

// Executes the class method.
ben.GetDetails();
