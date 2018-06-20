//
// Copyright 2018- Florian Eich <florian.eich@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "progress.h"


using namespace std::chrono_literals;


int main() {
  flrn::progress_bar pb{
    -13,   // min, also initial value
    156,   // max
    80,    // width in percent of screen (including percentage and time)
    true,  // auto increment when outputting
    true   // show time at the end
  };

  // generate some data
  std::vector<int> vec(pb.size());
  std::iota(std::begin(vec), std::end(vec), 42);

  for (auto & e : vec) {
    e *= 2;
    std::this_thread::sleep_for(50ms);
    std::cout << pb;
  }

  std::cout << pb;  // if progress bar is already full, no increment happens

  return 0;
}

