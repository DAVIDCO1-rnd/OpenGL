using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Eosim.SDK.ProtocolPlugins
{
    class ProtocolPluginLoader : IDisposable
    {
        private IProtocolPlugin plugin;

        public void BootstrapProtocolAndImplementation()
        {
            plugin = new SwaggerProtocolPlugin(); //TODO: reflection stuff
            (plugin as IProtocolPlugin).Bootstrap();
        }

        public void Dispose()
        {
            plugin.Dispose();
        }
    }
}
