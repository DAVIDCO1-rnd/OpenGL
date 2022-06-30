from __future__ import print_function

import numpy as np
import cv2
import time

import sys
# insert at 1, 0 is the script path (or '' in REPL)
sys.path.insert(1, '../python_client_generated_code')

import openapi_client
from openapi_client.rest import ApiException
from pprint import pprint

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

# Defining the host is optional and defaults to http://localhost:8080/v1alpha3
# See configuration.py for a list of all supported configuration parameters.
configuration = openapi_client.Configuration(
    host = "http://localhost:8080/v1alpha3"
)



# Enter a context with an instance of the API client
with openapi_client.ApiClient(configuration) as api_client:
    # Create an instance of the API class
    api_instance = openapi_client.PolygonsLosApi(api_client)
    terrain_name = 'terrain_name_example' # str | Name of terrain to add to scenario

try:
    # Initialize the scenario
    api_instance.scene_init(terrain_name)
except ApiException as e:
    print("Exception when calling PolygonsLosApi->scene_init: %s\n" % e)

try:
    camera_x = 3.4  # float | cameraX location
    camera_y = 3.4  # float | cameraY location
    camera_z = 3.4  # float | cameraZ location
    plate_height_above_target = 3.4  # float | Height (in meters) of the plate above the target
    # Returns a list of polygons given a target location (latitude, longtitude, altitude) and a height above the target. Meaning the parameter is an array of 4 doubles (latitude, longtitude, altitude, height)
    polygons = api_instance.scene_polygons(camera_x, camera_y, camera_z, plate_height_above_target)
    contours1 = convertPolygonsListToTuple(polygons)
    filename = '../../../network_folder/screenShot.bmp'
    color_image = cv2.imread(filename, cv2.IMREAD_COLOR)
    image_with_contours = color_image.copy()

    grayscale_image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
    thresh = 127
    binary_image = cv2.threshold(grayscale_image, thresh, 255, cv2.THRESH_BINARY)[1]
    contours, hierarchy = cv2.findContours(binary_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    cv2.drawContours(image=image_with_contours, contours=contours1, contourIdx=-1, color=(0, 255, 0), thickness=2,
                     lineType=cv2.LINE_AA)
    cv2.imshow("image_with_contours", image_with_contours)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    pass

    #pprint(contours)
except ApiException as e:
    print("Exception when calling PolygonsLosApi->scene_polygons: %s\n" % e)