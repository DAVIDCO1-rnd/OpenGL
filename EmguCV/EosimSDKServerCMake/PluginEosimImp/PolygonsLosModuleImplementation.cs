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

    }
}
