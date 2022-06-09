using System;
using EOSim.SDK.ProtocolPlugins;
using System.Threading;


namespace ServerApp
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            SynchronizationContext sctx = SynchronizationContext.Current;
            using (var p = new ProtocolPluginLoader())
            {
                p.BootstrapProtocolAndImplementation();
                Console.ReadLine();
            }
        }
    }
}
