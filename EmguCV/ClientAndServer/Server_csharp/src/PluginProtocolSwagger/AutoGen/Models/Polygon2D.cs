using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Sharpility.Extensions;
using NodaTime;

namespace EOSimU.API.AutoGen.v1alpha3.Models
{
    /// <summary>
    /// Polygon2D
    /// </summary>
    public sealed class Polygon2D:  IEquatable<Polygon2D>
    { 
        /// <summary>
        /// Vertices
        /// </summary>
        public List<Pixel> Vertices { get; private set; }


        /// <summary>
        /// Empty constructor required by some serializers.
        /// Use Polygon2D.Builder() for instance creation instead.
        /// </summary>
        [Obsolete]
        public Polygon2D()
        {
        }

        private Polygon2D(List<Pixel> Vertices)
        {
            
            this.Vertices = Vertices;
            
        }

        /// <summary>
        /// Returns builder of Polygon2D.
        /// </summary>
        /// <returns>Polygon2DBuilder</returns>
        public static Polygon2DBuilder Builder()
        {
            return new Polygon2DBuilder();
        }

        /// <summary>
        /// Returns Polygon2DBuilder with properties set.
        /// Use it to change properties.
        /// </summary>
        /// <returns>Polygon2DBuilder</returns>
        public Polygon2DBuilder With()
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

        public bool Equals(Polygon2D other)
        {
            return Equals((object) other);
        }

        public override int GetHashCode()
        {
            return this.PropertiesHash();
        }

        /// <summary>
        /// Implementation of == operator for (Polygon2D.
        /// </summary>
        /// <param name="left">Compared (Polygon2D</param>
        /// <param name="right">Compared (Polygon2D</param>
        /// <returns>true if compared items are equals, false otherwise</returns>
        public static bool operator == (Polygon2D left, Polygon2D right)
        {
            return Equals(left, right);
        }

        /// <summary>
        /// Implementation of != operator for (Polygon2D.
        /// </summary>
        /// <param name="left">Compared (Polygon2D</param>
        /// <param name="right">Compared (Polygon2D</param>
        /// <returns>true if compared items are not equals, false otherwise</returns>
        public static bool operator != (Polygon2D left, Polygon2D right)
        {
            return !Equals(left, right);
        }

        /// <summary>
        /// Builder of Polygon2D.
        /// </summary>
        public sealed class Polygon2DBuilder
        {
            private List<Pixel> _Vertices;

            internal Polygon2DBuilder()
            {
                SetupDefaults();
            }

            private void SetupDefaults()
            {
            }

            /// <summary>
            /// Sets value for Polygon2D.Vertices property.
            /// </summary>
            /// <param name="value">Vertices</param>
            public Polygon2DBuilder Vertices(List<Pixel> value)
            {
                _Vertices = value;
                return this;
            }


            /// <summary>
            /// Builds instance of Polygon2D.
            /// </summary>
            /// <returns>Polygon2D</returns>
            public Polygon2D Build()
            {
                Validate();
                return new Polygon2D(
                    Vertices: _Vertices
                );
            }

            private void Validate()
            { 
                if (_Vertices == null)
                {
                    throw new ArgumentException("Vertices is a required property for Polygon2D and cannot be null");
                } 
            }
        }

        
    }
}