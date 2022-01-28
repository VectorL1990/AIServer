cc_library(
    name = "opencv",
    srcs = glob(
        [
            "lib/libopencv_core.so",
            "lib/libopencv_highgui.so",
            "lib/libopencv_imgcodecs.so",
            "lib/libopencv_imgproc.so",
            "lib/libopencv_video.so",
            "lib/libopencv_videoio.so",
            "lib/libopencv_calib3d.so",
            "lib/libopencv_ximgproc.so"
        ],
    ),
    hdrs = glob(["include/opencv4/**/*.h*"]),
    includes = ["include/opencv4/"],
    linkstatic = 1,
    visibility = ["//visibility:public"],
)
