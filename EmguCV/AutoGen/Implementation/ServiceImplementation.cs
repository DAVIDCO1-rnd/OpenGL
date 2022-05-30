
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

using EOSimU.API.AutoGen.v1alpha3.Modules;
using EOSimU.API.AutoGen.v1alpha3.Models;
using Nancy;

using EOSimU.Util;
using System.Threading.Tasks;

namespace EOSimU.API
{
    internal class SceneModule : PetService
    {
        private readonly SceneController sceneController;

        public SceneModule(SceneController controller) : base()
        {
            sceneController = controller;
        }

        public Task AddPet(NancyContext context, Pet body)
        {
            sceneController.AddPet(body);
            return Task.CompletedTask;
        }

        public Task UpdatePet(NancyContext context, Pet body)
        {
            sceneController.UpdatePet(body);
            return Task.CompletedTask;
        }
    }
}