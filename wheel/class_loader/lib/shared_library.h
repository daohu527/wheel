// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.

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

//  Created Date: 2021-9-8
//  Author: daohu527

#pragma once

#include <string>
#include <mutex>

#include "class_loader/lib/shared_library.h"

namespace wheel {
namespace class_loader {

class SharedLibrary {
 public:
  enum Flags {
    SHLIB_GLOBAL = 1,
      /// On platforms that use dlopen(), use RTLD_GLOBAL. This is the default
      /// if no flags are given.
      ///
      /// This flag is ignored on platforms that do not use dlopen().

    SHLIB_LOCAL  = 2,
      /// On platforms that use dlopen(), use RTLD_LOCAL instead of RTLD_GLOBAL.
      ///
      /// Note that if this flag is specified, RTTI (including dynamic_cast and 
      /// throw) will not work for types defined in the shared library with GCC 
      /// and possibly other compilers as well. 
      /// See http://gcc.gnu.org/faq.html#dso for more information.
      ///
      /// This flag is ignored on platforms that do not use dlopen().
  }

  SharedLibrary() = default;
  ~SharedLibrary() = default;
  explicit SharedLibrary(const std::string& path);
  SharedLibrary(const std::string& path, int flags);

  SharedLibrary(const SharedLibrary&) = delete;
  SharedLibrary& operator=(const SharedLibrary&) = delete;

  void load(const std::string& path);

  void load(const std::string& path, int flags);

  void unload();

  bool isLoaded() const;

  bool hasSymbol(const std::string& name);

  void* getSymbol(const std::string& name);

  const std::string& getPath() const;

 private:
  std::string path_;
  void* handle_;
  mutable std::mutex mutex_;
};


}  // namespace class_loader
}  // namespace wheel
