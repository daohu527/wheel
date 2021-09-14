
cc_library(
  name = "bucket",
  hdrs = [
    "bucket.h",
  ],
  srcs = [
    "bucket.cc",
  ]
)

cc_library(
  name = "time_wheel",
  hdrs = [
    "time_wheel.h",
    "ticket.h",
  ],
  srcs = [
    "time_wheel.cc",
  ],
  deps = [
    ":bucket",
  ]
)

cc_library(
  name = "timer",
  hdrs = [
    "timer.h",
  ],
  srcs = [
    "timer.cc",
  ],
  deps = [
    ":time_wheel",
  ]
)

cc_test(
  name = "timer_test",
  size = "small",
  srcs = [
    "timer_test.cc",
  ],
  deps = [
    ":timer",
    "@com_google_googletest//:gtest_main",
  ],
)
