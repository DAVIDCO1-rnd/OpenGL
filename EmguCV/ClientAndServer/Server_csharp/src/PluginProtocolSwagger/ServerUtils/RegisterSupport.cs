using EOSim.SDK.Logic;
using EOSimU.API.Adaptors;
using EOSimU.API.AutoGen.v1alpha3.Modules;
using Nancy.TinyIoc;

namespace EOSimU.API.Utils
{
    public static class RegisterSupport
    {
        public static void RegisterCallbacks(TinyIoCContainer container)
        {
            EOSim.SDK.Logic.PolygonsLosImplementation polygonsLosImplementation = new EOSim.SDK.Logic.PolygonsLosImplementation();
            container.Register<PolygonsLosService>(new PolygonsLosAdaptor(polygonsLosImplementation));
        }
    }
}
