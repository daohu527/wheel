
cc_library(
  name = "time_wheel",
  hdrs = [
    "time_wheel.h",
    "bucket.h",
    "ticket.h",
  ],
  srcs = [
    "time_wheel.cc",
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
