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

#include <stdexcept>
#include <string>

namespace wheel {
namespace class_loader {

#define DECLARE_EXCEPTION_CODE(CLS, BASE) \
  class CLS : public BASE {               \
   public:                                \
    explicit CLS(const std::string& what_arg) : BASE(what_arg.c_str()) {} \
    explicit CLS(const char* what_arg) : BASE(what_arg) {}                \
  };


DECLARE_EXCEPTION_CODE(LibraryAlreadyLoadedException, std::runtime_error)
DECLARE_EXCEPTION_CODE(LibraryLoadException, std::runtime_error)
DECLARE_EXCEPTION_CODE(SymbolNotFoundException, std::runtime_error)


}  // namespace class_loader
}  // namespace wheel
