using System;

namespace EOSim.SDK.ProtocolPlugins
{
    public interface IProtocolPlugin : IDisposable
    {
        void Bootstrap();
    }
}
