import numpy as np

def convertPolygonsListToTuple(polygons):
    polygonsList = []
    for polygon in polygons:
        numOfPoints = len(polygon.vertices)
        polygonNumpy = np.zeros((numOfPoints, 1, 2), dtype=int)
        for i in range(0, numOfPoints):
            point = polygon.vertices[i]
            polygonNumpy[i, 0, 0] = point.latitude
            polygonNumpy[i, 0, 1] = point.y
        polygonsList.append(polygonNumpy)
    polygonsTuple = tuple(polygonsList)
    return polygonsTuple