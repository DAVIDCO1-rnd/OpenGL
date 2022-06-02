using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Eosim.SDK.ProtocolPlugins
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
