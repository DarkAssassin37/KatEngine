using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace KatEngine_F
{
    public class Camera
    {
        public double near=0.001, far=100;
        public double angleOfView { get; set; }
        public double aspectRatio { get; set; }

        public Matrix Projection;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="fov">in degrees</param>
        /// <param name="aspectRatio"></param>
        public Camera(double fov, double aspectRatio)
        {
            angleOfView = fov.ToRadians();
            this.aspectRatio = aspectRatio;

            Projection = CreatePMatrix();
        }

        Matrix CreatePMatrix2()
        {
            Matrix matrix = new Matrix(4);
            matrix.linefirst = false;
            // Some calculus before the formula.
            double size = near * Math.Tan(angleOfView / 2.0);
            double left = -size, right = size, bottom = -size / aspectRatio, top = size / aspectRatio;

            // First Column
            matrix[0] = 2 * near / (right - left);
            matrix[1] = 0.0;
            matrix[2] = 0.0;
            matrix[3] = 0.0;

            // Second Column
            matrix[4] = 0.0;
            matrix[5] = 2 * near / (top - bottom);
            matrix[6] = 0.0;
            matrix[7] = 0.0;

            // Third Column
            matrix[8] = (right + left) / (right - left);
            matrix[9] = (top + bottom) / (top - bottom);
            matrix[10] = -(far + near) / (far - near);
            matrix[11] = -1;

            // Fourth Column
            matrix[12] = 0.0;
            matrix[13] = 0.0;
            matrix[14] = -(2 * far * near) / (far - near);
            matrix[15] = 0.0;

            matrix.linefirst = true;
            return matrix;

        }

        public Matrix CreatePMatrix()
        {
            Matrix matrix = new Matrix(4);

            matrix[0] = Math.Atan(angleOfView / 2.0);
            matrix[1] = 0.0;
            matrix[2] = 0.0;
            matrix[3] = 0.0;

            matrix[4] = 0.0;
            matrix[5] = Math.Atan(angleOfView / 2.0);
            matrix[6] = 0.0;
            matrix[7] = 0.0;

            matrix[8] = 0.0;
            matrix[9] = 0.0;
            matrix[10] = -(2 / (far - near));
            matrix[11] = (far + near) / (far - near);

            matrix[12] = 0.0;
            matrix[13] = 0.0;
            matrix[14] = 0.0;
            matrix[15] = 1.0;

            return matrix;

        }

        public Matrix CreatePMatrix3()
        {
            Matrix matrix = new Matrix(4);

            matrix[0] = Math.Atan(angleOfView / 2.0);
            matrix[1] = 0.0;
            matrix[2] = 0.0;
            matrix[3] = 0.0;

            matrix[4] = 0.0;
            matrix[5] = Math.Atan(angleOfView / 2.0);
            matrix[6] = 0.0;
            matrix[7] = 0.0;

            matrix[8] = 0.0;
            matrix[9] = 0.0;
            matrix[10] = -(2 / (far - near));
            matrix[11] = (far + near) / (far - near);

            matrix[12] = 0.0;
            matrix[13] = 0.0;
            matrix[14] = 0.0;
            matrix[15] = 1.0;

            return matrix;

        }

        /*public Point3 ProjectPoint(Point3 p)
        {
            HPoint3 hp = p;
            var res = Projection * hp;
            return (Point3)res * (1/res.W);
        }*/

        public static PointF ProjectPoint(Point3 p, Matrix Projection, Matrix View, int width, int height)
        {
            Matrix viewProjectionMatrix = Projection * View;

            p = viewProjectionMatrix * p;
            
            float winX = (float)(((p.X + 1) / 2.0) *
                                         width);

            float winY = (float)((1 - ((p.Y + 1) / 2.0)) *
                                         height);
            return new PointF(winX, winY);
        }
    }

    public static class Extensions
    {
        public static double ToRadians(this double a)
        {
            return Math.PI / 180 * a;
        }

        public static double ToRadians(this int a)
        {
            return Math.PI / 180 * a;
        }
    }
}
