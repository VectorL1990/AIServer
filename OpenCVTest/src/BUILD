package(default_visibility = ["//visibility:public"])

cc_library (
    name = "cvlib",
    srcs = glob(["*.cc"], ["*.cpp"]),
    hdrs = glob(["*.h"]),
    deps = [
        "@linux_opencv//:opencv",
    ]
)

cc_binary (
    name = "cvtest",
    srcs = ["OpenCV.cc"],
    deps = [
        "@linux_opencv//:opencv",
        ":cvlib"
    ]
)