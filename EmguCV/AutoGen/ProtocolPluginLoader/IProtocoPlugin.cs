using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Eosim.SDK.ProtocolPlugins
{
    public interface IProtocolPlugin : IDisposable
    {
        void Bootstrap();
    }
}
