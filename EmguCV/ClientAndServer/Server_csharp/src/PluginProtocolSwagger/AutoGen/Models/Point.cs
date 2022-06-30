using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Sharpility.Extensions;
using NodaTime;

namespace EOSimU.API.AutoGen.v1alpha3.Models
{
    /// <summary>
    /// Point
    /// </summary>
    public sealed class Point:  IEquatable<Point>
    { 
        /// <summary>
        /// X
        /// </summary>
        public double? X { get; private set; }

        /// <summary>
        /// Y
        /// </summary>
        public double? Y { get; private set; }

        /// <summary>
        /// Z
        /// </summary>
        public double? Z { get; private set; }


        /// <summary>
        /// Empty constructor required by some serializers.
        /// Use Point.Builder() for instance creation instead.
        /// </summary>
        [Obsolete]
        public Point()
        {
        }

        private Point(double? X, double? Y, double? Z)
        {
            
            this.X = X;
            
            this.Y = Y;
            
            this.Z = Z;
            
        }

        /// <summary>
        /// Returns builder of Point.
        /// </summary>
        /// <returns>PointBuilder</returns>
        public static PointBuilder Builder()
        {
            return new PointBuilder();
        }

        /// <summary>
        /// Returns PointBuilder with properties set.
        /// Use it to change properties.
        /// </summary>
        /// <returns>PointBuilder</returns>
        public PointBuilder With()
        {
            return Builder()
                .X(X)
                .Y(Y)
                .Z(Z);
        }

        public override string ToString()
        {
            return this.PropertiesToString();
        }

        public override bool Equals(object obj)
        {
            return this.EqualsByProperties(obj);
        }

        public bool Equals(Point other)
        {
            return Equals((object) other);
        }

        public override int GetHashCode()
        {
            return this.PropertiesHash();
        }

        /// <summary>
        /// Implementation of == operator for (Point.
        /// </summary>
        /// <param name="left">Compared (Point</param>
        /// <param name="right">Compared (Point</param>
        /// <returns>true if compared items are equals, false otherwise</returns>
        public static bool operator == (Point left, Point right)
        {
            return Equals(left, right);
        }

        /// <summary>
        /// Implementation of != operator for (Point.
        /// </summary>
        /// <param name="left">Compared (Point</param>
        /// <param name="right">Compared (Point</param>
        /// <returns>true if compared items are not equals, false otherwise</returns>
        public static bool operator != (Point left, Point right)
        {
            return !Equals(left, right);
        }

        /// <summary>
        /// Builder of Point.
        /// </summary>
        public sealed class PointBuilder
        {
            private double? _X;
            private double? _Y;
            private double? _Z;

            internal PointBuilder()
            {
                SetupDefaults();
            }

            private void SetupDefaults()
            {
            }

            /// <summary>
            /// Sets value for Point.X property.
            /// </summary>
            /// <param name="value">X</param>
            public PointBuilder X(double? value)
            {
                _X = value;
                return this;
            }

            /// <summary>
            /// Sets value for Point.Y property.
            /// </summary>
            /// <param name="value">Y</param>
            public PointBuilder Y(double? value)
            {
                _Y = value;
                return this;
            }

            /// <summary>
            /// Sets value for Point.Z property.
            /// </summary>
            /// <param name="value">Z</param>
            public PointBuilder Z(double? value)
            {
                _Z = value;
                return this;
            }


            /// <summary>
            /// Builds instance of Point.
            /// </summary>
            /// <returns>Point</returns>
            public Point Build()
            {
                Validate();
                return new Point(
                    X: _X,
                    Y: _Y,
                    Z: _Z
                );
            }

            private void Validate()
            { 
            }
        }

        
    }
}