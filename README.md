# `progress` - a CLI progress bar

I don't want to bore you to death. Really. I just needed a command line
progress bar and none of the options I found on GitHub or via Google were much
to my liking, what with their camel cased names and lack of `operator<<`
overload and such.


## Using it
It's header only. It has no dependencies other than the STL. It's Apache v2
licensed (because that's what my template for C++ projects has and I couldn't
be bothered).

So basically you just go
```cpp
#include <numeric>
#include <vector>

#include "progress.h"

int main() {
  flrn::progress_bar pb{
    0,     // min, also initial value
    100,   // max
    50,    // width in percent of screen (including percentage and time)
    true,  // auto increment when outputting
    true   // show time at the end
  };

  // generate some data
  std::vector<int> vec(100);
  std::iota(std::begin(vec), std::end(vec), 42);

  for (auto & e : vec) {
    e *= 2;
    std::cout << pb;
  }

  std::cout << pb;  // if progress bar is already full, no increment happens

  return 0;
}
```

and you get
```shell
[----->                             ]  10% - 0:42.312 elapsed
```


## More
That's it. The example above sits in `src/main.cc` and actually does run. Other
than that - just clone this repo and/or throw the header file in your project.

You're good to go.

