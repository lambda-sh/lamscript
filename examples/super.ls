class First {
  method() {
    print "First Method";
  }
}

class Second extends First {
  method() {
    print "Second Method";
  }

  test() {
    super.method();
  }
}

class Third extends Second {}

Third().test();
