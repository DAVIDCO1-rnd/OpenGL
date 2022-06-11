using System;
using System.IO;
using System.Collections.Generic;
using Nancy;
using Nancy.TinyIoc;
using Nancy.Responses.Negotiation;
using Nancy.Json;
using Nancy.Bootstrapper;
#if NANCY_DIAGNOSTICS
using Nancy.Diagnostics;
#endif

namespace EOSimU.API.Swagger
{
    class SwaggerException : Exception
    {
        public int statusCode;

        public SwaggerException(int statusCode, string message) : base(message)
        {
            this.statusCode = statusCode;
        }
    }

    public class SwaggerBootstrapper : DefaultNancyBootstrapper
    {
        private readonly Action<TinyIoCContainer> registerHook;

        public SwaggerBootstrapper(Action<TinyIoCContainer> registerHook) : base()
        {
            this.registerHook = registerHook;
        }

        protected override void ConfigureApplicationContainer(TinyIoCContainer container)
        {
            registerHook.Invoke(container);
        }

        private static dynamic HandleSwaggerException(NancyContext context, Exception exception)
        {
            if (exception is SwaggerException)
            {
                SwaggerException swaggerException = (SwaggerException)exception;
                Response resp = exception.Message;
                resp.StatusCode = (HttpStatusCode)swaggerException.statusCode;
                return resp;
            }
            return null;
        }

        protected override void RequestStartup(TinyIoCContainer requestContainer, IPipelines pipelines, NancyContext context)
        {
            pipelines.BeforeRequest.AddItemToStartOfPipeline(ctx =>
            {
                if (ctx != null)
                {
                    Console.WriteLine("Request: " + ctx.Request.Url);
                }
                return null;
            });

            pipelines.OnError.AddItemToEndOfPipeline((ctx, ex) => HandleSwaggerException(ctx, ex));

            pipelines.AfterRequest += (ctx) =>
            {
                ctx.Response.Headers["Connection"] = "close";
            };
            base.RequestStartup(requestContainer, pipelines, context);
        }

#if NANCY_DIAGNOSTICS
        protected override DiagnosticsConfiguration DiagnosticsConfiguration
        {
            get { return new DiagnosticsConfiguration { Password = @"test"}; }
        }
#endif

        protected override void ApplicationStartup(TinyIoCContainer container, IPipelines pipelines)
        {
            base.ApplicationStartup(container, pipelines);
#if NANCY_DIAGNOSTICS
            StaticConfiguration.EnableRequestTracing = true;
            StaticConfiguration.DisableErrorTraces = false;
#endif
            StaticConfiguration.DisableRequestStreamSwitching = true;
            Nancy.Json.JsonSettings.PrimitiveConverters.Add(new SwaggerJsonEnumConverter());
        }

    }

    public class BinaryProcessor : IResponseProcessor
    {
        private static readonly IEnumerable<Tuple<string, MediaRange>> extensionMappings =
                new[] { new Tuple<string, MediaRange>("bin", new MediaRange("application/octet-stream")) };

        public IEnumerable<Tuple<string, MediaRange>> ExtensionMappings
        {
            get { return extensionMappings; }
        }

        public ProcessorMatch CanProcess(MediaRange requestedMediaRange, dynamic model, NancyContext context)
        {
            if (model is Stream)
            {
                return new ProcessorMatch { ModelResult = MatchResult.ExactMatch, RequestedContentTypeResult = MatchResult.DontCare };
            }
            return new ProcessorMatch { ModelResult = MatchResult.NoMatch, RequestedContentTypeResult = MatchResult.NoMatch };
        }

        public Response Process(MediaRange requestedMediaRange, dynamic model, NancyContext context)
        {
            var stream = model as Stream;
            return new Response
            {
                ContentType = requestedMediaRange,
                Contents = s =>
                {
                    stream.Position = 0;
                    stream.CopyTo(s);
                }
            };
        }
    }

    // This is for converting the auto generator int enums to their proper values.
    public class SwaggerJsonEnumConverter : JavaScriptPrimitiveConverter
    {
        public override IEnumerable<Type> SupportedTypes
        {
            get
            {
                yield return typeof(Enum);
            }
        }

        public override object Deserialize(
            object primitiveValue, Type type, JavaScriptSerializer serializer)
        {
            return Enum.Parse(type, (string)primitiveValue);
        }

        public override object Serialize(
            object obj, JavaScriptSerializer serializer)
        {
            var ret = obj.ToString();
            int value;
            if (ret.Length >= 2 && ret[0] == '_' && int.TryParse(ret.Substring(1), out value))
            {
                return value;
            }
            return ret;
        }
    }
}
