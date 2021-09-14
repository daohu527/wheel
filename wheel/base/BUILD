package(default_visibility = ["//visibility:public"])

cc_library(
  name = "macros",
  hdrs = [
    "macros.h",
  ],
)

cc_test(
  name = "macros_test",
  size = "small",
  srcs = [
    "macros_test.cc",
  ],
  deps = [
    ":macros",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_library(
  name = "object_pool",
  hdrs = [
    "object_pool.h",
  ],
)

cc_test(
  name = "object_pool_test",
  size = "small",
  srcs = [
    "object_pool_test.cc",
  ],
  deps = [
    ":object_pool",
    "@com_google_googletest//:gtest_main",
  ],
)
