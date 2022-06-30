using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Sharpility.Extensions;
using NodaTime;

namespace EOSimU.API.AutoGen.v1alpha3.Models
{
    /// <summary>
    /// Pixel
    /// </summary>
    public sealed class Pixel:  IEquatable<Pixel>
    { 
        /// <summary>
        /// X
        /// </summary>
        public decimal? X { get; private set; }

        /// <summary>
        /// Y
        /// </summary>
        public decimal? Y { get; private set; }


        /// <summary>
        /// Empty constructor required by some serializers.
        /// Use Pixel.Builder() for instance creation instead.
        /// </summary>
        [Obsolete]
        public Pixel()
        {
        }

        private Pixel(decimal? X, decimal? Y)
        {
            
            this.X = X;
            
            this.Y = Y;
            
        }

        /// <summary>
        /// Returns builder of Pixel.
        /// </summary>
        /// <returns>PixelBuilder</returns>
        public static PixelBuilder Builder()
        {
            return new PixelBuilder();
        }

        /// <summary>
        /// Returns PixelBuilder with properties set.
        /// Use it to change properties.
        /// </summary>
        /// <returns>PixelBuilder</returns>
        public PixelBuilder With()
        {
            return Builder()
                .X(X)
                .Y(Y);
        }

        public override string ToString()
        {
            return this.PropertiesToString();
        }

        public override bool Equals(object obj)
        {
            return this.EqualsByProperties(obj);
        }

        public bool Equals(Pixel other)
        {
            return Equals((object) other);
        }

        public override int GetHashCode()
        {
            return this.PropertiesHash();
        }

        /// <summary>
        /// Implementation of == operator for (Pixel.
        /// </summary>
        /// <param name="left">Compared (Pixel</param>
        /// <param name="right">Compared (Pixel</param>
        /// <returns>true if compared items are equals, false otherwise</returns>
        public static bool operator == (Pixel left, Pixel right)
        {
            return Equals(left, right);
        }

        /// <summary>
        /// Implementation of != operator for (Pixel.
        /// </summary>
        /// <param name="left">Compared (Pixel</param>
        /// <param name="right">Compared (Pixel</param>
        /// <returns>true if compared items are not equals, false otherwise</returns>
        public static bool operator != (Pixel left, Pixel right)
        {
            return !Equals(left, right);
        }

        /// <summary>
        /// Builder of Pixel.
        /// </summary>
        public sealed class PixelBuilder
        {
            private decimal? _X;
            private decimal? _Y;

            internal PixelBuilder()
            {
                SetupDefaults();
            }

            private void SetupDefaults()
            {
            }

            /// <summary>
            /// Sets value for Pixel.X property.
            /// </summary>
            /// <param name="value">X</param>
            public PixelBuilder X(decimal? value)
            {
                _X = value;
                return this;
            }

            /// <summary>
            /// Sets value for Pixel.Y property.
            /// </summary>
            /// <param name="value">Y</param>
            public PixelBuilder Y(decimal? value)
            {
                _Y = value;
                return this;
            }


            /// <summary>
            /// Builds instance of Pixel.
            /// </summary>
            /// <returns>Pixel</returns>
            public Pixel Build()
            {
                Validate();
                return new Pixel(
                    X: _X,
                    Y: _Y
                );
            }

            private void Validate()
            { 
                if (_X == null)
                {
                    throw new ArgumentException("X is a required property for Pixel and cannot be null");
                } 
                if (_Y == null)
                {
                    throw new ArgumentException("Y is a required property for Pixel and cannot be null");
                } 
            }
        }

        
    }
}