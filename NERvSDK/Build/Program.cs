using CLeopardZip;
using ICSharpCode.SharpZipLib.Zip;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Build
{
    class Program
    {
        static void Main(string[] args)
        {
            if (!Directory.GetCurrentDirectory().EndsWith("Package"))
                Console.Error.WriteLine("Usage: Put this util into Package folder under solution folder!");

            string[] refContent = File.ReadAllLines("..\\..\\Net Data Source\\include\\ref.h");
            int major = 1, minor = 0, subminor = 0;
            foreach (string line in refContent)
            {
                tryExtractVersion(line, "#define ITS_VERSION_MAJOR", ref major);
                tryExtractVersion(line, "#define ITS_VERSION_MINOR", ref minor);
                tryExtractVersion(line, "#define ITS_VERSION_SUBMINOR", ref subminor);
            }

            string zipFileName = null;
            if (args.Length > 0 && args[0] == "--stable")
            {
                zipFileName = string.Format("Net-DataSource-stable-{0}.{1}.{2}-[32&64-bit].nvg",
                major, minor, subminor);
            }
            else
            {
                zipFileName = string.Format("Net-DataSource-{0}.{1}.{2}-[32&64-bit].nvg",
                major, minor, subminor);
            }

            File.Delete(zipFileName);

            string directoryRoot = string.Format("Net DataSource ver{0}.{1}.{2}",
                major, minor, subminor);
            string directory = directoryRoot + "\\Net DataSource";

            Directory.CreateDirectory(directory);


            File.Copy("..\\..\\bin\\win32\\Plugins\\Net Data Source\\module32.dll", directory + "\\module32.dll", true);
            File.Copy("..\\..\\bin\\win64\\Plugins\\Net Data Source\\module64.dll", directory + "\\module64.dll", true);


            Environment.CurrentDirectory = Environment.CurrentDirectory + "\\" + directoryRoot;
            ZipHelper.ZipDirectory("Net DataSource", @zipFileName);
        }

        private static void tryExtractVersion(string line, string symbol, ref int ver)
        {
            if (line.Contains(symbol))
                ver = int.Parse(line.Substring(line.LastIndexOf(" ") + 1));
        }

        private static void CopyDirectory(string sourceDirName, string destDirName)
        {
            if (!Directory.Exists(destDirName))
            {
                Directory.CreateDirectory(destDirName);
                File.SetAttributes(destDirName, File.GetAttributes(sourceDirName));

            }

            if (destDirName[destDirName.Length - 1] != Path.DirectorySeparatorChar)
                destDirName = destDirName + Path.DirectorySeparatorChar;

            string[] files = Directory.GetFiles(sourceDirName);
            foreach (string file in files)
            {
                if (File.Exists(destDirName + Path.GetFileName(file)))
                    continue;
                File.Copy(file, destDirName + Path.GetFileName(file), true);
                File.SetAttributes(destDirName + Path.GetFileName(file), FileAttributes.Normal);
            }

            string[] dirs = Directory.GetDirectories(sourceDirName);
            foreach (string dir in dirs)
            {
                CopyDirectory(dir, destDirName + Path.GetFileName(dir));
            }
        }

        private static void CreateZipFile(string filesPath, string zipFilePath)
        {

            if (!Directory.Exists(filesPath))
            {
                Console.WriteLine("Cannot find directory '{0}'", filesPath);
                return;
            }

            try
            {
                string[] filenames = Directory.GetFiles(filesPath);
                using (ZipOutputStream s = new ZipOutputStream(File.Create(zipFilePath)))
                {

                    s.SetLevel(5); // 压缩级别 0-9
                    //s.Password = "123"; //Zip压缩文件密码
                    byte[] buffer = new byte[4096]; //缓冲区大小
                    foreach (string file in filenames)
                    {
                        ZipEntry entry = new ZipEntry(Path.GetFileName(file));
                        entry.DateTime = DateTime.Now;
                        s.PutNextEntry(entry);
                        using (FileStream fs = File.OpenRead(file))
                        {
                            int sourceBytes;
                            do
                            {
                                sourceBytes = fs.Read(buffer, 0, buffer.Length);
                                s.Write(buffer, 0, sourceBytes);
                            } while (sourceBytes > 0);
                        }
                    }
                    s.Finish();
                    s.Close();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Exception during processing {0}", ex);
            }
        }
    }
}
