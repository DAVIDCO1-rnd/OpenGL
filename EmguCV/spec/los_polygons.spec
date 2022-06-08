swagger: "2.0"
info:
  description: |
    This is the first draft proposal for LOS spec.
  version: "0.1.1"
  title: "LOS"
host: "localhost:8080"
basePath: "/v1alpha3"
tags:
- name: "PolygonsLos"
  description: "Polygons Los operations"
schemes:
- "http"
paths:
  /PolygonsLos/Init/{scenarioName}{cameraX}/{cameraY}/{cameraZ}/{plateHeightAboveTarget}:
    post:
      tags:
      - "PolygonsLos"
      summary: "Initialize the scenario"
      operationId: "SceneInit"
      parameters:
      - in: "path"
        name: "scenarioName"
        description: "Name of scenario to load"
        required: true
        type: "string"
      - in: "path"
        name: "cameraX"
        description: "cameraX location"
        required: true
        type: number
        format: double
      - in: "path"
        name: "cameraY"
        description: "cameraY location"
        required: true
        type: number
        format: double
      - in: "path"
        name: "cameraZ"
        description: "cameraZ location"
        required: true
        type: number
        format: double
      - in: "path"
        name: "plateHeightAboveTarget"
        description: "Height (in meters) of the plate above the target"
        required: true
        type: number
        format: double        
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "scenario not found"
  /PolygonsLos/getPolygons/{cameraX}/{cameraY}/{cameraZ}/{plateHeightAboveTarget}:
    get:
      tags:
      - "PolygonsLos"
      summary: "Returns a list of polygons given a target location (latitude, longtitude, altitude) and a height above the target. Meaning the parameter is an array of 4 doubles (latitude, longtitude, altitude, height)"
      operationId: "ScenePolygons"
      produces:
      - "application/json"
      parameters:
      - in: "path"
        name: "cameraX"
        description: "cameraX location"
        required: true
        type: number
        format: double
      - in: "path"
        name: "cameraY"
        description: "cameraY location"
        required: true
        type: number
        format: double
      - in: "path"
        name: "cameraZ"
        description: "cameraZ location"
        required: true
        type: number
        format: double
      - in: "path"
        name: "plateHeightAboveTarget"
        description: "Height (in meters) of the plate above the target"
        required: true
        type: number
        format: double        
      responses:
        "200":
          description: "successful operation"
          schema:
            type: "array"
            items:
              $ref: "#/definitions/Polygon"
        "401":
          description: "no scene loaded"
definitions:
  Point:
    type: "object"
    required:
    - "latitude"
    - "longitude"
    - "altitude"
    properties:
      x:
        type: "number"
        format: "double"
      y:
        type: "number"
        format: "double"
      z:
        type: "number"
        format: "double"
  Polygon:
    type: "object"
    required:
    - "vertices"
    properties:
      vertices:
        type: "array"
        items:
          $ref: "#/definitions/Point"