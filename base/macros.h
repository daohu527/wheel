#pragma once

#define DISALLOW_COPY_AND_ASSIGN(classname) \
  classname(const classname&) = delete; \
  classname& operator=(const classname&) = delete;

#define DECLARE_SINGLETON(classname) \
 public: \
  static classname& instance() { \
    static classname object; \
    return object; \
  } \
  DISALLOW_COPY_AND_ASSIGN(classname) \
 private: \
  classname() {}
