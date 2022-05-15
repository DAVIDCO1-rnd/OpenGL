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
        public UserControl1()
        {
            InitializeComponent();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;

            OpenFileDialog ofd = new OpenFileDialog();
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                imgInput = new Image<Bgr, byte>(ofd.FileName);
                pictureBox1.Image = imgInput.Bitmap;                
            }
        }

        private void findContoursToolStripMenuItem_Click(object sender, EventArgs e)
        {
            pictureBox2.SizeMode = PictureBoxSizeMode.StretchImage;
            Image<Gray, byte> imgOutput = imgInput.Convert<Gray, byte>().ThresholdBinary(new Gray(100), new Gray(255));
            Emgu.CV.Util.VectorOfVectorOfPoint contours = new Emgu.CV.Util.VectorOfVectorOfPoint();
            Mat hier = new Mat();

            Image<Bgr, byte> imgout = new Image<Bgr, byte>(imgInput.Width, imgInput.Height, new Bgr(255, 255, 255));

            CvInvoke.FindContours(imgOutput, contours, hier, Emgu.CV.CvEnum.RetrType.External, Emgu.CV.CvEnum.ChainApproxMethod.ChainApproxSimple);
            Point[][] contours1 = contours.ToArrayOfArray();
            for (int i=0; i< contours.Size; i++)
            {
                double blue = rand.Next(0, 129);
                double green = rand.Next(0, 129);
                double red = rand.Next(0, 129);
                CvInvoke.DrawContours(imgout, contours, i, new MCvScalar(blue, green, red));
            }
            
            

            pictureBox2.Image = imgout.Bitmap;
        }
    }
}
