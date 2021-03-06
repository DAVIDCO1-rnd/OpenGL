from __future__ import print_function

import numpy as np
import cv2
import time
import polygonsAdaptor
import utils3D

import sys
# insert at 1, 0 is the script path (or '' in REPL)
sys.path.insert(1, '../python_client_generated_code')

import openapi_client
from openapi_client.rest import ApiException
from pprint import pprint



# Defining the host is optional and defaults to http://localhost:8080/v1alpha3
# See configuration.py for a list of all supported configuration parameters.
configuration = openapi_client.Configuration(
    host = "http://localhost:8080/v1alpha3"
)


# decorator to calculate duration
# taken by any function.
def calculate_time(func):
    # added arguments inside the inner1,
    # if function takes any arguments,
    # can be added like this.
    def inner1(*args, **kwargs):
        # storing time before function execution
        begin = time.time()

        returned_value = func(*args, **kwargs)

        # storing time after function execution
        end = time.time()
        time_measurement = end - begin
        print("Total time taken in : ", func.__name__, time_measurement)
        return {'returned_value': returned_value, 'time_measurement': time_measurement}

    return inner1

@calculate_time
def get_polygons_data():
    try:
        target_latitude = 0.0  # float | cameraX location
        target_longitude = 0.0  # float | cameraY location
        target_height = 2.5  # float | cameraZ location
        plate_height_above_target = 500.0  # float | Height (in meters) of the plate above the target
        # Returns a list of polygons given a target location (latitude, longtitude, altitude) and a height above the target. Meaning the parameter is an array of 4 doubles (latitude, longtitude, altitude, height)
        polygons3D = api_instance.scene_polygons(target_latitude, target_longitude, target_height,
                                                 plate_height_above_target)
        target_location = np.array([target_latitude, target_longitude, target_height])
        utils3D.plotPolygons(polygons3D, target_location)
        pprint(polygons3D)
        polygons2D = api_instance.scene_polygons_pixels(target_latitude, target_longitude, target_height,
                                                        plate_height_above_target)
        # pprint(polygons2D)
        contours1 = polygonsAdaptor.convertPolygonsListToTuple(polygons2D)
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

        # pprint(contours)
    except ApiException as e:
        print("Exception when calling PolygonsLosApi->scene_polygons: %s\n" % e)
    return polygons3D


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

num_of_iterations = 1
times_arr = np.zeros(num_of_iterations)
for i in range(0,num_of_iterations):
    res = get_polygons_data()
    current_time = res['time_measurement']
    times_arr[i] = current_time
mean_time = np.mean(times_arr)
print(mean_time)
pass