package(default_visibility = ["//visibility:public"])

cc_library(
  name = "shared_library",
  hdrs = [
    "shared_library.h",
    "exceptions.h",
  ],
  srcs = [
    "shared_library.cc",
  ]
)

cc_test(
  name = "shared_library_test",
  size = "small",
  srcs = [
    "shared_library_test.cc",
  ],
  deps = [
    ":shared_library",
    "@com_google_googletest//:gtest_main",
  ],
)
