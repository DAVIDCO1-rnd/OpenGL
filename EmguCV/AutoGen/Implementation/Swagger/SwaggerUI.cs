using System.Collections.Generic;
using Nancy;

public class SpecModule : NancyModule
{
    public SpecModule() : base("/")
    {
        var mimetypes = new Dictionary<string, string>();
        mimetypes[".css"] = "text/css";
        mimetypes[".html"] = "text/html";
        mimetypes[".js"] = "text/javascript";

        Get["/ui/"] = _ =>
        {
            if ((string)this.Request.Query["url"] == null)
            {
                return Response.AsRedirect("/spec/ui/?url=/simulation.spec");
            }
            var response = (Response)System.IO.File.ReadAllText(System.IO.Path.Combine(Application.streamingAssetsPath, "ui", "index.html"));
            response.ContentType = "text/html";
            return response;
        };
        Get["/ui/{path}"] = path =>
        {
            var response = (Response)System.IO.File.ReadAllText(System.IO.Path.Combine(Application.streamingAssetsPath, "ui", path.path));
            string mimetype;
            if (mimetypes.TryGetValue(System.IO.Path.GetExtension(path.path), out mimetype))
            {
                response.ContentType = mimetype;
            }
            return response;
        };

        Get["/"] = _ =>
        {
            return Response.AsRedirect("/ui/?url=/simulation.spec");
        };

        Get["/simulation.spec"] = _ =>
        {
            return System.IO.File.ReadAllText(System.IO.Path.Combine(Application.streamingAssetsPath, "Spec", "simulation.spec"));
        };
    }
}