using System;
using Nancy.Hosting.Self;
using EOSimU.API.AutoGen.Swagger;

namespace EOSimU.API.Utils
{
    public class BaseServer : IDisposable
    {        
        private NancyHost hostNancy;
        public string hostUrl = "http://localhost:8080/";       

        public void Start()
        {
            HostConfiguration hostConfigs = new HostConfiguration()
            {
                UrlReservations = new UrlReservations() { CreateAutomatically = true },
            };

            hostNancy = new NancyHost(new System.Uri(hostUrl), new SwaggerBootstrapper(RegisterSupport.RegisterCallbacks), hostConfigs);
            hostNancy.Start();
        }

        public void Dispose()
        {
            hostNancy.Stop();
            hostNancy.Dispose();
        }
    }
}