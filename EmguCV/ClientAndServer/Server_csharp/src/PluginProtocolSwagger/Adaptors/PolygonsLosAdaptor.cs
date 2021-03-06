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

        List<Polygon2D> ConvertPixelsToListOf2DPolygons(Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours)
        {

            List<Polygon2D> listOfPolygons2D = new List<Polygon2D>();
            int numOfPolygons = pixelsContours.Size;
            for (int i = 0; i < numOfPolygons; i++)
            {
                List<Pixel> currentPolygon2DPixels = new List<Pixel>();
                System.Drawing.Point[] currentContoursArray = pixelsContours[i].ToArray();
                int numOfPoints = currentContoursArray.Length;
                for (int j = 0; j < numOfPoints; j++)
                {
                    int pixel_x = currentContoursArray[j].X;
                    int pixel_y = currentContoursArray[j].Y;


                    Pixel.PixelBuilder pixelBuilder = new Pixel.PixelBuilder();
                    pixelBuilder.X(pixel_x);
                    pixelBuilder.Y(pixel_y);
                    Pixel currenPixel = pixelBuilder.Build();
                    currentPolygon2DPixels.Add(currenPixel);
                }

                Polygon2D.Polygon2DBuilder currentPolygon2DBuilder = new Polygon2D.Polygon2DBuilder();
                currentPolygon2DBuilder.Vertices(currentPolygon2DPixels);
                Polygon2D currentPolygon2D = currentPolygon2DBuilder.Build();
                listOfPolygons2D.Add(currentPolygon2D);
            }
            return listOfPolygons2D;
        }

        public List<Polygon> ConvertListOfMatricesToListOfPolygons(List<Emgu.CV.Matrix<double>> listOfMatrices)
        {
            List<Polygon> listOfPolygons = new List<Polygon>();
            int numOfPolygons = listOfMatrices.Count;
            for (int i = 0; i < numOfPolygons; i++)
            {
                List<Point> currentPolygonPoints = new List<Point>();
                Emgu.CV.Matrix<double> currentMatrix = listOfMatrices[i];
                for (int j=0; j < currentMatrix.Rows; j++)
                {
                    double latitude = currentMatrix[j, 0];
                    double longitude = currentMatrix[j, 1];
                    double height = currentMatrix[j, 2];

                    Point.PointBuilder pointBuilder = new Point.PointBuilder();
                    pointBuilder.X(latitude);
                    pointBuilder.Y(longitude);
                    pointBuilder.Z(height);
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
                    double latitude = i * 10 + j + 1;
                    double longitude = i * 10 + j + 1;
                    double height = 1000.0f;

                    Point.PointBuilder pointBuilder = new Point.PointBuilder();
                    pointBuilder.X(latitude);
                    pointBuilder.Y(longitude);
                    pointBuilder.Z(height);
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
                List<Emgu.CV.Matrix<double>> listOfMatrices = service.CalculateWorldPoints(targetLatitudeDouble, targetLongitudeDouble, targetHeightDouble, plateHeightAboveTargetDouble, epsilon);
                List<Polygon> listOfPolygons = ConvertListOfMatricesToListOfPolygons(listOfMatrices);
                //List<Polygon> listOfPolygons = CreateDummyListOfPolygons();
                return listOfPolygons;
            }
        }

        public List<Polygon2D> ScenePolygonsPixels(NancyContext context, double? targetLatitude, double? targetLongitude, double? targetHeight, double? plateHeightAboveTarget)
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
                Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours = service.CalculatePixels(targetLatitudeDouble, targetLongitudeDouble, targetHeightDouble, plateHeightAboveTargetDouble, epsilon);
                List<Polygon2D> listOfPolygons2D = ConvertPixelsToListOf2DPolygons(pixelsContours);
                //List<Polygon> listOfPolygons = CreateDummyListOfPolygons();
                return listOfPolygons2D;
            }
        }


    }
}
