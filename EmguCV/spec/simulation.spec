swagger: "2.0"
info:
  description: |
    This is the first draft proposal to check if OpenAPI is suitable as a specification between EOSIMU and it's clients.

    It should be noted that if this document is too complex in the end, it can be split to multiple different documents, each covering a specific concern of the simulation.

    The Scene group below is your starting point for interacting with the simulation. The groups below follow a hierarchy where the pipe signifies an "<parent>|<child>" inheritance model.
  version: "0.1.1"
  title: "EOSIMU"
host: "localhost:8080"
basePath: "/v1alpha3"
tags:
- name: "Scene"
  description: "Scene operations"
- name: "Entity"
  description: "Common entities operations"
- name: "Entity|Camera"
  description: "Common camera entities operations"
- name: "Entity|Camera|Quantum"
  description: "Quantum camera entities operations"
schemes:
- "http"
paths:
  /Version:
    get:
      summary: "Return the version information for all components"
      operationId: "Version"
      produces:
      - "application/json"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: "array"
            items:
              $ref: "#/definitions/ComponentVersion"
  /Scene/LoadScenario/{scenarioName}:
    post:
      tags:
      - "Scene"
      summary: "Reset simulation and load a new scenario"
      operationId: "SceneLoadScenario"
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
  /Scene/ListScenarioNames:
    get:
      tags:
      - "Scene"
      summary: "List the possible scenarios names"
      operationId: "SceneListScenario"
      produces:
      - "application/json"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: "array"
            items:
              type: "string"
  /Scene/ListEntityTypes:
    get:
      tags:
      - "Scene"
      summary: "Shows all possible types for entites"
      operationId: "SceneListEntityTypes"
      produces:
      - "application/json"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: "array"
            items:
              $ref: "#/definitions/EntityType"
        "401":
          description: "no scene loaded"
  /Scene/ListEntitiesByClass/{entityClass}:
    get:
      tags:
      - "Scene"
      summary: "Finds entities by class"
      operationId: "SceneListEntitiesByClass"
      produces:
      - "application/json"
      parameters:
      - name: "entityClass"
        in: "path"
        description: "Class of entity to find (SceneEntity, CameraEntity, QuantumSensorCamera, ...)."
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: "array"
            items:
              type: "string"
        "404":
          description: "entity class not found"
        "401":
          description: "no scene loaded"
  /Scene/Entity/{entityType}:
    post:
      tags:
      - "Scene"
      summary: "Create a new entity of a given type"
      operationId: "SceneCreateEntity"
      produces:
      - "application/json"
      parameters:
      - name: "entityType"
        in: "path"
        description: "Type of entity to create"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: "string"
        "404":
          description: "entity type not found"
        "401":
          description: "no scene loaded"
  /Scene/OriginPoint:
    get:
      tags:
      - "Scene"
      summary: "Return the origin point of the scene"
      operationId: "SceneOriginPoint"
      produces:
      - "application/json"
      responses:
        "200":
          description: "successful operation"
          schema:
            $ref: "#/definitions/OriginPoint"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Position:
    get:
      tags:
      - "Entity"
      summary: "Get the SixDOF position of an entity"
      operationId: "EntityGetPosition"
      produces:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of entity"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            $ref: "#/definitions/SixDOF"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
    put:
      tags:
      - "Entity"
      summary: "Change the SixDOF position of an entity"
      operationId: "EntitySetPosition"
      consumes:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of entity"
        required: true
        type: "string"
      - in: "body"
        name: "body"
        description: "SixDOF position object to put the entity in"
        required: true
        schema:
          $ref: "#/definitions/SixDOF"
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Enabled:
    get:
      tags:
      - "Entity"
      summary: "Return the entity enabled state"
      operationId: "EntityGetEnabled"
      produces:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of entity"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: "boolean"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
    put:
      tags:
      - "Entity"
      summary: "Set the entity enabled state"
      operationId: "EntitySetEnabled"
      consumes:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of entity"
        required: true
        type: "string"
      - in: "body"
        name: "body"
        description: "Status"
        required: true
        schema:
          type: "boolean"
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}:
    delete:
      tags:
      - "Entity"
      summary: "Delete a given entity id"
      operationId: "EntityDelete"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of entity to remove"
        required: true
        type: "string"
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Camera/Draw/{timeStamp}:
    post:
      tags:
      - "Entity|Camera"
      description: |
        You need to call this image to tell the simulation at what timestamp you want to capture an image. Afterwards you can call the ImageMetadata + ImageData to retrieve the image when it's ready.
      operationId: "CameraDraw"
      consumes:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera to render"
        required: true
        type: "string"
      - name: "timeStamp"
        in: "path"
        description: "The simulation time to render the frame in"
        required: true
        type: "number"
        format: "double"
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "camera not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Camera/Dimensions:
    get:
      tags:
      - "Entity|Camera"
      summary: "Get the TwoDDimensions of the camera"
      operationId: "CameraGetDimensions"
      produces:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            $ref: "#/definitions/TwoDDimensions"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Camera/ImageMetadata:
    get:
      tags:
      - "Entity|Camera"
      summary: "Get the image metadata"
      operationId: "CameraGetImageMetadata"
      produces:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            $ref: "#/definitions/ImageMetadata"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Camera/ImageData:
    get:
      tags:
      - "Entity|Camera"
      summary: "Get the image data"
      operationId: "CameraGetImageData"
      produces:
      - "application/octet-stream"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: string
            format: binary
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Camera/BrowserDebugImage:
    get:
      tags:
      - "Entity|Camera"
      summary: "Create an image which can be shown in the browser"
      operationId: "CameraGetBrowserDebugImage"
      produces:
      - "image/png"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: string
            format: binary
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Camera/Projection:
    get:
      tags:
      - "Entity|Camera"
      summary: "Get the Camera projection"
      description: ""
      operationId: "CameraGetProjection"
      produces:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera to update"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            $ref: "#/definitions/Projection"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
    put:
      tags:
      - "Entity|Camera"
      summary: "Set the Camera projection"
      description: ""
      operationId: "CameraSetProjection"
      consumes:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera to update"
        required: true
        type: "string"
      - in: "body"
        name: "body"
        description: "projection object to set the camera to"
        required: true
        schema:
          $ref: "#/definitions/Projection"
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
  /Entity/{entityId}/Camera/Quantum/IntegrationTime:
    get:
      tags:
      - "Entity|Camera|Quantum"
      summary: "Get the Quantum camera integration time"
      description: ""
      operationId: "CameraQuantumGetIntegrationTime"
      produces:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera"
        required: true
        type: "string"
      responses:
        "200":
          description: "successful operation"
          schema:
            type: "number"
            format: "double"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
    put:
      tags:
      - "Entity|Camera|Quantum"
      summary: "Set the Quantum camera integration time"
      description: ""
      operationId: "CameraQuantumSetIntegrationTime"
      consumes:
      - "application/json"
      parameters:
      - name: "entityId"
        in: "path"
        description: "ID of camera to update"
        required: true
        type: "string"
      - in: "body"
        name: "body"
        description: "The integration time to set the quantum camera to"
        required: true
        schema:
          type: "number"
          format: "double"
      responses:
        "204":
          description: "successful operation"
        "404":
          description: "entity not found"
        "401":
          description: "no scene loaded"
