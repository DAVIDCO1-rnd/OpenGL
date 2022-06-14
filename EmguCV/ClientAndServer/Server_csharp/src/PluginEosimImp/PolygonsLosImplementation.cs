using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

using Emgu.CV;
using Emgu.CV.Structure;

namespace EOSim.SDK.Logic
{
    public sealed class PolygonsLosImplementation
    {
        Image<Bgr, byte> imgInput;


        public void InitLos(string scenarioName, double cameraX, double cameraY, double cameraZ, double plateHeightAbovePoint, double epsilon, int width, int height)
        {
            //dummy code - do nothing for now
            int david = 5;
        }

        private Image<Gray, byte> convertRgbToBinaryImage(Image<Bgr, byte> imgInput)
        {
            Image<Gray, byte> binaryImage = imgInput.Convert<Gray, byte>().ThresholdBinary(new Gray(100), new Gray(255));
            return binaryImage;
        }

        /// <summary>
        /// Get a neighbor index in the heirarchy tree.
        /// </summary>
        /// <returns>
        /// A neighbor index or -1 if the given neighbor does not exist.
        /// </returns>
        //public int Get(HierarchyIndex component, int index)
        //public int GetHierarchy(Mat Hierarchy, int contourIdx, int component)
        public int[] GetHierarchy(Mat hierarchy, int contourIdx)
        {
            int[] ret = new int[] { };

            if (hierarchy.Depth != Emgu.CV.CvEnum.DepthType.Cv32S)
            {
                throw new ArgumentOutOfRangeException("ContourData must have Cv32S hierarchy element type.");
            }
            if (hierarchy.Rows != 1)
            {
                throw new ArgumentOutOfRangeException("ContourData must have one hierarchy hierarchy row.");
            }
            if (hierarchy.NumberOfChannels != 4)
            {
                throw new ArgumentOutOfRangeException("ContourData must have four hierarchy channels.");
            }
            if (hierarchy.Dims != 2)
            {
                throw new ArgumentOutOfRangeException("ContourData must have two dimensional hierarchy.");
            }
            long elementStride = hierarchy.ElementSize / sizeof(Int32);
            var offset0 = (long)0 + contourIdx * elementStride;
            if (0 <= offset0 && offset0 < hierarchy.Total.ToInt64() * elementStride)
            {


                var offset1 = (long)1 + contourIdx * elementStride;
                var offset2 = (long)2 + contourIdx * elementStride;
                var offset3 = (long)3 + contourIdx * elementStride;

                ret = new int[4];

                unsafe
                {
                    //return *((Int32*)Hierarchy.DataPointer.ToPointer() + offset);

                    ret[0] = *((Int32*)hierarchy.DataPointer.ToPointer() + offset0);
                    ret[1] = *((Int32*)hierarchy.DataPointer.ToPointer() + offset1);
                    ret[2] = *((Int32*)hierarchy.DataPointer.ToPointer() + offset2);
                    ret[3] = *((Int32*)hierarchy.DataPointer.ToPointer() + offset3);
                }


            }
            //else
            //{
            //    return new int[] { };
            //}

            return ret;
        }

        private void calcPixelContours(Image<Gray, byte> binaryImage, out Emgu.CV.Util.VectorOfVectorOfPoint approxPixelsContours, out Point[][] approxPixelsContoursArray, out List<int[]> hierarchies)
        {
            Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours = new Emgu.CV.Util.VectorOfVectorOfPoint();
            Emgu.CV.Mat hierarchyMat = new Emgu.CV.Mat();
            CvInvoke.FindContours(binaryImage, pixelsContours, hierarchyMat, Emgu.CV.CvEnum.RetrType.Tree, Emgu.CV.CvEnum.ChainApproxMethod.ChainApproxSimple);

            approxPixelsContours = new Emgu.CV.Util.VectorOfVectorOfPoint();

            double epsilon = 10.0;
            bool closed = true;

            for (int contourIdx = 0; contourIdx < pixelsContours.Size; contourIdx++)
            {
                Emgu.CV.Util.VectorOfPoint currentContour = pixelsContours[contourIdx];
                double peri = CvInvoke.ArcLength(currentContour, closed);
                Emgu.CV.Util.VectorOfPoint currentapproxContour = new Emgu.CV.Util.VectorOfPoint();
                CvInvoke.ApproxPolyDP(currentContour, currentapproxContour, epsilon, closed);
                approxPixelsContours.Push(currentapproxContour);
            }



            hierarchies = new List<int[]>();
            for (int contourIdx = 0; contourIdx < approxPixelsContours.Size; contourIdx++)
            {
                int[] hierarcyOfCurrentCountour = GetHierarchy(hierarchyMat, contourIdx);
                hierarchies.Add(hierarcyOfCurrentCountour);
            }

            Point[][] pixelsContoursArray = pixelsContours.ToArrayOfArray();
            approxPixelsContoursArray = approxPixelsContours.ToArrayOfArray();
        }

