using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

using Emgu.CV;
using Emgu.CV.Structure;

namespace tutorial1
{
    public partial class UserControl1: UserControl
    {
        Random rand = new Random();
        Image<Bgr, byte> imgInput;
        Image<Bgr, byte> imgWithContours;
        public UserControl1()
        {
            InitializeComponent();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox2.SizeMode = PictureBoxSizeMode.StretchImage;

            OpenFileDialog ofd = new OpenFileDialog();
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                imgInput = new Image<Bgr, byte>(ofd.FileName);
                imgWithContours = imgInput.Copy();
                pictureBox1.Image = imgInput.Bitmap;
                calcContoursAndDisplay();
            }
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

        //public Image<Bgr, byte> CreateImageFromBytesArray(byte[] bytes)
        //{
        //    Image<Bgr, byte> image = new Image<Bgr, byte>(width, height);
        //    image.Bytes = bytes;
        //    return image;
        //}

        private void convertPixelToWorld3DPoint(float pixel_x, float pixel_y, float center_x, float center_y, int width, int height, float radius, out float world_x, out float world_y)
        {
            float aspect;

            float min_pixel_x = 0.0f;
            float max_pixel_x = (float)width - 1;
            float min_world_x;
            float max_world_x;

            float min_pixel_y = 0.0f;
            float max_pixel_y = (float)height - 1;
            float min_world_y;
            float max_world_y;


            if (width >= height)
            {
                //I assume here that the height of the image = 2 * radius
                aspect = (float)width / (float)height;
                min_world_x = center_x - radius * aspect;
                max_world_x = center_x + radius * aspect;
                min_world_y = center_y - radius;
                max_world_y = center_y + radius;
            }
            else //height > width
            {
                //I assume here that the width of the image = 2 * radius
                aspect = (float)height / (float)width;
                min_world_x = center_x - radius;
                max_world_x = center_x + radius;
                min_world_y = center_y - radius * aspect;
                max_world_y = center_y + radius * aspect;
            }

            float ratio_x = (pixel_x - min_pixel_x) / (max_pixel_x - min_pixel_x);
            float ratio_y = (pixel_y - min_pixel_y) / (max_pixel_y - min_pixel_y);
            world_x = min_world_x + ratio_x * (max_world_x - min_world_x);
            world_y = min_world_y + ratio_y * (max_world_y - min_world_y);
        }

        Image<Gray, byte> convertRgbToBinaryImage(Image<Bgr, byte> imgInput)
        {
            Image<Gray, byte> binaryImage = imgInput.Convert<Gray, byte>().ThresholdBinary(new Gray(100), new Gray(255));
            return binaryImage;
        }



        private void calcPixelContours(Image<Gray, byte> binaryImage, out Emgu.CV.Util.VectorOfVectorOfPoint approxPixelsContours, out Point[][] approxPixelsContoursArray, out List<int[]> hierarchies)
        {
            Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours = new Emgu.CV.Util.VectorOfVectorOfPoint();
            Mat hierarchyMat = new Mat();
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

        
        private Image<Bgr, byte> createImageWithContours(Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours)
        {
            Image<Bgr, byte> imgWithContours = new Image<Bgr, byte>(imgInput.Width, imgInput.Height, new Bgr(255, 255, 255));
            for (int contourIdx = 0; contourIdx < pixelsContours.Size; contourIdx++)
            {
                double blue = rand.Next(0, 231);
                double green = rand.Next(0, 231);
                double red = rand.Next(0, 231);
                int thickness = 5;
                CvInvoke.DrawContours(imgWithContours, pixelsContours, contourIdx, new MCvScalar(blue, green, red), thickness);
            }
            return imgWithContours;
        }

        
        private List<Matrix<float>> convertPixelsPointsToWorld3DPoints(Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours, float radius, float z, float center_x, float center_y)
        {
            float world_x;
            float world_y;

            List<Matrix<float>> worldPoints = new List<Matrix<float>>();
            for (int i = 0; i < pixelsContours.Size; i++)
            {
                Point[] currentContoursArray = pixelsContours[i].ToArray();
                int numOfPoints = currentContoursArray.Length;
                Matrix<float> singleContourWorldPoints = new Matrix<float>(numOfPoints, 3);

                for (int j = 0; j < numOfPoints; j++)
                {
                    int pixel_x = currentContoursArray[j].X;
                    int pixel_y = currentContoursArray[j].Y;
                    convertPixelToWorld3DPoint(pixel_x, pixel_y, center_x, center_y, imgInput.Width, imgInput.Height, radius, out world_x, out world_y);

                    singleContourWorldPoints[j, 0] = world_x;
                    singleContourWorldPoints[j, 1] = world_y;
                    singleContourWorldPoints[j, 2] = z;
                }
                float[,] elements = singleContourWorldPoints.Data;
                worldPoints.Add(singleContourWorldPoints);
            }
            return worldPoints;
        }

        private void calcContoursAndDisplay()
        {
            float radius = 10000.0f;
            float z = 500.0f;

            float center_x = 0.0f;
            float center_y = 0.0f;


            List<int[]> hierarchies;
            Point[][] pixelsContoursArray;
            Emgu.CV.Util.VectorOfVectorOfPoint pixelsContours;

            DateTime start = DateTime.Now;

            Image<Gray, byte> binaryImage = convertRgbToBinaryImage(imgInput);
            calcPixelContours(binaryImage, out pixelsContours, out pixelsContoursArray, out hierarchies);
            Image<Bgr, byte> imgWithContours = createImageWithContours(pixelsContours);
            List<Matrix<float>> worldPoints = convertPixelsPointsToWorld3DPoints(pixelsContours, radius, z, center_x, center_y);

            DateTime end = DateTime.Now;

            TimeSpan ts = (end - start);
            label1.Text = String.Format("Elapsed Time is {0} ms", ts.TotalMilliseconds);




            pictureBox2.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox2.Image = imgWithContours.Bitmap;
        }

        private void findContoursToolStripMenuItem_Click(object sender, EventArgs e)
        {
            calcContoursAndDisplay();
        }
    }
}
