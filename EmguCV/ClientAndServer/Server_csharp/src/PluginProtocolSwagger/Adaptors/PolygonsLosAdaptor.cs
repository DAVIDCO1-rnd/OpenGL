using EOSimU.API.AutoGen.v1alpha3.Models;
using EOSimU.API.AutoGen.v1alpha3.Modules;
using Nancy;
using System.Collections.Generic;

namespace EOSimU.API.Adaptors
{
    internal class PolygonsLosAdaptor : EOSimU.API.AutoGen.v1alpha3.Modules.PolygonsLosService
    {
        private EOSim.SDK.Logic.PolygonsLosImplementation service;

        private double cameraX;
        private double cameraY;
        private double cameraZ;
        private double plateHeightAbovePoint;
        private double epsilon = 1.0;
        private static bool firstTime = true; 

        public PolygonsLosAdaptor(EOSim.SDK.Logic.PolygonsLosImplementation service)
        {
            this.service = service;
        }

        public void SceneInit(NancyContext context, string terrainName)
        {
            epsilon = 1.0;
            int width = 2048;
            int height = 2560;

            service.InitLos(terrainName, epsilon, width, height);
        }

        public List<Polygon> ConvertListOfMatricesToListOfPolygons(List<Emgu.CV.Matrix<float>> listOfMatrices)
        {
            List<Polygon> listOfPolygons = new List<Polygon>();
            int numOfPolygons = listOfMatrices.Count;
            for (int i = 0; i < numOfPolygons; i++)
            {
                List<Point> currentPolygonPoints = new List<Point>();
                Emgu.CV.Matrix<float> currentMatrix = listOfMatrices[i];
                for (int j=0; j < currentMatrix.Rows; j++)
                {
                    float latitude = currentMatrix[j, 0];
                    float longitude = currentMatrix[j, 1];
                    float height = currentMatrix[j, 2];

                    Point.PointBuilder pointBuilder = new Point.PointBuilder();
                    pointBuilder.Latitude(latitude);
                    pointBuilder.Longitude(longitude);
                    pointBuilder.Height(height);
                    Point currentPoint = pointBuilder.Build();
                    currentPolygonPoints.Add(currentPoint);
                }

                Polygon.PolygonBuilder currentPolygonBuilder = new Polygon.PolygonBuilder();
                currentPolygonBuilder.Vertices(currentPolygonPoints);
                Polygon currentPolygon = currentPolygonBuilder.Build();
                listOfPolygons.Add(currentPolygon);
            }
            return listOfPolygons;
        }


        public List<Polygon> CreateDummyListOfPolygons()
        {
            List<Polygon> listOfPolygons = new List<Polygon>();
            int numOfPolygons = 2;
            List<int> numOfPointsForEachPolygon = new List<int> { 5, 7 };
            for (int i = 0; i < numOfPolygons; i++)
            {
                List<Point> currentPolygonPoints = new List<Point>();
                int numOfPoints = numOfPointsForEachPolygon[i];
                for (int j = 0; j < numOfPoints; j++)
                {
                    float latitude = i * 10 + j + 1;
                    float longitude = i * 10 + j + 1;
                    float height = 1000.0f;

                    Point.PointBuilder pointBuilder = new Point.PointBuilder();
                    pointBuilder.Latitude(latitude);
                    pointBuilder.Longitude(longitude);
                    pointBuilder.Height(height);
                    Point currentPoint = pointBuilder.Build();
                    currentPolygonPoints.Add(currentPoint);
                }

                Polygon.PolygonBuilder currentPolygonBuilder = new Polygon.PolygonBuilder();
                currentPolygonBuilder.Vertices(currentPolygonPoints);
                Polygon currentPolygon = currentPolygonBuilder.Build();
                listOfPolygons.Add(currentPolygon);
            }
            return listOfPolygons;
        }



        public List<Polygon> ScenePolygons(NancyContext context, double? targetLatitude, double? targetLongitude, double? targetHeight, double? plateHeightAboveTarget)
        {
            if (targetLatitude == null || targetLongitude == null || targetHeight == null || plateHeightAboveTarget == null)
            {
                return null;
            }
            else
            {
                double targetLatitudeDouble = (double)targetLatitude;
                double targetLongitudeDouble = (double)targetLongitude;
                double targetHeightDouble = (double)targetHeight;
                double plateHeightAboveTargetDouble = (double)plateHeightAboveTarget;
                List<Emgu.CV.Matrix<float>> listOfMatrices = new List<Emgu.CV.Matrix<float>>();
                if (firstTime == true)
                {
                    firstTime = false;
                    for (int i=0; i<5; i++)
                    {
                        listOfMatrices = service.CalculateWorldPoints(targetLatitudeDouble, targetLongitudeDouble, targetHeightDouble, plateHeightAboveTargetDouble, epsilon);
                    }
                }
                else
                {
                    listOfMatrices = service.CalculateWorldPoints(targetLatitudeDouble, targetLongitudeDouble, targetHeightDouble, plateHeightAboveTargetDouble, epsilon);
                }
                List<Polygon> listOfPolygons = ConvertListOfMatricesToListOfPolygons(listOfMatrices);
                //List<Polygon> listOfPolygons = CreateDummyListOfPolygons();
                return listOfPolygons;
            }
        }

        //public List<Polygon> ScenePolygons(NancyContext context, double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget)
        //{
        //    if (cameraX == null || cameraY == null || cameraZ == null || plateHeightAboveTarget == null)
        //    {
        //        return null;
        //    }
        //    else
        //    {
        //        double cameraXDouble = (double)cameraX;
        //        double cameraYDouble = (double)cameraY;
        //        double cameraZDouble = (double)cameraZ;
        //        double plateHeightAboveTargetDouble = (double)plateHeightAboveTarget;
        //        List<Emgu.CV.Matrix<float>> listOfMatrices = service.CalculateWorldPoints(cameraXDouble, cameraYDouble, cameraZDouble, plateHeightAboveTargetDouble, epsilon);
        //        List<Polygon> listOfPolygons = ConvertListOfMatricesToListOfPolygons(listOfMatrices);
        //        //List<Polygon> listOfPolygons = CreateDummyListOfPolygons();
        //        return listOfPolygons;
        //    }
        //}
    }
}
