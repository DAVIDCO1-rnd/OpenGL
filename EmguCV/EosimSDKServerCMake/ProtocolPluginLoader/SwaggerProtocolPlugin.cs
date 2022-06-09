using EOSimU.API.Utils;

namespace EOSim.SDK.ProtocolPlugins
{
    class SwaggerProtocolPlugin : IProtocolPlugin
    {
        BaseServer server;

        public void Bootstrap()
        {
            server = new BaseServer();
            server.Start();
        }

        public void Dispose()
        {
            server.Dispose();
        }
    }
}
