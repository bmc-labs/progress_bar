#include "progress.h"

int main() {
  flrn::progress_bar pb{
    100,   // steps
    50,    // width in percent of screen
    0,     // starting value
    true,  // auto increment when outputting
    true   // show time at the end
  };

  // generate some data
  std::vector<int> vec(100);
  std::iota(std::begin(vec), std::end(vec), 42);

  for (const auto & e : vec) {
    e *= 2;
    std::cout << pb;
  }

  std::cout << pb;  // if progress bar is already full, nothing happens

  return 0;
}
