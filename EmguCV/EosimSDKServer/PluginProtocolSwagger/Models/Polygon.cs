using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Sharpility.Extensions;
using NodaTime;

namespace EOSimU.API.AutoGen.v1alpha3.Models
{
    /// <summary>
    /// Polygon
    /// </summary>
    public sealed class Polygon:  IEquatable<Polygon>
    { 
        /// <summary>
        /// Vertices
        /// </summary>
        public List<Point> Vertices { get; private set; }


        /// <summary>
        /// Empty constructor required by some serializers.
        /// Use Polygon.Builder() for instance creation instead.
        /// </summary>
        [Obsolete]
        public Polygon()
        {
        }

        private Polygon(List<Point> Vertices)
        {
            
            this.Vertices = Vertices;
            
        }

        /// <summary>
        /// Returns builder of Polygon.
        /// </summary>
        /// <returns>PolygonBuilder</returns>
        public static PolygonBuilder Builder()
        {
            return new PolygonBuilder();
        }

        /// <summary>
        /// Returns PolygonBuilder with properties set.
        /// Use it to change properties.
        /// </summary>
        /// <returns>PolygonBuilder</returns>
        public PolygonBuilder With()
        {
            return Builder()
                .Vertices(Vertices);
        }

        public override string ToString()
        {
            return this.PropertiesToString();
        }

        public override bool Equals(object obj)
        {
            return this.EqualsByProperties(obj);
        }

        public bool Equals(Polygon other)
        {
            return Equals((object) other);
        }

        public override int GetHashCode()
        {
            return this.PropertiesHash();
        }

        /// <summary>
        /// Implementation of == operator for (Polygon.
        /// </summary>
        /// <param name="left">Compared (Polygon</param>
        /// <param name="right">Compared (Polygon</param>
        /// <returns>true if compared items are equals, false otherwise</returns>
        public static bool operator == (Polygon left, Polygon right)
        {
            return Equals(left, right);
        }

        /// <summary>
        /// Implementation of != operator for (Polygon.
        /// </summary>
        /// <param name="left">Compared (Polygon</param>
        /// <param name="right">Compared (Polygon</param>
        /// <returns>true if compared items are not equals, false otherwise</returns>
        public static bool operator != (Polygon left, Polygon right)
        {
            return !Equals(left, right);
        }

        /// <summary>
        /// Builder of Polygon.
        /// </summary>
        public sealed class PolygonBuilder
        {
            private List<Point> _Vertices;

            internal PolygonBuilder()
            {
                SetupDefaults();
            }

            private void SetupDefaults()
            {
            }

            /// <summary>
            /// Sets value for Polygon.Vertices property.
            /// </summary>
            /// <param name="value">Vertices</param>
            public PolygonBuilder Vertices(List<Point> value)
            {
                _Vertices = value;
                return this;
            }


            /// <summary>
            /// Builds instance of Polygon.
            /// </summary>
            /// <returns>Polygon</returns>
            public Polygon Build()
            {
                Validate();
                return new Polygon(
                    Vertices: _Vertices
                );
            }

            private void Validate()
            { 
                if (_Vertices == null)
                {
                    throw new ArgumentException("Vertices is a required property for Polygon and cannot be null");
                } 
            }
        }

        
    }
}