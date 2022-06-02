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
    /// Module processing requests of Scene domain.
    /// </summary>
    public sealed class SceneModule : NancyModule
    {
        /// <summary>
        /// Sets up HTTP methods mappings.
        /// </summary>
        /// <param name="service">Service handling requests</param>
        public SceneModule(SceneService service) : base("/v1alpha3")
        { 
            Post["/Scene/Init/{scenarioName}", true] = async (parameters, ct) =>
            {
                var scenarioName = Parameters.ValueOf<string>(parameters, Context.Request, "scenarioName", ParameterType.Path);
                Preconditions.IsNotNull(scenarioName, "Required parameter: 'scenarioName' is missing at 'SceneInit'");
                
                await service.SceneInit(Context, scenarioName);
                return new Response { ContentType = ""};
            };
        }
    }

    /// <summary>
    /// Service handling Scene requests.
    /// </summary>
    public interface SceneService
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="context">Context of request</param>
        /// <param name="scenarioName">Name of scenario to load</param>
        /// <returns></returns>
        Task SceneInit(NancyContext context, string scenarioName);
    }

    /// <summary>
    /// Abstraction of SceneService.
    /// </summary>
    public abstract class AbstractSceneService: SceneService
    {
        public virtual Task SceneInit(NancyContext context, string scenarioName)
        {
            return SceneInit(scenarioName);
        }

        protected abstract Task SceneInit(string scenarioName);
    }

}
