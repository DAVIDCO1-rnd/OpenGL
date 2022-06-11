using EOSim.SDK.Logic;
using EOSimU.API.AutoGen.v1alpha3.Adaptors;
using EOSimU.API.AutoGen.v1alpha3.Modules;
using Nancy.TinyIoc;

namespace EOSimU.API.Utils
{
    public static class RegisterSupport
    {
        public static void RegisterCallbacks(TinyIoCContainer container)
        {
            EOSim.SDK.Logic.PolygonsLosModuleImplementation polygonsLosModule = new EOSim.SDK.Logic.PolygonsLosModuleImplementation();
            container.Register<PolygonsLosService>(new PolygonsLosAdaptor(polygonsLosModule));
        }
    }
}
