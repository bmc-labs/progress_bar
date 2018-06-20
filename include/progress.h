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

#ifndef PROGRESS_INCLUDE_PROGRESS_H_
#define PROGRESS_INCLUDE_PROGRESS_H_

#include <cstdlib>
#include <exception>
#include <iomanip>
#include <ostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace flrn {


class progress_bar
{
  using self_t = progress_bar;
  friend std::ostream & operator<<(std::ostream & os, self_t & pb);

public:
  progress_bar(
    int min, int max, std::size_t width, bool auto_inc, bool show_time)
   : _min(min),
     _max(max),
     _current(min),
     _width(width),
     _auto_inc(auto_inc),
     _show_time(show_time) {
    if (_max <= _min) {
      throw std::range_error("max must be greater than min");
    }
    if (!_width) {
      throw std::invalid_argument("width cannot be 0% of terminal");
    }
  }

  self_t & operator++() noexcept {
    if (_current < _max) { ++_current; }
    return *this;
  }

  self_t operator++(int) noexcept {
    if (_current < _max) {
      auto tmp{*this};
      ++_current;
      return tmp;
    }
    return *this;
  }

  self_t & operator--() noexcept {
    if (_current > _min) { --_current; }
    return *this;
  }

  self_t operator--(int) noexcept {
    if (_current > _min) {
      auto tmp{*this};
      --_current;
      return tmp;
    }
    return *this;
  }

  void step(int steps = 1) noexcept { _current = clamp(_current + steps); }

  void set(int value) noexcept { _current = clamp(value); }

  std::string disp() noexcept {
    if (_auto_inc) { step(); }

    std::size_t line_length{term_width() * _width / 100};
    if (line_length < 50) {
      _show_time = false;
    } else {
      line_length -= 20;
    }

    std::size_t progress{(_current - _min) / (_max - _min) * 100};
    std::size_t bar_length{line_length - 7};

    std::stringstream ss;
    ss << '[';
    for (std::size_t i{0}; i < bar_length; ++i) {
      if (i < progress) {
        ss << '-';
      } else if (i == progress) {
        ss << '>';
      } else {
        ss << ' ';
      }
    }
    ss << "] " << std::setw(3) << progress << '%';

    if (_show_time) {
      ss << " - "
         << "01:34.514"
         << " elapsed";
    }

    ss << '\n';

    return ss.str();
  }

private:
  int clamp(int input) const noexcept {
    if (input < _min) { return _min; }
    if (input > _max) { return _max; }
    return input;
  }

  int term_width() const noexcept {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.srWindow.Right - info.srWindow.Left + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
  }

private:
  const int         _min;
  const int         _max;
  int               _current;
  const std::size_t _width;
  bool              _auto_inc;
  bool              _show_time;
};


std::ostream & operator<<(std::ostream & os, flrn::progress_bar & pb) {
  os << pb.disp();
  return os;
}

}  // namespace flrn

#endif  // PROGRESS_INCLUDE_PROGRESS_H_

