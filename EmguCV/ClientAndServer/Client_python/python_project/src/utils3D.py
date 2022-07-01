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

def plotPolygons(polygons, target_location):
    polygonsList = convertPolygonsToListOfNumpy(polygons)
    ax = plt.axes(projection='3d')
    numOfPolygons = len(polygonsList)
    for i in range(0, numOfPolygons):
        x = polygonsList[i][0]
        y = polygonsList[i][1]
        z = polygonsList[i][2]
        ax.plot(x, y, z)
        numOfPoints = x.size
        for j in range(0, numOfPoints, 30):
            x_to_target = np.array([x[j], target_location[0]])
            y_to_target = np.array([y[j], target_location[1]])
            z_to_target = np.array([z[j], target_location[2]])
            ax.plot(x_to_target, y_to_target, z_to_target)
    ax.scatter3D(target_location[0], target_location[1], target_location[2], color = "green")
    ax.set_aspect('auto')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()