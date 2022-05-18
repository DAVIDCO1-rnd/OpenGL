using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Emgu.CV;
using Emgu.CV.Structure;

namespace tutorial1
{
    public partial class UserControl1: UserControl
    {
        Random rand = new Random();
        Image<Bgr, byte> imgInput;
        Image<Bgr, byte> imgCopy;
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
                imgCopy = imgInput.Copy();
                pictureBox1.Image = imgInput.Bitmap;
                Image<Bgr, byte> imgout = findContours();                                
                pictureBox2.Image = imgout.Bitmap;
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

        private void convertPixelToWorld(float pixel_x, float pixel_y, float center_x, float center_y, int width, int height, float radius, out float world_x, out float world_y)
        {
            //I assume here that the height of the image = 2 * radius
            float aspect = (float)imgInput.Width / (float)imgInput.Height;

            float min_pixel_x = 0.0f;
            float max_pixel_x = (float)width - 1;
            float min_world_x = center_x - radius * aspect;
            float max_world_x = center_x + radius * aspect;

            float min_pixel_y = 0.0f;
            float max_pixel_y = (float)height - 1;
            float min_world_y = center_y - radius;
            float max_world_y = center_y + radius;

            float ratio_x = (pixel_x - min_pixel_x) / (max_pixel_x - min_pixel_x);
            float ratio_y = (pixel_y - min_pixel_y) / (max_pixel_y - min_pixel_y);
            world_x = min_world_x + ratio_x * (max_world_x - min_world_x);
            world_y = min_world_y + ratio_y * (max_world_y - min_world_y);
        }

        private Image<Bgr, byte> findContours()
        {
            Image<Gray, byte> imgOutput = imgInput.Convert<Gray, byte>().ThresholdBinary(new Gray(100), new Gray(255));
            Emgu.CV.Util.VectorOfVectorOfPoint contours = new Emgu.CV.Util.VectorOfVectorOfPoint();
            Mat hierarchyMat = new Mat();

            Image<Bgr, byte> imgout = new Image<Bgr, byte>(imgInput.Width, imgInput.Height, new Bgr(255, 255, 255));

            CvInvoke.FindContours(imgOutput, contours, hierarchyMat, Emgu.CV.CvEnum.RetrType.Tree, Emgu.CV.CvEnum.ChainApproxMethod.ChainApproxSimple);

            Point[][] contoursArray = contours.ToArrayOfArray();

            float radius = 10000.0f;
            float z = 500.0f;

            float center_x = 0.0f;
            float center_y = 0.0f;

            float world_x;
            float world_y;

            List<Matrix<float>> worldPoints = new List<Matrix<float>>();
            for (int i = 0; i < contours.Size; i++)
            {
                Point[] currentContoursArray = contours[i].ToArray();
                int numOfPoints = currentContoursArray.Length;
                Matrix<float> singleContourWorldPoints = new Matrix<float>(numOfPoints, 3);

                for (int j=0; j<numOfPoints; j++)
                {
                    int pixel_x = currentContoursArray[j].X;
                    int pixel_y = currentContoursArray[j].Y;
                    convertPixelToWorld(pixel_x, pixel_y, center_x, center_y, imgInput.Width, imgInput.Height, radius, out world_x, out world_y);

                    singleContourWorldPoints[j, 0] = world_x;
                    singleContourWorldPoints[j, 1] = world_y;
                    singleContourWorldPoints[j, 2] = z;                    
                }
                float[,] elements = singleContourWorldPoints.Data;
                worldPoints.Add(singleContourWorldPoints);
            }
            


            for (int contourIdx = 0; contourIdx < contours.Size; contourIdx++)
            {
                int[] heirarcyOfCurrentCountour = GetHierarchy(hierarchyMat, contourIdx);
                double blue = rand.Next(0, 231);
                double green = rand.Next(0, 231);
                double red = rand.Next(0, 231);
                int thickness = 5;
                CvInvoke.DrawContours(imgCopy, contours, contourIdx, new MCvScalar(blue, green, red), thickness);
            }
            return imgCopy;
        }

        private void findContoursToolStripMenuItem_Click(object sender, EventArgs e)
        {
            pictureBox2.SizeMode = PictureBoxSizeMode.StretchImage;
            Image<Bgr, byte> imgout = findContours();
            pictureBox2.Image = imgout.Bitmap;
        }
    }
}
