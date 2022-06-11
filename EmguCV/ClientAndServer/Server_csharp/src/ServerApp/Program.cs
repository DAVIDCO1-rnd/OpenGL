using System;
using EOSim.SDK.ProtocolPlugins;


namespace ServerApp
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            using (var eosimLoader = new EOSim.SDK.ProtocolPlugins.ProtocolPluginLoader())
            {
                eosimLoader.BootstrapProtocolAndImplementation();
                Console.ReadLine();
            }
        }
    }
}
