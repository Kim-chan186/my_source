import cv as cv
import cv2 as cv
import numpy as np
import sys
import copy

from matplotlib import pyplot as plt

# 카메라에 접근하기 위해 VideoCapture 객체를 생성
# cap = cv.VideoCapture(0)

# Create a black image
point = (-10, -10)
Mouse_flag = 0  # if 1, L_mouse down
Mouse_stuff_flag = 0  # 연필, 지우개, 낙서
pen_size = 5

img = cv.imread('E:\opencv\sources\samples\data\messi5.jpg')
ori = cv.imread('E:\opencv\sources\samples\data\messi5.jpg')

Eraser = cv.resize(~cv.cvtColor(cv.imread("Eraser.png"), cv.COLOR_BGR2BGRA), (30, 30))
Pencil = cv.resize(~cv.imread("pencil.png"), (30, 30))

cv.imshow("test", Pencil)

mask = np.zeros(img.shape[:2], np.uint8)
new_mask = np.zeros(img.shape[:2], np.uint8)
mask_tup = np.zeros(img.shape[:2], np.uint8)

bgdModel = np.zeros((1, 65), np.float64)
fgdModel = np.zeros((1, 65), np.float64)
rect = (0, 0, 547, 341)


# mouse callback function
def MouseEvent(event, x, y, flags, userdata):
    global point, Mouse_flag, mask, new_mask
    # 마우스 버튼 눌림 판단
    if event == cv.EVENT_LBUTTONDOWN:
        Mouse_flag = 1
    elif event == cv.EVENT_LBUTTONUP:
        Mouse_flag = 0
    # 마우스 위치
    if event == cv.EVENT_MOUSEMOVE:
        point = x, y

    # 눌려있을때 그림그리기
    if Mouse_flag == 1:
        if Mouse_stuff_flag == 0:  #  배경
            cv.circle(new_mask, (x, y), pen_size, 0, -1)
            mask[new_mask == 0] = 0
            mask[new_mask == 255] = 1

        elif Mouse_stuff_flag == 1:  #
            cv.circle(new_mask, (x, y), pen_size, 70, -1)
            mask[new_mask == 0] = 0
            mask[new_mask == 255] = 1

        elif Mouse_stuff_flag == 2:  #
            cv.circle(new_mask, (x, y), pen_size, 140, -1)
            mask[new_mask == 0] = 0
            mask[new_mask == 255] = 1

        elif Mouse_stuff_flag == 3:  # 확실한 전경
            cv.circle(new_mask, (x, y), pen_size, 254, -1)
            mask[new_mask == 0] = 0
            mask[new_mask == 255] = 1


    return


# show img with mouse state
# control imshow for all Windows
def showWindows() -> object:
    global point, img, mask, new_mask
    gain = 85

    img_show = cv.copyMakeBorder(img, 0, 0, 0, 0, cv.BORDER_REPLICATE)
    mask_show = cv.cvtColor(cv.copyMakeBorder(mask, 0, 0, 0, 0, cv.BORDER_REPLICATE) * gain, cv.COLOR_GRAY2BGR)

    color = (Mouse_stuff_flag * gain, Mouse_stuff_flag * gain, Mouse_stuff_flag * gain)
    cv.circle(img_show, point, pen_size, color, -1)
    cv.circle(mask_show, point, pen_size, color, -1)
    cv.circle(img_show, point, pen_size+1, (0, 255, 0), 1)
    cv.circle(mask_show, point, pen_size+1, (0, 255, 0), 1)

    cv.imshow("img", img_show)
    cv.imshow("mask", mask_show)
    cv.imshow("ori_show", ori)
    return


# Create a window and bind the function to window
def Windows_set() -> object:
    cv.namedWindow('ori_show')
    cv.setMouseCallback('ori_show', MouseEvent)
    return


def first_grabCut() -> object:
    global img, mask, rect, bgdModel, fgdModel, ori, new_mask

    cv.grabCut(ori, mask, rect, bgdModel, fgdModel, 5, cv.GC_INIT_WITH_RECT)
    new_mask = np.where((mask == 2) | (mask == 0), 0, 1).astype('uint8')
    img = img * new_mask[:, :, np.newaxis]
    return


def grabCut() -> object:
    global img, mask, rect, bgdModel, fgdModel, ori, new_mask

    mask[new_mask == 0] = 0
    mask[new_mask == 255] = 1

    mask, bgdModel, fgdModel = cv.grabCut(ori, mask, None, bgdModel, fgdModel, 5, cv.GC_INIT_WITH_MASK)
    mask2 = np.where((mask == 2) | (mask == 0), 0, 1).astype('uint8')
    img = img * mask2[:, :, np.newaxis]

    return


def Keyborde_event() -> object:
    global Mouse_stuff_flag, pen_size, mask

    input_key = cv.waitKeyEx(1)
    if input_key == 27:  # ESC
        sys.exit()
    elif input_key == 32:
        grabCut()
    elif input_key == 97:
        Mouse_stuff_flag = 0 if Mouse_stuff_flag > 3 else Mouse_stuff_flag + 1
    elif input_key == 2490368:  # up
        pen_size += 1
    elif input_key == 2621440:  # down
        pen_size -= 1
    elif input_key == 2424832:  # <-
        kernel = np.ones((3, 3), np.uint8)
        mask = cv.erode(mask, kernel, iterations=1)
    elif input_key == 2555904:  # ->
        kernel = np.ones((3, 3), np.uint8)
        mask = cv.dilate(mask, kernel, iterations=1)
    elif input_key != -1:
        print("int put key's core name is %d", input_key)


first_grabCut()
Windows_set()
while True:
    showWindows()
    Keyborde_event()

# cv.destroyAllWCindows()