        public Emgu.CV.Util.VectorOfVectorOfPoint CalcPixelsContours(double cameraX, double cameraY, double plateHeightAbovePoint, double epsilon)
        {
            List<int[]> hierarchies;
            Point[][] pixelsContoursArray;
            Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours;

            //string fileName = "D:/Developments/OpenGL/EmguCV/ClientAndServer/Server_csharp/images/screenShot.bmp";
            string fileName = "../../../Server_csharp/images/screenShot.bmp";
            imgInput = new Image<Bgr, byte>(fileName);
            Image<Gray, byte> binaryImage = convertRgbToBinaryImage(imgInput);
            calcPixelContours(binaryImage, out pixelsContours, out pixelsContoursArray, out hierarchies);
            return pixelsContours;
        }

        private Image<Bgr, byte> GetImageWithPixelsContours(Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours, int threshold)
        {
            Random rand = new Random();
            Image<Bgr, byte> imgWithContours = new Image<Bgr, byte>(imgInput.Width, imgInput.Height, new Bgr(255, 255, 255));
            for (int contourIdx = 0; contourIdx < pixelsContours.Size; contourIdx++)
            {
                if (pixelsContours[contourIdx].Size < threshold)
                {
                    continue;
                }

                double blue = rand.Next(0, 231);
                double green = rand.Next(0, 231);
                double red = rand.Next(0, 231);
                int thickness = 5;
                CvInvoke.DrawContours(imgWithContours, pixelsContours, contourIdx, new MCvScalar(blue, green, red), thickness);
            }
            return imgWithContours;
        }

        private void convertPixelToWorld3DPoint(float pixel_x, float pixel_y, out float world_x, out float world_y)
        {
            world_x = pixel_x;
            world_y = pixel_y;
        }

        private List<Matrix<float>> ConvertPixelsContoursToWorldPoints(Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours, float plateHeight)
        {
            float world_x;
            float world_y;

            List<Matrix<float>> worldPoints = new List<Matrix<float>>();
            for (int i=0; i<pixelsContours.Size; i++)
            {
                System.Drawing.Point[] currentContoursArray = pixelsContours[i].ToArray();
                int numOfPoints = currentContoursArray.Length;
                Matrix<float> singleContourWorldPoints = new Matrix<float>(numOfPoints, 3);

                for (int j=0; j < numOfPoints; j++)
                {
                    int pixel_x = currentContoursArray[j].X;
                    int pixel_y = currentContoursArray[j].Y;
                    convertPixelToWorld3DPoint(pixel_x, pixel_y, out world_x, out world_y);

                    singleContourWorldPoints[j, 0] = world_x;
                    singleContourWorldPoints[j, 1] = world_y;
                    singleContourWorldPoints[j, 2] = plateHeight;
                }
                float[,] elements = singleContourWorldPoints.Data;
                worldPoints.Add(singleContourWorldPoints);
            }
            return worldPoints;
        }


        public List<Emgu.CV.Matrix<float>> CalculateWorldPoints(double cameraX, double cameraY, double cameraZ, double plateHeightAbovePoint, double epsilon)
        {
            //List<Emgu.CV.Matrix<float>> worldPoints = new List<Emgu.CV.Matrix<float>>();
            //List<Emgu.CV.Matrix<float>> worldPoints = createDummyMatrix();
            Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours = CalcPixelsContours(cameraX, cameraY, plateHeightAbovePoint, epsilon);
            int contoursThreshold = 10; //contours with less vertices than contoursThreshold will not be rendered to image
            Image<Bgr, byte> imageWithContours = GetImageWithPixelsContours(pixelsContours, contoursThreshold);
            double plateHeight = 1000.0;
            List<Matrix<float>> worldPoints = ConvertPixelsContoursToWorldPoints(pixelsContours, (float)plateHeight);

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
