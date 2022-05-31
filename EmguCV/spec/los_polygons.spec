swagger: "2.0"
info:
  description: |
    This is the first draft proposal for LOS spec.
  version: "0.1.1"
  title: "LOS"
host: "localhost:8080"
basePath: "/v1alpha3"
tags:
- name: "Scene"
  description: "Scene operations"
- name: "Entity"
  description: "Common entities operations"
schemes:
- "http"
paths:
  /Scene/Init/{scenarioName}:
    post:
      tags:
      - "Scene"
      summary: "Initialize the scenario"
      operationId: "SceneInit"
      parameters:
      - name: "scenarioName"
        in: "path"
        description: "Name of scenario to load"
        required: true
        type: "string"
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "scenario not found"
  /Entity/Polygons{target3dLocationAndHeight}:
    get:
      tags:
      - "Entity"
      summary: "Returns a list of polygons given a target location (latitude, longtitude, altitude) and a height above the target. Meaning the parameter is an array of 4 doubles (latitude, longtitude, altitude, height)"
      operationId: "ScenePolygons"
      produces:
      - "application/json"
      parameters:
      - in: "path"
        name: "target3dLocationAndHeight"
        description: "target location"
        required: true
        type: "array"
        collectionFormat: csv
        items:
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
      latitude:
        type: "number"
        format: "double"
      longitude:
        type: "number"
        format: "double"
      altitude:
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