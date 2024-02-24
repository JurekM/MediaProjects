using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace path2list
{
    public class FileGroupGenerator
    {
        private readonly Regex _r1 = new Regex("^([0-9a-fA-F]+).*\\((.+)\\).*([0-9]{8}).*([0-9]{6}).*$");
        //private readonly Regex _r2 = new Regex("^([0-9a-fA-F]+).*\\((.+)\\).*([0-9]{8})([0-9]{6}).*$");
        private readonly Regex _r3 = new Regex("^([0-9a-fA-F]+).*([0-9]{8})([0-9]{6}).*$");
        // 000DC5D739DC(Sarenki)_1_20171008134036_233.jpg

        public bool ParseFileName(string filepath, out string device, out string name, out string date, out string time)
        {
            device = "";
            name = "";
            date = "";
            time = "";

            int ndx = filepath.LastIndexOf(@"\", StringComparison.Ordinal);
            if (0 <= ndx)
                filepath = filepath.Substring(ndx + 1);
            ndx = filepath.LastIndexOf(@"/", StringComparison.Ordinal);
            if (0 <= ndx)
                filepath = filepath.Substring(ndx + 1);

            var groups = _r1.Match(filepath).Groups;
            if (groups.Count == 5)
            {
                device = groups[1].Value;
                name = groups[2].Value;
                date = groups[3].Value;
                time = groups[4].Value;
                return true;
            }

            //groups = _r2.Match(filepath).Groups;
            //if (groups.Count == 5)
            //{
            //    device = groups[1].Value;
            //    name = groups[2].Value;
            //    date = groups[3].Value;
            //    time = groups[4].Value;
            //    return true;
            //}

            groups = _r3.Match(filepath).Groups;
            if (groups.Count == 4)
            {
                device = groups[1].Value;
                name = "";
                date = groups[2].Value;
                time = groups[3].Value;
                return true;
            }

            return false;
        }

        public class FileInfo
        {
            public string Filepath;
            public string Deviceid;
            public string Devicename;
            public string Date;
            public string Time;
        }

        public List<FileInfo> ParseAllFileNames(IEnumerable<string> filepaths)
        {
            List<FileInfo> list = new List<FileInfo>();
            foreach (string path in filepaths)
            {
                FileInfo fi = new FileInfo { Filepath = path };
                if (ParseFileName(path, out fi.Deviceid, out fi.Devicename, out fi.Date, out fi.Time))
                    list.Add(fi);
            }

            return list;
        }

        public Dictionary<string, List<FileInfo>> GroupByDevice(IEnumerable<FileInfo> files)
        {
            Dictionary<string, List<FileInfo>> dict = new Dictionary<string, List<FileInfo>>();
            foreach (FileInfo info in files)
            {
                if (!dict.ContainsKey(info.Deviceid))
                    dict[info.Deviceid] = new List<FileInfo>();
                dict[info.Deviceid].Add(info);
            }

            return dict;
        }

        public Dictionary<string, List<FileInfo>> GroupByDate(IEnumerable<FileInfo> files)
        {
            Dictionary<string, List<FileInfo>> dict = new Dictionary<string, List<FileInfo>>();
            foreach (FileInfo info in files)
            {
                if (!dict.ContainsKey(info.Date))
                    dict[info.Date] = new List<FileInfo>();
                dict[info.Date].Add(info);
            }

            return dict;
        }

        public List<string> GenerateImgTags(IEnumerable<FileInfo> files, string root)
        {
            List<string> list = new List<string>();

            foreach (FileInfo file in files)
            {
                list.Add($"<a href=\"{root}{file.Filepath}\"><img src=\"{root}{file.Filepath}\" height=\"240\" width=\"320\" /></a>\n");
            }

            return list;
        }

        public string GenerateImgPage(IEnumerable<FileInfo> files, string root)
        {
            return "<html><body>\n" + string.Join("", GenerateImgTags(files, root)) + "\n</body></html>";
        }
    }

}