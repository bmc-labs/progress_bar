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

#include <chrono>
#include <cmath>
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

using namespace std::chrono_literals;


namespace flrn {


constexpr const std::size_t MIN_LINE_LENGTH{50};
constexpr const std::size_t TIME_OUTPUT_LENGTH{20};
constexpr const std::size_t MISC_SYMBOLS_LENGTH{7};


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
     _show_time(show_time),
     _timestamp(std::chrono::high_resolution_clock::now()) {
    if (_max <= _min) {
      throw std::range_error("max must be greater than min");
    }
    if (!_width) {
      throw std::invalid_argument("width cannot be 0% of terminal");
    }
  }

  int size() const noexcept { return _max - _min; }

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

    auto line_length{term_width() * _width / 100};
    if (line_length < MIN_LINE_LENGTH) {
      _show_time = false;
    } else {
      line_length -= TIME_OUTPUT_LENGTH;
    }
    auto bar_length{line_length - MISC_SYMBOLS_LENGTH};

    auto progress{
      _current == _max ? 100.0 : (_current - _min) / ((_max - _min) / 100.0)};

    auto position{progress == 100.0 ? bar_length
                                    : static_cast<std::size_t>(std::round(
                                        progress * (bar_length / 100.0)))};

    std::stringstream ss;
    ss << '[';
    for (std::size_t i{0}; i <= bar_length; ++i) {
      if (i < position) {
        ss << '-';
      } else if (i == position) {
        ss << '>';
      } else {
        ss << ' ';
      }
    }
    ss << "] " << std::setw(3) << static_cast<int>(progress) << '%';

    if (_show_time) {
      std::chrono::duration<double> elapsed =
        std::chrono::high_resolution_clock::now() - _timestamp;

      auto minutes = static_cast<int>(elapsed.count()) / 60;

      ss << " - " << std::setw(2) << minutes << ":" << std::right
         << std::setfill('0') << std::fixed << std::setprecision(2)
         << std::setw(5) << elapsed.count() << " elapsed";
    }

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
  const int                                                   _min;
  const int                                                   _max;
  int                                                         _current;
  const std::size_t                                           _width;
  bool                                                        _auto_inc;
  bool                                                        _show_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> _timestamp;
};


std::ostream & operator<<(std::ostream & os, flrn::progress_bar & pb) {
  os << "\33[2K\033[A\33[2K\r" << pb.disp() << '\n';
  return os;
}

}  // namespace flrn

#endif  // PROGRESS_INCLUDE_PROGRESS_H_

