using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KatEngine_F
{
    public partial class ViewPort : UserControl
    {
      
        public ViewPort()
        {
            InitializeComponent();
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw | ControlStyles.UserPaint, true);

            double m = 5;
            cube = new Point3[8];
            cube[0] = new Point3(0, 0, 0) * m;
            cube[1] = new Point3(0, 0, 1) * m;
            cube[2] = new Point3(0, 1, 1) * m;
            cube[3] = new Point3(0, 1, 0) * m;
            cube[4] = new Point3(1, 1, 0) * m;
            cube[5] = new Point3(1, 1, 1) * m;
            cube[6] = new Point3(1, 0, 1) * m;
            cube[7] = new Point3(1, 0, 0) * m;

            fcube = new PointF[8];

            Matrix projectionMatrix = Transformation.Projection((120).ToRadians(), (120).ToRadians(), 0.001, 100);

            Quaternion rot = new Quaternion(0.707, 0.707, 0, (0).ToRadians());
            Matrix ViewMatrix = Transformation.Translate(-1,-5,0) * rot.ToRMatrix();

            for (int i = 0; i < 8; i++)
            {
                fcube[i] = Camera.ProjectPoint(cube[i], projectionMatrix, ViewMatrix, 100, 100);
            }


        }

        public void Rotate(double degrees)
        {
            Matrix projectionMatrix = Transformation.Projection((120).ToRadians(), (120).ToRadians(), 0.001, 100);

            Quaternion rot = new Quaternion(1, 1, 1, (degrees).ToRadians());
            rot.Normalize();
            Matrix ViewMatrix = Transformation.Translate(0, -5, 0) * rot.ToRMatrix();

            for (int i = 0; i < 8; i++)
            {
                fcube[i] = Camera.ProjectPoint(cube[i], projectionMatrix, ViewMatrix, 100, 100);
            }

            Invalidate();
        }

        //Camera cam;
        PointF[] fcube;
        Point3[] cube;
        Graphics g;

        protected override void OnPaint(PaintEventArgs e)
        {
            g = e.Graphics;

            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            for (int i = 0; i < 8; i++)
            {
                g.FillEllipse(Brushes.SkyBlue, fcube[i].X - 2, fcube[i].Y - 2, 4, 4);
            }

            drawLine(0, 1);
            drawLine(0, 7);
            drawLine(0, 3);

            drawLine(1, 2);
            drawLine(1, 6);

            drawLine(2, 3);
            drawLine(2, 5);

            drawLine(3, 4);

            drawLine(4, 5);
            drawLine(4, 7);

            drawLine(5, 6);

            drawLine(6, 7);
            
        }

        void drawLine(int a, int b)
        {
            g.DrawLine(Pens.SkyBlue, fcube[a], fcube[b]);
        }
    }
}
