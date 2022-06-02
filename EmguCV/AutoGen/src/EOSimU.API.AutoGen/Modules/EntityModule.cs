using System;
using Nancy;
using Nancy.ModelBinding;
using System.Collections.Generic;
using Sharpility.Base;
using EOSimU.API.AutoGen.v1alpha3.Models;
using EOSimU.API.AutoGen.v1alpha3.Utils;
using NodaTime;
using System.Threading.Tasks;


namespace EOSimU.API.AutoGen.v1alpha3.Modules
{ 

    /// <summary>
    /// Module processing requests of Entity domain.
    /// </summary>
    public sealed class EntityModule : NancyModule
    {
        /// <summary>
        /// Sets up HTTP methods mappings.
        /// </summary>
        /// <param name="service">Service handling requests</param>
        public EntityModule(EntityService service) : base("/v1alpha3")
        { 
            Get["/Entity/Polygons{target3dLocationAndHeight}", true] = async (parameters, ct) =>
            {
                var target3dLocationAndHeight = Parameters.ValueOf<List<double?>>(parameters, Context.Request, "target3dLocationAndHeight", ParameterType.Path);
                Preconditions.IsNotNull(target3dLocationAndHeight, "Required parameter: 'target3dLocationAndHeight' is missing at 'ScenePolygons'");
                
                return await service.ScenePolygons(Context, target3dLocationAndHeight).ToArray();
            };
        }
    }

    /// <summary>
    /// Service handling Entity requests.
    /// </summary>
    public interface EntityService
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="context">Context of request</param>
        /// <param name="target3dLocationAndHeight">target location</param>
        /// <returns>List&lt;Polygon&gt;</returns>
        Task<List<Polygon>> ScenePolygons(NancyContext context, List<double?> target3dLocationAndHeight);
    }

    /// <summary>
    /// Abstraction of EntityService.
    /// </summary>
    public abstract class AbstractEntityService: EntityService
    {
        public virtual Task<List<Polygon>> ScenePolygons(NancyContext context, List<double?> target3dLocationAndHeight)
        {
            return ScenePolygons(target3dLocationAndHeight);
        }

        protected abstract Task<List<Polygon>> ScenePolygons(List<double?> target3dLocationAndHeight);
    }

}
