// Copyright 2021 daohu527@gmail.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//  Created Date: 2021-8-25
//  Author: daohu527

#pragma once

#include <functional>

namespace wheel {
namespace timer {

class Timer {
 public:
  using Callback = std::function<void()>;
  Timer(int interval, int time_uint, Callback callback, bool run_once_flag);

  void start();

  void stop();

 private:
  Callback callback_;
};

}  // namespace timer
}  // namespace wheel
