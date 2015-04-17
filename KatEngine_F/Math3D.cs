using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KatEngine_F
{

    public class Quaternion
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }
        public double W { get; set; }

        public Quaternion() { }

        public Quaternion(double x, double y, double z, double angle)
        {
            W = Math.Cos(angle);
            double sin = Math.Sin(angle);
            X = x * sin;
            Y = y * sin;
            Z = z * sin;
        }

        public static Quaternion FromValues(double x, double y, double z, double w)
        {
            Quaternion q = new Quaternion();
            q.X = x;
            q.Y = y;
            q.Z = z;
            q.W = w;
            return q;
        }

        public static Quaternion FromEuler(double Y, double Z, double X)
        {
            Quaternion q = new Quaternion();

            double sY = Math.Sin(Y * 0.5);
            double cY = Math.Cos(Y * 0.5);
            double sZ = Math.Sin(Z * 0.5);
            double cZ = Math.Cos(Z * 0.5);
            double sX = Math.Sin(X * 0.5);
            double cX = Math.Cos(X * 0.5);

            //Le nasty formula 1
            q.W = cY * cZ * cX - sY * sZ * sX;
            q.X = sY * sZ * cX + cY * cZ * sX;
            q.Y = sY * cZ * cX + cY * sZ * sX;
            q.Z = cY * sZ * cX - sY * cZ * sX;

            return q;
        }

        public static Quaternion operator *(Quaternion a, Quaternion b)
        {
            Quaternion res = new Quaternion();

            //Le nasty formula 2
            res.W = a.W * b.W - a.X * b.X - a.Y * b.Y - a.Z * b.Z;
            res.X = a.W * b.X + a.X * b.W + a.Y * b.Z - a.Z * b.Y;
            res.Y = a.W * b.Y - a.X * b.Z + a.Y * b.W + a.Z * b.X;
            res.Z = a.W * b.Z + a.X * b.Y - a.Y * b.X + a.Z * b.W;

            return res;
        }

        public void Normalize()
        {
            double length = Math.Sqrt(X * X + Y * Y + Z * Z + W * W);
            X /= length;
            Y /= length;
            Z /= length;
            W /= length;
        }

        public static Quaternion operator ~(Quaternion a)
        {
            a.X *= -1;
            a.Y *= -1;
            a.Z *= -1;
            return a;
        }

        public static Quaternion I = new Quaternion(0, 0, 0, 1);

        public Matrix ToRMatrix()
        {
            Matrix res = new Matrix(4);

            res.linefirst = false;

            res[0] = 1 - 2 * (Y * Y + Z * Z);
            res[1] = 2 * (X * Y + Z * W);
            res[2] = 2 * (X * Z - Y * W);
            res[3] = 0;

            // Second Column
            res[4] = 2 * (X * Y - Z * W);
            res[5] = 1 - 2 * (X * X + Z * Z);
            res[6] = 2 * (Z * Y + X * W);
            res[7] = 0;

            // Third Column
            res[8] = 2 * (X * Z + Y * W);
            res[9] = 2 * (Y * Z - X * W);
            res[10] = 1 - 2 * (X * X + Y * Y);
            res[11] = 0;

            // Fourth Column
            res[12] = 0;
            res[13] = 0;
            res[14] = 0;
            res[15] = 1;

            res.linefirst = true;

            return res;
        }
    }

    public class Transformation
    {

        public static Matrix Scale(double x, double y, double z)
        {
            Matrix res = new Matrix(4);
            res[0] = x;
            res[5] = y;
            res[10] = z;
            res[15] = 1;
            return res;
        }

        public static Matrix Translate(double x, double y, double z)
        {
            Matrix res = Matrix.I(4);
            res[3] = x;
            res[7] = y;
            res[11] = z;

            return res;
        }

        public static Matrix Projection(double FovX, double FovY, double Znear, double Zfar)
        {
            Matrix res = new Matrix(4);

            res[0] = Math.Atan(FovX / 2f);
            res[5] = Math.Atan(FovY / 2f);
            res[10] = -(2f / (Zfar - Znear));
            res[11] = (Zfar + Znear) / (Zfar - Znear);
            res[15] = 1;

            return res;
        }
    }

    public class Matrix
    {
        public bool linefirst = true;
        double[,] arr;
        public int Nx { get; set; }
        public int Ny { get; set; }

        public Matrix(int n)
        {
            arr = new double[n, n];
            Nx = Ny = n;
        }

        public Matrix(int nx, int ny)
        {
            arr = new double[ny, nx];
            Nx = nx;
            Ny = ny;
        }

        public double this[int i]
        {
            get
            {
                if (linefirst)
                    return arr[i / Nx, i % Nx];
                else 
                    return arr[i % Ny, i / Ny];
            }
            set
            {
                if (linefirst)
                    arr[i / Nx, i % Nx] = value;
                else
                    arr[i % Ny, i / Ny] = value;
            }
        }


        public double this[int y, int x]
        {
            get
            {
                return arr[y, x];
            }
            set
            {
                arr[y, x] = value;
            }
        }

        /// <summary>
        /// Manually set values
        /// </summary>
        /// <param name="linefirst">row-major/column-major -> order of elements</param>
        /// <param name="values"></param>
        public void SetValues(params double[] values)
        {
            for (int i = 0; i < values.Length; i++)
            {
                if (linefirst)
                    arr[i / Nx, i % Nx] = values[i];
                else
                    arr[i % Ny, i / Ny] = values[i];
            }
        }

        public static Matrix operator *(Matrix a, Matrix b)
        {
            if (a.Nx != b.Ny) return null;
            Matrix res = new Matrix(b.Nx, a.Ny);

            for (int y = 0; y < res.Ny; y++)
            {
                for (int x = 0; x < res.Nx; x++)
                {
                    double temp = 0;
                    for (int k = 0; k < a.Nx; k++)
                    {
                        temp += a[y, k] * b[k, x];
                    }
                    res[y, x] = temp;
                }
            }

            return res;

        }

        public static Point3 operator *(Matrix a, Point3 b)
        {
            var res = a * (Matrix)((HPoint3)b);

            return new Point3(res[0], res[1], res[2]);
        }

        public static explicit operator Matrix(HPoint3 a)
        {
            Matrix m = new Matrix(1, 4);
            m[0] = a.X;
            m[1] = a.Y;
            m[2] = a.Z;
            m[3] = a.W;

            return m;
        }

        public static implicit operator HPoint3(Matrix a)
        {
            HPoint3 m = new HPoint3(a[0], a[1], a[2], a[3]);

            return m;
        }


        public static Matrix operator !(Matrix a)
        {
            Matrix res = new Matrix(a.Nx, a.Ny);

            for (int y = 0; y < res.Ny; y++)
            {
                for (int x = 0; x < res.Nx; x++)
                {
                    res[x, y] = a[y, x];
                }
            }

            return res;
        }

        public static Matrix I(int n)
        {
            Matrix res = new Matrix(n);

            for (int i = 0; i < n; i++)
            {
                res[i, i] = 1;
            }
            return res;
        }

        public override string ToString()
        {
            string res = "";
            for (int y = 0; y <  Ny; y++)
            {
                for (int x = 0; x < Nx; x++)
                {
                    res += this[y, x] + " ";
                }
                res += "|";
            }

            return res;
        }


    }

    public class HPoint3
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }
        public double W { get; set; }

        public HPoint3()
        {

        }

        public HPoint3(double X, double Y, double Z, double W)
        {
            this.X = X; this.Y = Y; this.Z = Z; this.W = W;
        }

        public static implicit operator HPoint3(Point3 a)
        {
            return new HPoint3(a.X, a.Y, a.Z, 1);
        }

        public static implicit operator Point3(HPoint3 a)
        {
            return new Point3(a.X/a.W, a.Y/a.W, a.Z/a.W);
        }
    }

    public class Point3
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }

        public Point3()
        {

        }

        public Point3(double X, double Y, double Z)
        {
            this.X = X; this.Y = Y; this.Z = Z;
        }

        public static Point3 operator *(Point3 a, double c)
        {
            return new Point3(a.X * c, a.Y * c, a.Z * c);
        }

        public static Point3 operator /(Point3 a, double c)
        {
            return new Point3(a.X / c, a.Y / c, a.Z / c);
        }

        public static Point3 operator +(Point3 a, Point3 c)
        {
            return new Point3(a.X + c.X, a.Y + c.Y, a.Z + c.Z);
        }

        public override string ToString()
        {
            return "X:" + X + " Y:" + Y + " Z:" + Z;
        }
    }
}
