# A simple script to calculate BMI
from pywebio.input import input, FLOAT
from pywebio.output import put_text, put_image
import math
import matplotlib.pyplot as plt
from heapq import heappush, heappop
from collections import defaultdict
from math import inf
import cv2
from pywebio import STATIC_PATH




def fixPixelValues(px):
    # convert the RGB values into floating point to avoid an overflow that will give me wrong answers
    return [float(px[0]), float(px[1]), float(px[2])]


## Given (x,y) coordinates of two neighboring pixels, calculate the edge weight.
# We take the squared euclidean distance between the pixel values and add 0.1
def getEdgeWeight(img, u, v):
    # get edge weight for edge between u, v
    # First make sure that the edge is legit
    i0, j0 = u[0], u[1]
    i1, j1 = v[0], v[1]
    height, width, _ = img.shape
    assert i0 >= 0 and j0 >= 0 and i0 < width and j0 < height  # pixel position valid?
    assert i1 >= 0 and j1 >= 0 and i1 < width and j1 < height  # pixel position valid?
    assert -1 <= i0 - i1 <= 1  # edge between node and neighbor?
    assert -1 <= j0 - j1 <= 1
    px1 = fixPixelValues(img[j0, i0])
    px2 = fixPixelValues(img[j1, i1])
    return .1 + (px1[0] - px2[0]) ** 2 + (px1[1] - px2[1]) ** 2 + (px1[2] - px2[2]) ** 2


# This is a useful function that given a list of (x,y) values,
# draw a series of red lines between each coordinate and next to
# show the path in the image
def drawPath(img, path, pThick=2):
    v = path[0]
    x0, y0 = v[0], v[1]
    for v in path:
        x, y = v[0], v[1]
        cv2.line(img, (x, y), (x0, y0), (255, 0, 0), pThick)
        x0, y0 = x, y
# ------------------




def computeShortestPath(img, source, dest):
    q = [(0, source, None)]

    parents = {source: None}

    seen = set()
    while q:
        dist, pos, parent = heappop(q)
        if pos == dest:
            parents[pos] = parent
            break
        if pos not in seen:
            seen.add(pos)
            parents[pos] = parent
            for dx, dy in ((-1, 0), (0, -1), (0, 1), (1, 0)):
                neighbor_pos = (pos[0] + dx, pos[1] + dy)
                try:
                    edge_weight = getEdgeWeight(img, pos, neighbor_pos)
                    heappush(q, (dist + edge_weight, neighbor_pos, pos))
                except AssertionError:
                    pass

    def path(parents, pos):
        path = []
        while parents[pos] != None:
            path.append(pos)
            pos = parents[pos]
        return path

    return path(parents, dest)


def lyft():
    Example = input("Please enter 1, 2 or 3 to view an example route：", type=FLOAT)


    top_status = [(1, 'Maps'), (2, 'Maze'),
                  (3, 'Maze2')]

    for top, status in top_status:
        if Example == top:
            put_text('Example: %.1d Type: %s' % (Example, status))
            put_text('The two dots on the map represent points of interest. Close the map to see the shortest route!')
    if Example == 1:

        img = cv2.imread('maps2.png')  # read an image from a file using opencv (cv2) library
        # you can annotate images
        cv2.circle(img, (242, 137), 7, (255, 0, 0),
                    -1)  # add a circle centered at (5, 220) radius 3, color red (RGB: 255,0,0)
        cv2.circle(img, (1019, 971), 7, (0, 0, 255),
                    -1)  # add a circle centered at (5,5) radius 3, color red (RGB: 0,0,255)
        plt.imshow(img)  # show the image on the screen

        plt.title('MAP')
        plt.show()

        img = cv2.imread('maps2.png')  # read an image from a file using opencv (cv2) library
        p = computeShortestPath(img, (242, 137), (1019, 971))
        drawPath(img, p)
        plt.imshow(img)  # show the image on the screen
        plt.title('SHORTEST PATH')
        plt.show()

    if Example == 2:
        img = cv2.imread('maze.png')  # read an image from a file using opencv (cv2) library
        # you can annotate images
        cv2.circle(img, (5, 220), 3, (255, 0, 0),
                   -1)  # add a circle centered at (5, 220) radius 3, color red (RGB: 255,0,0)
        cv2.circle(img, (5, 5), 3, (0, 0, 255),
                     -1)  # add a circle centered at (5,5) radius 3, color red (RGB: 0,0,255)
        plt.imshow(img)  # show the image on the screen
        plt.title('MAZE')
        plt.show()
        img = cv2.imread('maze.png')  # read an image from a file using opencv (cv2) library
        p = computeShortestPath(img, (5, 220), (5, 5))
        drawPath(img, p)
        plt.imshow(img)  # show the image on the screen
        plt.title('SHORTEST PATH')
        plt.show()

    if Example == 3:
        img = cv2.imread('maze3.JPG')
        cv2.circle(img, (70, 1750), 15, (255, 0, 0),
                   -1)  # add a circle centered at (600, 70) radius 10, color red (RGB: 255,0,0)
        cv2.circle(img, (900, 500), 15, (0, 255, 255),
                   -1)  # add a circle centered at (790,200) radius 10, color red (RGB: 255,0,0)
        plt.imshow(img)  # show the image on the screen
        plt.title('MAZE 2')
        plt.show()
        img = cv2.imread('maze3.JPG')  # read an image from a file using opencv (cv2) library
        p = computeShortestPath(img, (70, 1750), (900, 500))
        drawPath(img, p, 10)
        plt.imshow(img)  # show the image on the screen
        plt.title('MAZE 2 SHORTEST PATH')
        plt.show()

    else:
        put_text('Wrong Number!')



if __name__ == '__main__':
    lyft()


