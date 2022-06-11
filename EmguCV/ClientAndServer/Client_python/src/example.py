from __future__ import print_function

import time
import openapi_client
from openapi_client.rest import ApiException
from pprint import pprint

# Defining the host is optional and defaults to http://localhost:8080/v1alpha3
# See configuration.py for a list of all supported configuration parameters.
configuration = openapi_client.Configuration(
    host = "http://localhost:8080/v1alpha3"
)



# Enter a context with an instance of the API client
with openapi_client.ApiClient(configuration) as api_client:
    # Create an instance of the API class
    api_instance = openapi_client.PolygonsLosApi(api_client)
    scenario_name = 'scenario_name_example' # str | Name of scenario to load
camera_x = 3.4 # float | cameraX location
camera_y = 3.4 # float | cameraY location
camera_z = 3.4 # float | cameraZ location
plate_height_above_target = 3.4 # float | Height (in meters) of the plate above the target

try:
    # Initialize the scenario
    api_instance.scene_init(scenario_name, camera_x, camera_y, camera_z, plate_height_above_target)
except ApiException as e:
    print("Exception when calling PolygonsLosApi->scene_init: %s\n" % e)

try:
    # Returns a list of polygons given a target location (latitude, longtitude, altitude) and a height above the target. Meaning the parameter is an array of 4 doubles (latitude, longtitude, altitude, height)
    api_response = api_instance.scene_polygons(camera_x, camera_y, camera_z, plate_height_above_target)
    pprint(api_response)
except ApiException as e:
    print("Exception when calling PolygonsLosApi->scene_polygons: %s\n" % e)