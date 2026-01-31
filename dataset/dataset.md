
# RGB-D Dataset

``` link
    https://cvg.cit.tum.de/data/datasets/rgbd-dataset/download
```

## Camera Intrinsics
``` intrinsics
    fx = 525.0  # focal length x
    fy = 525.0  # focal length y
    cx = 319.5  # optical center x
    cy = 239.5  # optical center y
```

``` K 

cv::Mat K = (cv::Mat_<double>(3, 3) << 
            fx, 0, cx,
            0, fy, cy,
            0, 0, 1);


```
