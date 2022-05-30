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
            host = new NancyHost(new SwaggerBootstrapper((ioc) => SwaggerUtils.RegisterCallbacks(ioc, controller)), new Uri(uri));
            host.Start();
        }

        //private void OnDestroy()
        //{
        //    host?.Stop();
        //    host?.Dispose();
        //}
    }
}