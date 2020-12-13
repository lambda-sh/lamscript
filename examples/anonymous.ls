func ExecuteCallback(callback) {
  print "Inside function about to execute callback.";
  callback();
}

var callback = func () {
  print "hello";
};

ExecuteCallback(func () {
  print "Inside of the anonymous function.";
});
