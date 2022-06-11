using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EOSim.SDK.Logic
{
    public sealed class PolygonsLosModuleImplementation
    {
        public void InitLos(string scenarioName, double cameraX, double cameraY, double cameraZ, double plateHeightAbovePoint, double epsilon, int width, int height)
        {
            //dummy code - do nothing for now
            int david = 5;
        }


        public List<Emgu.CV.Matrix<float>> CalculateWorldPoints(double cameraX, double cameraY, double cameraZ, double plateHeightAbovePoint, double epsilon)
        {
            List<Emgu.CV.Matrix<float>> worldPoints = new List<Emgu.CV.Matrix<float>>();
            return worldPoints;
        }

        private List<Emgu.CV.Matrix<float>> createDummyMatrix()
        {
            float world_x;
            float world_y;
            float z = 1000.0f;

            int numOfPolygons = 2;
            List<int> numOfPointsForEachPolygon = new List<int> { 5, 7};

            List<Emgu.CV.Matrix<float>> worldPoints = new List<Emgu.CV.Matrix<float>>();
            for (int i = 0; i < numOfPolygons; i++)
            {
                int numOfPoints = numOfPointsForEachPolygon[i];
                Emgu.CV.Matrix<float> singleContourWorldPoints = new Emgu.CV.Matrix<float>(numOfPoints, 3);

                for (int j = 0; j < numOfPoints; j++)
                {
                    world_x = (float)(i * 10 + j + 1);
                    world_y = (float)(i * 10 + j + 1);
                    singleContourWorldPoints[j, 0] = world_x;
                    singleContourWorldPoints[j, 1] = world_y;
                    singleContourWorldPoints[j, 2] = z;
                }
                float[,] elements = singleContourWorldPoints.Data;
                worldPoints.Add(singleContourWorldPoints);
            }
            return worldPoints;
        }

    }
}
