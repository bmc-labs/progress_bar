<p align="center"><img src="./misc/progress_small.png" width="15%" /></p>

# `progress_bar` - a CLI progress bar

<a href="https://asciinema.org/a/ZesQrQgJhN1HEu5nwbzww9GTf" target="_blank">
  <img src="https://asciinema.org/a/ZesQrQgJhN1HEu5nwbzww9GTf.svg" />
</a>

I just needed a command line progress bar and none of the options I found on
GitHub or via Google were much to my liking, what with their camel cased names
and lack of `operator<<` overload and such. Now this is certainly not the best
ever implementation of anything, I just hacked this together real quick. Ah
yeah btw, the code supposed to figure out terminal size on Windows - I didn't
test that, as I would've needed to boot up the Windows VM and build it there,
yadda yadda, maybe somebody else can do it.


## Using it
It's header only. It has no dependencies other than the STL. It's Apache v2
licensed (because that's what my template for C++ projects has and I couldn't
be bothered).

So basically you just go
```cpp
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "progress_bar.h"


using namespace std::chrono_literals;


int main() {
  bmc::progress_bar pb{
    13,    // min, also initial value
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
```

and you get
```shell
[----->                             ]  10% -  0:42.312 elapsed
```

Of course there are a bunch more things you can do. If you don't like it to
auto increment when using `operator<<`, just give it a `false` there (4th ctor
argument). You might then want to use `++` or `--` in exactly the way you'd
expect, or `step(int steps)`, or `set(int pos)`. Essentially, the following
works:
```cpp
bmc::progress_bar pb{-13, 156, 80, false, true};

++pb;
pb++;
--pb;
pb--;

pb.step(5);   // move 5 steps forward
pb.set(137);  // go to progress 137
```

If you try to move outside `min` and `max`, it simply doesn't do that. If you
try to give it a `min` that is bigger than `max`, it throws. If you try to push
`width` (percentage of terminal width) to 0, it throws.


## More
That's it. The example above sits in `src/main.cc` and actually does run. Other
than that - just clone this repo and/or throw the header file in your project.

You're good to go.

