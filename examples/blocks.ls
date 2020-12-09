// Blocks allow variables to create isolated scopes from one another, allowing
// var to be reused for variables with the same name.

{
    var x = 10;
    print x;
}

{
    var x = "I'm in my own scope!";
    print x;
}
