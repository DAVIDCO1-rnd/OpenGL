using EOSimU.API.Utils;

namespace EOSim.SDK.ProtocolPlugins
{
    class SwaggerProtocolPlugin : IProtocolPlugin
    {
        EOSimU.API.Utils.BaseServer server;

        public void Bootstrap()
        {
            server = new EOSimU.API.Utils.BaseServer();
            server.Start();
        }

        public void Dispose()
        {
            server.Dispose();
        }
    }
}
