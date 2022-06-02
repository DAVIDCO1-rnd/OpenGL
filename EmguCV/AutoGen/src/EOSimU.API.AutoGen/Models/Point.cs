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
        /// Longitude
        /// </summary>
        public double? Longitude { get; private set; }

        /// <summary>
        /// Altitude
        /// </summary>
        public double? Altitude { get; private set; }


        /// <summary>
        /// Empty constructor required by some serializers.
        /// Use Point.Builder() for instance creation instead.
        /// </summary>
        [Obsolete]
        public Point()
        {
        }

        private Point(double? Latitude, double? Longitude, double? Altitude)
        {
            
            this.Latitude = Latitude;
            
            this.Longitude = Longitude;
            
            this.Altitude = Altitude;
            
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
                .Longitude(Longitude)
                .Altitude(Altitude);
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
            private double? _Longitude;
            private double? _Altitude;

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
            /// Sets value for Point.Longitude property.
            /// </summary>
            /// <param name="value">Longitude</param>
            public PointBuilder Longitude(double? value)
            {
                _Longitude = value;
                return this;
            }

            /// <summary>
            /// Sets value for Point.Altitude property.
            /// </summary>
            /// <param name="value">Altitude</param>
            public PointBuilder Altitude(double? value)
            {
                _Altitude = value;
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
                    Longitude: _Longitude,
                    Altitude: _Altitude
                );
            }

            private void Validate()
            { 
                if (_Latitude == null)
                {
                    throw new ArgumentException("Latitude is a required property for Point and cannot be null");
                } 
                if (_Longitude == null)
                {
                    throw new ArgumentException("Longitude is a required property for Point and cannot be null");
                } 
                if (_Altitude == null)
                {
                    throw new ArgumentException("Altitude is a required property for Point and cannot be null");
                } 
            }
        }

        
    }
}