using System;
using Nancy.Hosting.Self;
using EOSimU.API.AutoGen.Swagger;

namespace EOSimU.API.Utils
{
    public class BaseServer
    {
        private NancyHost hostNancy;
        public string uri = "http://localhost:8080/";
        
        public void Start()
        {
            HostConfiguration hostConfigs = new HostConfiguration()
            {
                UrlReservations = new UrlReservations() { CreateAutomatically = true },
            };

            hostNancy = new NancyHost(new Uri(uri), new SwaggerBootstrapper(RegisterSupport.RegisterCallbacks), hostConfigs);
            hostNancy.Start();
        }

        private void OnDestroy()
        {
            hostNancy.Stop();
            hostNancy.Dispose();
        }
    }
}