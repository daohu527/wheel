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

//  Created Date: 2021-9-15
//  Author: daohu527

#pragma once

#include <string>


namespace wheel {
namespace class_loader {

class Classloader; // forward declaration

namespace impl {

std::string getCurrentlyLoadingLibraryName() const;

void setCurrentlyLoadingLibraryName(const std::string& name);

template <typename Derived, typename Base>
void registerPlugin(const std::string& derived_class_name,
    const std::string& base_class_name) {

}

template <typename Base>
Base* createInstance(const std::string& derived_class_name,
    Classloader* class_loader) {

}

bool isLibraryLoaded(const std::string& library_path,
    Classloader* class_loader) {

}

bool isLibraryLoadedByAnybody(const std::string& library_path);

void loadLibrary(const std::string& library_path, Classloader* class_loader);

void unloadLibrary(const std::string& library_path, Classloader* class_loader);


}  // namespace impl
}  // namespace class_loader
}  // namespace wheel
