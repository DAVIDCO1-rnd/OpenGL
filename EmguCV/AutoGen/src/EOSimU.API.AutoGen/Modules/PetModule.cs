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
    /// Module processing requests of Pet domain.
    /// </summary>
    public sealed class PetModule : NancyModule
    {
        /// <summary>
        /// Sets up HTTP methods mappings.
        /// </summary>
        /// <param name="service">Service handling requests</param>
        public PetModule(PetService service) : base("/v2")
        { 
            Post["/pet", true] = async (parameters, ct) =>
            {
                var body = this.Bind<Pet>();
                Preconditions.IsNotNull(body, "Required parameter: 'body' is missing at 'AddPet'");
                
                await service.AddPet(Context, body);
                return new Response { ContentType = ""};
            };

            Put["/pet", true] = async (parameters, ct) =>
            {
                var body = this.Bind<Pet>();
                Preconditions.IsNotNull(body, "Required parameter: 'body' is missing at 'UpdatePet'");
                
                await service.UpdatePet(Context, body);
                return new Response { ContentType = ""};
            };
        }
    }

    /// <summary>
    /// Service handling Pet requests.
    /// </summary>
    public interface PetService
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="context">Context of request</param>
        /// <param name="body">Pet object that needs to be added to the store</param>
        /// <returns></returns>
        Task AddPet(NancyContext context, Pet body);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="context">Context of request</param>
        /// <param name="body">Pet object that needs to be added to the store</param>
        /// <returns></returns>
        Task UpdatePet(NancyContext context, Pet body);
    }

    /// <summary>
    /// Abstraction of PetService.
    /// </summary>
    public abstract class AbstractPetService: PetService
    {
        public virtual Task AddPet(NancyContext context, Pet body)
        {
            return AddPet(body);
        }

        public virtual Task UpdatePet(NancyContext context, Pet body)
        {
            return UpdatePet(body);
        }

        protected abstract Task AddPet(Pet body);

        protected abstract Task UpdatePet(Pet body);
    }

}
