# Lamscript
An interpreted language aimed for productivity and simplicity.

## Description
Lamscript is a language I've been working on for the last couple of months with
the goal of creating a programming language to further my learnings and
integrate into another project, [Lambda](https://github.com/lambda-sh/lambda).

This language is primarily aimed to be simple, consistent, and lightweight.
It may not be the best language in terms of performance, as it's developed
around an being an interpreted tree-walk language that primarily relies on
recursion to handle parsing, resolving, and interpretation.

This language is still currently experimental with many more features to come.

## Current features with working examples.
* [Classes](examples/class.ls).
* [Inheritance](examples/inheritance.ls).
* [Functions](examples/func.ls).
* [Variables](examples/variable.ls).
* [Conditionals](examples/conditionals.ls).
* [Logical operators](examples/logical.ls).
* [Loops](examples/loops.ls).
* [Closures](examples/closure.ls).
* [Recursion](examples/recursion.ls).
* [Blocks](examples/blocks.ls).
* [Printing](examples/print.ls).
* [Lambda expressions](examples/anonymous.ls).
* [Static & Instance getters](examples/getters.ls).
* Static class methods.
* Automatic memory management via c++ smart pointers.
* Interactive mode.

## Features to come
* Standard library (Arrays, Lists, File I/O, threading)
* modules (import statements like `import module_name`)
* const (`const x = 10;`)
* async (`async func MakeHttpRequest() {...}`)
* Compiling the language as a library for use in other software, providing the
ability for easily binding your c++ code to the lamscript runtime.
* Testing and an automated testing suite for other developers to use for working
on or with lamscript.

And many more features that I've forgot to list or haven't thought of yet!

## Setup
As of right now, I've only tested this setup on linux. However, there shouldn't
be any reason to think that this shouldn't work on macos (and maybe windows if
using a bash based shell). If there are issues building on any platform, please
file an issue on this repository and I'll gladly look into creating a solution.

I utilize gcc 10.2 for building and gdb for debugging.

Once you've cloned the repository and are in lamscripts working directory:

### linux
#### Dependencies
First, you need to setup any dependencies associated with the project with:
```bash
./scripts/setup.sh
```
This will fetch any sub modules and other dependencies that may be later added.

#### Building
To generate a release build of the interpreter, you need to execute:
```bash
./scripts/compile_lamscript.sh
```

Generating a debug build can be done with:
```bash
./scripts/compile_lamscript.sh --build Debug
```

And all flags associated with this and other scripts can be checked with:
```bash
./scripts/compile_lamscript.sh --help
```

#### Running an example
To run an example using the release build you've generated previously, you can
execute:
```bash
./scripts/run_example.sh --example <example_name>.ls
```
Where example name is the name of an example inside of the examples folder.

To run an example with a debug build, you can simply execute:
```bash
./scripts/run_example.sh --example <example_name>.ls --build Debug
```

And lastly if you'd like to run either

Flag positioning doesn't matter and this assumes you built a debug build
previously.

### macos
Should be the same as linux, but I could be wrong if someone volunteers to try
it out and prove me wrong!

### windows
Not directly supported yet, but should be the same as linux assuming you have a
bash shell and access to a c++ compiler from it.

## Resources I used to implement this langauge
* [Crafting interpreters](http://craftinginterpreters.com/inheritance.html) was
used for learning the theory behind how languages work. Unfortunately, their
guide is written in java while I chose to use c++ for the implementation.
A lot of my core implementation has drastically changed from the tutorials, but
the overall theory remains highly relevant to this project.
* [Recursive descent parsing](https://en.wikipedia.org/wiki/Recursive_descent_parser)
was used for developing a programming language parser that recursively parses a
series of tokens into their corresponding statements and expressions.
* [Curated language resource list](https://tomassetti.me/resources-create-programming-languages/)
is a list of programming language resources that was used for finding valuable
and modern resources centered around developing a language. There are many great
references to go through here if you'd like to look at all of the individual
components a programming language is built from.
* [Formal langauge theory - caltech](https://www.its.caltech.edu/~matilde/FormalLanguageTheory.pdf)
is a pdf that was used for understanding the intricacies of language and how you
can mathematically describe things like utilizing a finite set of syntax to
produce an infinite number of different strings.
* [cpp reference](https://en.cppreference.com/w/) was used for all things c++
related.

## License
This repository uses the MIT license to protect it's distributed works. Please
make sure to include a copy of it in your works. As a personal favor, I ask that
you also include this README to give credit to all of the resources and people
that helped me build this project.