definitions:
  SixDOF:
    type: "object"
    required:
    - "h"
    - "p"
    - "r"
    - "x"
    - "y"
    - "z"
    properties:
      h:
        type: "number"
        format: "double"
      p:
        type: "number"
        format: "double"
      r:
        type: "number"
        format: "double"
      x:
        type: "number"
        format: "double"
      y:
        type: "number"
        format: "double"
      z:
        type: "number"
        format: "double"
  TwoDDimensions:
    type: "object"
    required:
    - "width"
    - "height"
    properties:
      width:
        type: "integer"
        format: "int32"
        description: "Pixels"
      height:
        type: "integer"
        format: "int32"
        description: "Pixels"
  ImageUserData:
    type: "object"
    required:
    - "cameraName"
    - "frameName"
    - "frameTime"
    - "systemTime"
    properties:
      cameraName:
        type: "string"
      frameName:
        type: "string"
      frameTime:
        type: "number"
        format: "double"
      systemTime:
        type: "number"
        format: "double"
  ImageMetadata:
    type: "object"
    required:
    - "bitsPerchannel"
    - "channels"
    - "height"
    - "width"
    - "name"
    - "number"
    - "writableStride"
    properties:
      bitsPerchannel:
        type: "integer"
        format: "int32"
        enum:
        - 8
        - 16
      channels:
        type: "integer"
        format: "int32"
        enum:
        - 1
        - 3
      height:
        type: "integer"
        format: "int32"
        description: "Pixels"
      width:
        type: "integer"
        format: "int32"
        description: "Pixels"
      name:
        type: "string"
      number:
        type: "integer"
        format: "int32"
      writableStride:
        type: "integer"
        format: "int32"
      userData:
        $ref: "#/definitions/ImageUserData"
  Projection:
    type: "object"
    required:
    - "fovx"
    - "fovy"
    - "near"
    - "far"
    properties:
      fovx:
        type: "number"
        format: "double"
        description: "Degrees"
        minimum: 0
      fovy:
        type: "number"
        format: "double"
        description: "Degrees"
        minimum: 0
      near:
        type: "number"
        format: "double"
        description: "Meters"
        minimum: 0
      far:
        type: "number"
        format: "double"
        description: "Meters"
        minimum: 0
  OriginPoint:
    type: "object"
    required:
    - "latitude"
    - "longitude"
    - "altitude"
    - "crs"
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
      crs:
        type: "string"
  ComponentVersion:
    type: "object"
    required:
    - "component"
    - "version"
    properties:
      component:
        type: "string"
      version:
        type: "string"
  EntityType:
    type: "object"
    required:
    - "entityType"
    - "entityClass"
    properties:
      entityType:
        type: "string"
      entityClass:
        type: "string"
