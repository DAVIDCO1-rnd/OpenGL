using System;
using Nancy.Hosting.Self;

namespace EOSimU.API
{
    public class BaseServer
    {
        public string uri = "http://localhost:8080/";
        private NancyHost host;

        public SceneController controller;

        void Main(string[] args)
        {
            HostConfiguration hostConfigs = new HostConfiguration()
            {
                UrlReservations = new UrlReservations() { CreateAutomatically = true },
            };

            host = new NancyHost(new Uri(uri), new SwaggerBootstrapper(RegisterSupport.RegisterCallbacks), hostConfigs);
            host.Start();
        }

        //private void OnDestroy()
        //{
        //    host?.Stop();
        //    host?.Dispose();
        //}
    }
}