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
        /// Latitude
        /// </summary>
        public double? Latitude { get; private set; }

        /// <summary>
        /// Y
        /// </summary>
        public double? Y { get; private set; }

        /// <summary>
        /// Height
        /// </summary>
        public double? Height { get; private set; }


        /// <summary>
        /// Empty constructor required by some serializers.
        /// Use Point.Builder() for instance creation instead.
        /// </summary>
        [Obsolete]
        public Point()
        {
        }

        private Point(double? Latitude, double? Y, double? Height)
        {
            
            this.Latitude = Latitude;
            
            this.Y = Y;
            
            this.Height = Height;
            
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
                .Latitude(Latitude)
                .Y(Y)
                .Height(Height);
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
            private double? _Latitude;
            private double? _Y;
            private double? _Height;

            internal PointBuilder()
            {
                SetupDefaults();
            }

            private void SetupDefaults()
            {
            }

            /// <summary>
            /// Sets value for Point.Latitude property.
            /// </summary>
            /// <param name="value">Latitude</param>
            public PointBuilder Latitude(double? value)
            {
                _Latitude = value;
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
            /// Sets value for Point.Height property.
            /// </summary>
            /// <param name="value">Height</param>
            public PointBuilder Height(double? value)
            {
                _Height = value;
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
                    Latitude: _Latitude,
                    Y: _Y,
                    Height: _Height
                );
            }

            private void Validate()
            { 
                if (_Latitude == null)
                {
                    throw new ArgumentException("Latitude is a required property for Point and cannot be null");
                } 
            }
        }

        
    }
}