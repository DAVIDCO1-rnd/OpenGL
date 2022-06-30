import numpy as np
import matplotlib.pyplot as plt

def convertPolygonsToListOfNumpy(polygons):
    polygonsList = []
    for polygon in polygons:
        numOfPoints = len(polygon.vertices)
        x = np.zeros(numOfPoints)
        y = np.zeros(numOfPoints)
        z = np.zeros(numOfPoints)
        for i in range(0, numOfPoints):
            point = polygon.vertices[i]
            x[i] = point.x
            y[i] = point.y
            z[i] = point.z
        singlePolygon = [x, y, z]
        polygonsList.append(singlePolygon)
    return polygonsList

def plotPolygons(polygons):
    polygonsList = convertPolygonsToListOfNumpy(polygons)
    ax = plt.axes(projection='3d')
    numOfPolygons = len(polygonsList)
    for i in range(0, numOfPolygons):
        x = polygonsList[i][0]
        y = polygonsList[i][1]
        z = polygonsList[i][2]
        ax.plot3D(z, y, x)
    ax.set_aspect('auto')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()