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
            Post["/PolygonsLos/Init/{scenarioName}/{cameraX}/{cameraY}/{cameraZ}/{plateHeightAboveTarget}"] = parameters =>
            {
                var scenarioName = Parameters.ValueOf<string>(parameters, Context.Request, "scenarioName", ParameterType.Path);
                var cameraX = Parameters.ValueOf<double?>(parameters, Context.Request, "cameraX", ParameterType.Path);
                var cameraY = Parameters.ValueOf<double?>(parameters, Context.Request, "cameraY", ParameterType.Path);
                var cameraZ = Parameters.ValueOf<double?>(parameters, Context.Request, "cameraZ", ParameterType.Path);
                var plateHeightAboveTarget = Parameters.ValueOf<double?>(parameters, Context.Request, "plateHeightAboveTarget", ParameterType.Path);
                Preconditions.IsNotNull(scenarioName, "Required parameter: 'scenarioName' is missing at 'SceneInit'");

                Preconditions.IsNotNull(cameraX, "Required parameter: 'cameraX' is missing at 'SceneInit'");

                Preconditions.IsNotNull(cameraY, "Required parameter: 'cameraY' is missing at 'SceneInit'");

                Preconditions.IsNotNull(cameraZ, "Required parameter: 'cameraZ' is missing at 'SceneInit'");

                Preconditions.IsNotNull(plateHeightAboveTarget, "Required parameter: 'plateHeightAboveTarget' is missing at 'SceneInit'");

                service.SceneInit(Context, scenarioName, cameraX, cameraY, cameraZ, plateHeightAboveTarget);
                return new Response { ContentType = "" };
            };

            Get["/PolygonsLos/getPolygons/{cameraX}/{cameraY}/{cameraZ}/{plateHeightAboveTarget}"] = parameters =>
            {
                var cameraX = Parameters.ValueOf<double?>(parameters, Context.Request, "cameraX", ParameterType.Path);
                var cameraY = Parameters.ValueOf<double?>(parameters, Context.Request, "cameraY", ParameterType.Path);
                var cameraZ = Parameters.ValueOf<double?>(parameters, Context.Request, "cameraZ", ParameterType.Path);
                var plateHeightAboveTarget = Parameters.ValueOf<double?>(parameters, Context.Request, "plateHeightAboveTarget", ParameterType.Path);
                Preconditions.IsNotNull(cameraX, "Required parameter: 'cameraX' is missing at 'ScenePolygons'");

                Preconditions.IsNotNull(cameraY, "Required parameter: 'cameraY' is missing at 'ScenePolygons'");

                Preconditions.IsNotNull(cameraZ, "Required parameter: 'cameraZ' is missing at 'ScenePolygons'");

                Preconditions.IsNotNull(plateHeightAboveTarget, "Required parameter: 'plateHeightAboveTarget' is missing at 'ScenePolygons'");

                return service.ScenePolygons(Context, cameraX, cameraY, cameraZ, plateHeightAboveTarget).ToArray();
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
        /// <param name="scenarioName">Name of scenario to load</param>
        /// <param name="cameraX">cameraX location</param>
        /// <param name="cameraY">cameraY location</param>
        /// <param name="cameraZ">cameraZ location</param>
        /// <param name="plateHeightAboveTarget">Height (in meters) of the plate above the target</param>
        /// <returns></returns>
        void SceneInit(NancyContext context, string scenarioName, double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="context">Context of request</param>
        /// <param name="cameraX">cameraX location</param>
        /// <param name="cameraY">cameraY location</param>
        /// <param name="cameraZ">cameraZ location</param>
        /// <param name="plateHeightAboveTarget">Height (in meters) of the plate above the target</param>
        /// <returns>List&lt;Polygon&gt;</returns>
        List<Polygon> ScenePolygons(NancyContext context, double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget);
    }

    /// <summary>
    /// Abstraction of PolygonsLosService.
    /// </summary>
    public abstract class AbstractPolygonsLosService : PolygonsLosService
    {
        public virtual void SceneInit(NancyContext context, string scenarioName, double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget)
        {
            SceneInit(scenarioName, cameraX, cameraY, cameraZ, plateHeightAboveTarget);
        }

        public virtual List<Polygon> ScenePolygons(NancyContext context, double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget)
        {
            return ScenePolygons(cameraX, cameraY, cameraZ, plateHeightAboveTarget);
        }

        protected abstract void SceneInit(string scenarioName, double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget);

        protected abstract List<Polygon> ScenePolygons(double? cameraX, double? cameraY, double? cameraZ, double? plateHeightAboveTarget);
    }

}
