# test file using opencv / python to compare and contrast with itk / c++ ir methods
from __future__ import print_function
import numpy as np
import cv2
# total number of feature points
MAX = 400
# percent similarity of point matches
MIN = 0.1

def feature_matches(i, i2):
    # change to b/w images
    new_i = cv2.cvtColor(i, cv2.COLOR_BGR2GRAY)
    new_i2 = cv2.cvtColor(i2, cv2.COLOR_BGR2GRAY)

    # set keypoints/descriptors
    x = cv2.ORB_create(MAX)
    k1, d1 = x.detectAndCompute(new_i, None)
    k2, d2 = x.detectAndCompute(new_i2, None)

    # compare similar features
    comp = cv2.DescriptorMatcher_create(cv2.DESCRIPTOR_MATCHER_BRUTEFORCE_HAMMING)
    matches = comp.match(d1, d2, None)
    # sort based on distance/score
    matches.sort(key=lambda s: s.distance, reverse=False)
    # remove matches below min percent similarity
    n = int(len(matches) * MIN)
    matches = matches[:n]
    # create new image with feature matches
    features = cv2.drawMatches(i, k1, i2, k2, matches, None)
    cv2.imwrite("matches.jpg", features)

    # find coordinates of feature matches
    xy1 = np.zeros((len(matches), 2), dtype=np.float32)
    xy2 = np.zeros((len(matches), 2), dtype=np.float32)
    for i, m in enumerate(matches):
        xy1[i, :] = k1[m.queryIdx].pt
        xy2[i, :] = k2[m.trainIdx].pt

    # set estimated homography
    h, mask = cv2.findHomography(xy1, xy2, cv2.RANSAC)
    # set parameters for warpPerspective
    height, width, channels = i2.shape
    # image registration
    r = cv2.warpPerspective(i, h, (width, height))
    return r, h


if __name__ == '__main__':
    reference = "i1.png"
    ref_image = cv2.imread(reference, cv2.IMREAD_COLOR)
    moving = "i2.png"
    moving_image = cv2.imread(moving, cv2.IMREAD_COLOR)
    reg, h = feature_matches(moving_image, ref_image)
    # create alignment
    output = "align.jpg"
    print("Feature matched image created: ", output);
    cv2.imwrite(output, reg)
    print("Homography : \n", h)
