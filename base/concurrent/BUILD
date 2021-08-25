#load("//tools:cpplint.bzl", "cpplint")

cc_library(
  name = "blocking_queue",
  hdrs = [
    "blocking_queue.h",
  ],
)

cc_test(
  name = "blocking_queue_test",
  size = "small",
  srcs = [
    "blocking_queue_test.cc",
  ],
  deps = [
    ":blocking_queue",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_library(
  name = "spsc_queue",
  hdrs = [
    "spsc_queue.h",
  ],
)

cc_test(
  name = "spsc_queue_test",
  size = "small",
  srcs = [
    "spsc_queue_test.cc",
  ],
  deps = [
    ":spsc_queue",
    "@com_google_googletest//:gtest_main",
  ]
)
