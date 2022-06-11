using System;
using Nancy.Hosting.Self;
using EOSimU.API.AutoGen.Swagger;
using EOSimU.API.AutoGen.v1alpha3.Modules;

namespace EOSimU.API.Utils
{
    public class BaseServer : IDisposable
    {        
        private Nancy.Hosting.Self.NancyHost hostNancy;
        public string hostUrl = "http://localhost:8080/";       

        public void Start()
        {
            Nancy.Hosting.Self.HostConfiguration hostConfigs = new Nancy.Hosting.Self.HostConfiguration()
            {
                UrlReservations = new Nancy.Hosting.Self.UrlReservations() { CreateAutomatically = true },
            };

            hostNancy = new Nancy.Hosting.Self.NancyHost(new System.Uri(hostUrl), new EOSimU.API.AutoGen.Swagger.SwaggerBootstrapper(EOSimU.API.Utils.RegisterSupport.RegisterCallbacks), hostConfigs);
            hostNancy.Start();
        }

        public void Dispose()
        {
            hostNancy.Stop();
            hostNancy.Dispose();
        }
    }
}