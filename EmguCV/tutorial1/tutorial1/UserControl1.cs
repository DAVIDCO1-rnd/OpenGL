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

        private Image<Bgr, byte> findContours()
        {
            Image<Gray, byte> imgOutput = imgInput.Convert<Gray, byte>().ThresholdBinary(new Gray(100), new Gray(255));
            Emgu.CV.Util.VectorOfVectorOfPoint contours = new Emgu.CV.Util.VectorOfVectorOfPoint();
            Mat hierarchyMat = new Mat();

            Image<Bgr, byte> imgout = new Image<Bgr, byte>(imgInput.Width, imgInput.Height, new Bgr(255, 255, 255));

            CvInvoke.FindContours(imgOutput, contours, hierarchyMat, Emgu.CV.CvEnum.RetrType.Tree, Emgu.CV.CvEnum.ChainApproxMethod.ChainApproxSimple);

            Point[][] contoursArray = contours.ToArrayOfArray();            
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
