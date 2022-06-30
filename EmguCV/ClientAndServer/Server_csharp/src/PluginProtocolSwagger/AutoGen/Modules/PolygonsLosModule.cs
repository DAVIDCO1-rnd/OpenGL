using System;
using Nancy;
using Nancy.ModelBinding;
using System.Collections.Generic;
using Sharpility.Base;
using EOSimU.API.AutoGen.v1alpha3.Models;
using EOSimU.API.AutoGen.v1alpha3.Utils;
using NodaTime;


namespace EOSimU.API.AutoGen.v1alpha3.Modules
{ 

    /// <summary>
    /// Module processing requests of PolygonsLos domain.
    /// </summary>
    public sealed class PolygonsLosModule : NancyModule
    {
        /// <summary>
        /// Sets up HTTP methods mappings.
        /// </summary>
        /// <param name="service">Service handling requests</param>
        public PolygonsLosModule(PolygonsLosService service) : base("/v1alpha3")
        { 
            Post["/PolygonsLos/Init/{terrainName}"] = parameters =>
            {
                var terrainName = Parameters.ValueOf<string>(parameters, Context.Request, "terrainName", ParameterType.Path);
                Preconditions.IsNotNull(terrainName, "Required parameter: 'terrainName' is missing at 'SceneInit'");
                
                service.SceneInit(Context, terrainName);
                return new Response { ContentType = ""};
            };

            Get["/PolygonsLos/getPolygons/{targetLatitude}/{targetLongitude}/{targetHeight}/{plateHeightAboveTarget}"] = parameters =>
            {
                var targetLatitude = Parameters.ValueOf<double?>(parameters, Context.Request, "targetLatitude", ParameterType.Path);
                var targetLongitude = Parameters.ValueOf<double?>(parameters, Context.Request, "targetLongitude", ParameterType.Path);
                var targetHeight = Parameters.ValueOf<double?>(parameters, Context.Request, "targetHeight", ParameterType.Path);
                var plateHeightAboveTarget = Parameters.ValueOf<double?>(parameters, Context.Request, "plateHeightAboveTarget", ParameterType.Path);
                Preconditions.IsNotNull(targetLatitude, "Required parameter: 'targetLatitude' is missing at 'ScenePolygons'");
                
                Preconditions.IsNotNull(targetLongitude, "Required parameter: 'targetLongitude' is missing at 'ScenePolygons'");
                
                Preconditions.IsNotNull(targetHeight, "Required parameter: 'targetHeight' is missing at 'ScenePolygons'");
                
                Preconditions.IsNotNull(plateHeightAboveTarget, "Required parameter: 'plateHeightAboveTarget' is missing at 'ScenePolygons'");
                
                return service.ScenePolygons(Context, targetLatitude, targetLongitude, targetHeight, plateHeightAboveTarget).ToArray();
            };
        }
    }

    /// <summary>
    /// Service handling PolygonsLos requests.
    /// </summary>
    public interface PolygonsLosService
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="context">Context of request</param>
        /// <param name="terrainName">Name of terrain to add to scenario</param>
        /// <returns></returns>
        void SceneInit(NancyContext context, string terrainName);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="context">Context of request</param>
        /// <param name="targetLatitude">target latitude</param>
        /// <param name="targetLongitude">target longitude</param>
        /// <param name="targetHeight">target height</param>
        /// <param name="plateHeightAboveTarget">Height (in meters) of the plate above the target</param>
        /// <returns>List&lt;Polygon&gt;</returns>
        List<Polygon> ScenePolygons(NancyContext context, double? targetLatitude, double? targetLongitude, double? targetHeight, double? plateHeightAboveTarget);
    }

    /// <summary>
    /// Abstraction of PolygonsLosService.
    /// </summary>
    public abstract class AbstractPolygonsLosService: PolygonsLosService
    {
        public virtual void SceneInit(NancyContext context, string terrainName)
        {
            SceneInit(terrainName);
        }

        public virtual List<Polygon> ScenePolygons(NancyContext context, double? targetLatitude, double? targetLongitude, double? targetHeight, double? plateHeightAboveTarget)
        {
            return ScenePolygons(targetLatitude, targetLongitude, targetHeight, plateHeightAboveTarget);
        }

        protected abstract void SceneInit(string terrainName);

        protected abstract List<Polygon> ScenePolygons(double? targetLatitude, double? targetLongitude, double? targetHeight, double? plateHeightAboveTarget);
    }

}
