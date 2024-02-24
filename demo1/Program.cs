using System;
using System.IO;
using System.Runtime.InteropServices;

namespace Demo1
{
    class Program
    {
        static int Main(string[] args)
        {
            if (args.Length != 2 && args.Length != 3)
            {
                Console.WriteLine("Syntax: Demo1 <inputfile> <outputfile> [.|root] [yyyymmdd]");
                return 1;
            }
            string inpath = args[0];
            string oupath = args[1];
            string root = 2 < args.Length && "." != args[2] ? args[2] : "";
            string date = 3 < args.Length ? args[3] : null;

            if (File.Exists(oupath))
            {
                Console.WriteLine($"Output file [{oupath}] exists, aborting.");
                return 1;
            }

            string text = new StreamReader(inpath).ReadToEnd();
            HtmlGenerator.HtmlGenerator hg = new HtmlGenerator.HtmlGenerator();
            char[] splitter = {'\"', '\r', '\n', '\t'};
            var files = hg.ParseAllFileNames(text.Split(splitter, StringSplitOptions.RemoveEmptyEntries));

            if (!string.IsNullOrWhiteSpace(date))
            {
                var bydate = hg.GroupByDate(files);
                if (!bydate.ContainsKey(date))
                {
                    Console.WriteLine($"File [{inpath}] does not contain date [{date}], aborting.");
                    return 2;
                }
                files = bydate[date];
            }


            string html = hg.GenerateImgPage(files, root);
            using (StreamWriter w = new StreamWriter(oupath))
                w.Write(html);

            Console.WriteLine($"Found {files.Count} files for date [{date ?? "any"}].");
            return 0;
        }

        //string file = "C:\\Data\\000DC5D739DC(Sarenki)_1_20171008134036_233.jpg";
        //hg.ParseFileName(file, out var device, out var name, out var date, out var time);    }
    }
}