#include <iostream>

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cout << "Usage: lamscript [script]" << std::endl; 
    exit(64);
  } else if (argc == 2) {
    std::cout << "This would run the script" << argv[1] << std::endl;
    exit(0);
  } else {
    std::cout << "This would be interpreted." << std::endl;
    exit(0);
  }
}
