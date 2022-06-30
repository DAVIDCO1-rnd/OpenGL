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
  /PolygonsLos/Init/{terrainName}:
    post:
      tags:
      - "PolygonsLos"
      summary: "Initialize the scenario"
      operationId: "SceneInit"
      parameters:
      - in: "path"
        name: "terrainName"
        description: "Name of scenario to load"
        required: true
        type: "string"
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "scenario not found"
  /PolygonsLos/getPolygons/{targetLatitude}/{targetLongitude}/{targetHeight}/{plateHeightAboveTarget}:
    get:
      tags:
      - "PolygonsLos"
      summary: "Returns a list of polygons given a target location (latitude, longtitude, altitude) and a height above the target. Meaning the parameter is an array of 4 doubles (latitude, longtitude, altitude, height)"
      operationId: "ScenePolygons"
      produces:
      - "application/json"
      parameters:
      - in: "path"
        name: "targetLatitude"
        description: "target latitude"
        required: true
        type: number
        format: double
      - in: "path"
        name: "targetLongitude"
        description: "target longitude"
        required: true
        type: number
        format: double
      - in: "path"
        name: "targetHeight"
        description: "target height"
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
  /PolygonsLos/getPolygons2D/{targetLatitude}/{targetLongitude}/{targetHeight}/{plateHeightAboveTarget}:
    get:
      tags:
      - "PolygonsLos"
      summary: "Returns a list of 2D polygons on an image given a target location (latitude, longtitude, altitude) and a height above the target. Meaning the parameter is an array of 4 doubles (latitude, longtitude, altitude, height)"
      operationId: "ScenePolygonsPixels"
      produces:
      - "application/json"
      parameters:
      - in: "path"
        name: "targetLatitude"
        description: "target latitude"
        required: true
        type: number
        format: double
      - in: "path"
        name: "targetLongitude"
        description: "target longitude"
        required: true
        type: number
        format: double
      - in: "path"
        name: "targetHeight"
        description: "target height"
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
              $ref: "#/definitions/Polygon2D"
        "401":
          description: "no scene loaded"          
definitions:
  Point:
    type: "object"
    required:
    - "x"
    - "y"
    - "z"
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
  Pixel:
    type: "object"
    required:
    - "x"
    - "y"
    properties:
      x:
        type: "number"
        format: "int"
      y:
        type: "number"
        format: "int"
  Polygon2D:
    type: "object"
    required:
    - "vertices"
    properties:
      vertices:
        type: "array"
        items:
          $ref: "#/definitions/Pixel"          