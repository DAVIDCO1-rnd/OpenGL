using EOSimU.API.AutoGen.v1alpha3.Models;
using EOSimU.API.AutoGen.v1alpha3.Modules;
using Nancy;
using System.Collections.Generic;

namespace EOSimU.API.AutoGen.v1alpha3.Adaptors
{
    internal class PolygonsLosAdaptor : EOSimU.API.AutoGen.v1alpha3.Modules.PolygonsLosService
    {
        private EOSim.SDK.Logic.PolygonsLosModuleImplementation service;

        private double cameraX;
        private double cameraY;
        private double cameraZ;
        private double plateHeightAbovePoint;
        private double epsilon = 1.0;

        public PolygonsLosAdaptor(EOSim.SDK.Logic.PolygonsLosModuleImplementation service)
        {
            this.service = service;
        }

        public void SceneInit(NancyContext context, string scenarioName, double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget)
        {
            if (cameraX == null || cameraY == null || cameraZ == null || plateHeightAboveTarget == null)
            {
                return;
            }
            else
            {
                double cameraXDouble = (double)cameraX;
                double cameraYDouble = (double)cameraY;
                double cameraZDouble = (double)cameraZ;
                double plateHeightAboveTargetDouble = (double)plateHeightAboveTarget;
                epsilon = 1.0;
                int width = 2048;
                int height = 2560;

                service.initLos(scenarioName, cameraXDouble, cameraYDouble, cameraZDouble, plateHeightAboveTargetDouble, epsilon, width, height);
            }
        }

        public List<Polygon> ConvertListOfMatricesToListOfPolygons(List<Emgu.CV.Matrix<float>> listOfMatrices)
        {
            List<Polygon> listOfPolygons = new List<Polygon>();
            int numOfPolygons = listOfPolygons.Count;
            for (int i = 0; i < numOfPolygons; i++)
            {
                List<Point> currentPolygonPoints = new List<Point>();
                Emgu.CV.Matrix<float> currentMatrix = listOfMatrices[i];
                for (int j=0; j < currentMatrix.Rows; j++)
                {
                    float x = currentMatrix[j, 0];
                    float y = currentMatrix[j, 1];
                    float z = currentMatrix[j, 2];

                    Point.PointBuilder pointBuilder = new Point.PointBuilder();
                    pointBuilder.X(x);
                    pointBuilder.Y(y);
                    pointBuilder.Z(z);
                    Point currentPoint = pointBuilder.Build();
                }

                Polygon.PolygonBuilder currentPolygonBuilder = new Polygon.PolygonBuilder();
                currentPolygonBuilder.Vertices(currentPolygonPoints);
                Polygon currentPolygon = currentPolygonBuilder.Build();
                listOfPolygons.Add(currentPolygon);
            }
            return listOfPolygons;
        }

        public List<Polygon> ScenePolygons(NancyContext context, double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget)
        {
            if (cameraX == null || cameraY == null || cameraZ == null || plateHeightAboveTarget == null)
            {
                return null;
            }
            else
            {
                double cameraXDouble = (double)cameraX;
                double cameraYDouble = (double)cameraY;
                double cameraZDouble = (double)cameraZ;
                double plateHeightAboveTargetDouble = (double)plateHeightAboveTarget;
                List<Emgu.CV.Matrix<float>> listOfMatrices = service.CalculateWorldPoints(cameraXDouble, cameraYDouble, cameraZDouble, plateHeightAboveTargetDouble, epsilon);
                List<Polygon> listOfPolygons = ConvertListOfMatricesToListOfPolygons(listOfMatrices);
                return listOfPolygons;
            }
        }
    }
}
