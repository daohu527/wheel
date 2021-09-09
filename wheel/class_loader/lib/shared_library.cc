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

#include "class_loader/lib/exceptions.h"
#include "class_loader/lib/shared_library.h"

namespace wheel {
namespace class_loader {


SharedLibrary::SharedLibrary(const std::string& path) {
  load(path, 0);
}

SharedLibrary::SharedLibrary(const std::string& path, int flags) {
  load(path, flags);
}

void SharedLibrary::load(const std::string& path) {
  load(path, 0);
}

void SharedLibrary::load(const std::string& path, int flags) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (handle_) throw LibraryAlreadyLoadedException(path);

  int real_flags = RTLD_LAZY;
  if (flags & SHLIB_LOCAL)
    real_flags |= RTLD_LOCAL;
  else
    real_flags |= RTLD_GLOBAL;

  handle_ = dlopen(path.c_str(), real_flags);
  if (!handle_) {
    const char* err = dlerror();
    throw LibraryLoadException(err);
  }

  path_ = path;
}

void SharedLibrary::unload() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (handle_) {
    dlclose(handle_);
    handle_ = 0;
  }
}

bool SharedLibrary::isLoaded() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return handle_ != 0;
}

bool SharedLibrary::hasSymbol(const std::string& name) {
  return getSymbol(name) != 0;
}

void* SharedLibrary::getSymbol(const std::string& name) {
  std::lock_guard<std::mutex> lock(mutex_);

  void* result = 0;
  if (handle_) {
    result = dlsym(handle_, name.c_str());
    if ((char* error = dlerror()) != nullptr) {
      throw SymbolNotFoundException(name);
    }
  }
  return result;
}

const std::string& SharedLibrary::getPath() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return path_;
}


}  // namespace class_loader
}  // namespace wheel
