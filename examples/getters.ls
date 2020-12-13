class GetterDemo {
  constructor(name) {
    this._name = name;
  }

  // Instance getter associated only with instances.
  name {
    return this._name;
  }

  // Static getter not associated to any instance.
  static type {
    return "Getter Demo";
  }
}

// Access static getter.
print GetterDemo.type;

// Access instance getter.
print GetterDemo("Erik").name;
