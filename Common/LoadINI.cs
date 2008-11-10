using System;
using System.IO;
using System.Text;

namespace PAGfx.Common
{
    static class LoadINI
    {
        // Duplicated code
        static UInt16 PA_RGB(int r, int g, int b)
        {
            return (UInt16)((1 << 15) + r + (g << 5) + (b << 10));
        }

        // Ini parser structures...
        public struct IniSpriteStruct
        {
            public string Name;
            public string Path; // Path to open file
            public int ColorMode; // 0 pour 16 couleurs, 1 pour 256, 2 pour 16bit...
            public string PaletteName;
        };
        public struct IniBgStruct
        {
            public string Name;
            public string Path; // Path to open file...
            public int BgMode; // 8bit, 16bit, TileBg, RotBg, LargeMap
            public string PaletteName;
        };
        public struct IniTexStruct
        {
            public string Name;
            public string Path; // Path to open file...
            public int ColorMode; // 8bit, 16bit, TileBg, RotBg, LargeMap
            public string PaletteName;
        };
        public struct TranspStruct
        {
            public int Number; // Number (0-4)
            public UInt16 Color; // Color
            public string Name; // Colorname...
        }
        public struct IniStruct
        {
            public string[] Lines; // Different lines
            public int NLines; // Number of lines
            public string[] Strings; // Strings in a line...
        }

        static public int NSprites = 0; // Nombre de sprites
        static public int NBackgrounds = 0; // Nombre de bg
        static public int NTextures = 0; // Nombre de textures

        static public IniSpriteStruct[] IniSprites;
        static public IniBgStruct[] IniBgs;
        static public IniTexStruct[] IniTextures;
        static public TranspStruct TranspColor;
        static IniStruct Ini;
        static public string[] SpriteModes;
        static public string[] BgModes;
        static public string[] TextureModes;
        static public string[] TranspColorStrings;
        static UInt16[] TranspColors;

        static void InitTranspColor()
        {
            // Init the color modes...
            TranspColorStrings = new string[5];
            TranspColorStrings[0] = "Black";
            TranspColorStrings[1] = "White";
            TranspColorStrings[2] = "Magenta";
            TranspColorStrings[3] = "Fuschia";
            TranspColorStrings[4] = "Green";
            TranspColors = new UInt16[5];
            TranspColors[0] = PA_RGB(0, 0, 0);
            TranspColors[1] = PA_RGB(31, 31, 31);
            TranspColors[2] = PA_RGB(31, 0, 31);
            TranspColors[3] = PA_RGB(31, 0, 31);
            TranspColors[4] = PA_RGB(0, 31, 0);

            TranspColor.Number = GetTranspColor();
            TranspColor.Color = TranspColors[TranspColor.Number];
            TranspColor.Name = TranspColorStrings[TranspColor.Number];
        }

        static int GetTranspColor()
        {
            // Transparent color...
            int Transp = 0;
            for (int i = 0; i < Ini.NLines; i++)
            {
                Ini.Strings = Ini.Lines[i].Split();
                if (Ini.Strings[0] == "#TranspColor")
                {
                    //Look for the transparent color...
                    for (int j = 0; j < 5; j++)
                        if (Ini.Strings[1].ToLower() == TranspColorStrings[j].ToLower())
                            Transp = j;
                    break;
                }
            }
            return Transp;
        }

        // Extracts the filename (removing all the path and the extension)
        static public string ExtractName(string Path)
        {
            string Name;
            byte[] tempbyte = Encoding.UTF8.GetBytes(Path);
            int length = tempbyte.Length, start = 0;
            for (int i = 0; i < tempbyte.Length; i++)
                if ((tempbyte[i] == '/') || (tempbyte[i] == '\\'))
                    start = i + 1;
            for (int i = 0; i < tempbyte.Length; i++)
                if (tempbyte[i] == '.')
                    length = i - start;
            Name = Encoding.UTF8.GetString(tempbyte, start, length);


            if ((Name[0] >= '0') && (Name[0] <= '9'))
            {
                Console.WriteLine("ERROR : Name " + Name + " is not compatible with C code => names cannot start with a number...");
                Console.ReadKey();
            }

            return Name;
        }

        // Extracts the path from the ini file
        static string ExtractPath()
        {
            bool tempok = false;
            while (!tempok)
            {
                byte[] tempbyte = Encoding.ASCII.GetBytes(Ini.Strings[0]);
                for (int i = 0; i < (tempbyte.Length - 1) && (!tempok); i++)
                    if (tempbyte[i] == '.')
                        tempok = true; // On cherche un fichier...
                if (!tempok)
                {
                    Ini.Strings[0] += " " + Ini.Strings[1];
                    for (int j = 1; j < Ini.Strings.Length - 1; j++)
                    {
                        Ini.Strings[j] = Ini.Strings[j + 1];
                    }
                }
            }
            Console.WriteLine(Ini.Strings[0]);
            return Ini.Strings[0];
        }

        static bool GetSpriteInfo()
        {
            // Puts the sprite infos into the sprite structure
            bool FileOk = true;
            IniSprites[NSprites].Path = ExtractPath();
            string PathBackup = IniSprites[NSprites].Path;

            FileOk = File.Exists(IniSprites[NSprites].Path);
            if (!FileOk && !Path.IsPathRooted(IniSprites[NSprites].Path))
            {
                IniSprites[NSprites].Path = System.AppDomain.CurrentDomain.BaseDirectory + IniSprites[NSprites].Path;
                FileOk = File.Exists(IniSprites[NSprites].Path);
            }

            if (FileOk)
            {
                IniSprites[NSprites].Name = ExtractName(IniSprites[NSprites].Path);


                // Try to get the color mode, sets to 256Colors by default
                IniSprites[NSprites].ColorMode = 1;
                //Console.Write(Ini.Strings.Length);
                if (Ini.Strings.Length > 1)
                    for (int i = 0; i < 5; i++)
                        if (Ini.Strings[1].ToLower() == SpriteModes[i].ToLower())
                            IniSprites[NSprites].ColorMode = i;

                // Try to get the palette name, gives the sprite name if no palette found
                if (IniSprites[NSprites].ColorMode != 2) // If not 16bit...
                {
                    IniSprites[NSprites].PaletteName = IniSprites[NSprites].Name;
                    if (Ini.Strings.Length > 2)
                        IniSprites[NSprites].PaletteName = Ini.Strings[2];
                }

                NSprites++;
            }
            if (!FileOk)
            {
                Console.WriteLine("ERROR : File " + PathBackup + " could not be found. Please check the filename for a typo... Press any key to continue.");
                Console.ReadKey();
            }
            return FileOk;
        }

        static string InitSpriteInfos()
        {
            // Init the different sprite modes...
            SpriteModes = new string[5];
            SpriteModes[0] = "16colors";
            SpriteModes[1] = "256colors";
            SpriteModes[2] = "16bit";
            SpriteModes[3] = "KeepPal";
            SpriteModes[4] = "JustPal";
            IniSprites = new IniSpriteStruct[10000]; // Memory alloc...

            // Get the starting line for sprites...
            int SpriteLine = -1;
            for (int i = 0; i < Ini.NLines; i++)
            {
                Ini.Strings = Ini.Lines[i].Split();
                if (Ini.Strings[0] == "#Sprites")
                {
                    SpriteLine = i;
                    break;
                }
            }

            SpriteLine++; // First sprite info

            NSprites = 0;
            for (int i = 0; ; i++)
            {
                if (SpriteLine + i == Ini.NLines)
                    break;
                Ini.Strings = Ini.Lines[SpriteLine + i].Split();
                if ((Ini.Strings[0].Length > 0) && (!Ini.Strings[0].StartsWith("#")))
                {
                    if (!GetSpriteInfo())
                        return "Sorry, " + IniSprites[NSprites].Path + " does not exist !";
                }
                else
                    break; // On a trouvé la bonne ligne !
            }
            return "";
        }

        static bool GetBgInfo()
        {
            // Puts the sprite infos into the sprite structure
            bool FileOk = true;
            IniBgs[NBackgrounds].Path = ExtractPath();
            string PathBackup = IniSprites[NBackgrounds].Path;

            FileOk = File.Exists(IniBgs[NBackgrounds].Path);
            if (!FileOk && !Path.IsPathRooted(IniBgs[NBackgrounds].Path))
            {
                IniBgs[NBackgrounds].Path = System.AppDomain.CurrentDomain.BaseDirectory + IniBgs[NBackgrounds].Path;
                FileOk = File.Exists(IniBgs[NBackgrounds].Path);
            }

            if (FileOk)
            {
                IniBgs[NBackgrounds].Name = ExtractName(IniBgs[NBackgrounds].Path);

                // Try to get the color mode, sets to Tiled by default
                IniBgs[NBackgrounds].BgMode = 2; // Tiled Bg
                if (Ini.Strings.Length > 1)
                    for (int i = 0; i < 10; i++)
                        if (Ini.Strings[1].ToLower() == BgModes[i].ToLower())
                            IniBgs[NBackgrounds].BgMode = i;

                if (IniBgs[NBackgrounds].BgMode != 1)
                {
                    // Try to get the palette name, gives the sprite name if no palette found
                    IniBgs[NBackgrounds].PaletteName = IniBgs[NBackgrounds].Name;
                    if (Ini.Strings.Length > 2)
                        IniBgs[NBackgrounds].PaletteName = Ini.Strings[2];
                }

                NBackgrounds++;
            }
            if (!FileOk)
            {
                Console.WriteLine("ERROR : File " + PathBackup + " could not be found. Please check the filename for a typo... Press any key to continue.");
                Console.ReadKey();
            }
            return FileOk;
        }

        static string InitBgInfos()
        {
            // Init the different sprite modes...
            BgModes = new string[10];
            BgModes[0] = "8bit";
            BgModes[1] = "16bit";
            BgModes[2] = "TileBg";
            BgModes[3] = "RotBg";
            BgModes[4] = "LargeMap";
            BgModes[5] = "InfiniteMap";
            BgModes[6] = "EasyBg";
            BgModes[7] = "16cFont";
            BgModes[8] = "8bitFont";
            BgModes[9] = "1bitFont";

            IniBgs = new IniBgStruct[10000]; // Memory alloc...

            // Get the starting line for backgrounds...
            int BgLine = -1;
            for (int i = 0; i < Ini.NLines; i++)
            {
                Ini.Strings = Ini.Lines[i].Split();
                if (Ini.Strings[0] == "#Backgrounds")
                {
                    BgLine = i;
                    break;
                }
            }

            BgLine++; // First bg info

            NBackgrounds = 0;
            for (int i = 0; ; i++)
            {
                if (BgLine + i == Ini.NLines)
                    break;
                Ini.Strings = Ini.Lines[BgLine + i].Split();
                if ((Ini.Strings[0].Length > 0) && (!Ini.Strings[0].StartsWith("#")))
                {
                    if (!GetBgInfo())
                        return "Sorry, " + IniBgs[NBackgrounds].Path + " does not exist !";
                }
                else
                    break; // On a trouvé la bonne ligne !
            }
            return "";
        }

        static bool GetTextureInfo()
        {
            // Puts the sprite infos into the sprite structure
            bool FileOk = true;
            IniTextures[NTextures].Path = ExtractPath();
            string PathBackup = IniTextures[NTextures].Path;

            FileOk = File.Exists(IniTextures[NTextures].Path);
            if (!FileOk && !Path.IsPathRooted(IniTextures[NTextures].Path))
            {
                IniTextures[NTextures].Path = System.AppDomain.CurrentDomain.BaseDirectory + IniTextures[NTextures].Path;
                FileOk = File.Exists(IniTextures[NTextures].Path);
            }

            if (FileOk)
            {
                IniTextures[NTextures].Name = ExtractName(IniTextures[NTextures].Path);



                // Try to get the color mode, sets to 256Colors by default
                IniTextures[NTextures].ColorMode = 1;
                //Console.Write(Ini.Strings.Length);
                if (Ini.Strings.Length > 1)
                    for (int i = 0; i < 6; i++)
                        if (Ini.Strings[1].ToLower() == TextureModes[i].ToLower())
                            IniTextures[NTextures].ColorMode = i;

                // Try to get the palette name, gives the sprite name if no palette found
                if (IniTextures[NTextures].ColorMode != 2) // If not 16bit...
                {
                    IniTextures[NTextures].PaletteName = IniTextures[NTextures].Name;
                    if (Ini.Strings.Length > 2)
                        IniTextures[NTextures].PaletteName = Ini.Strings[2];
                }

                NTextures++;
            }
            if (!FileOk)
            {
                Console.WriteLine("ERROR : File " + PathBackup + " could not be found. Please check the filename for a typo... Press any key to continue.");
                Console.ReadKey();
            }
           
            
            return FileOk;
        }

        static string InitTextureInfos()
        {
            // Init the different sprite modes...
            TextureModes = new string[6];
            TextureModes[0] = "16colors";
            TextureModes[1] = "256colors";
            TextureModes[2] = "16bit";
            TextureModes[3] = "4colors";
            TextureModes[4] = "A3I5";
            TextureModes[5] = "A5I3";
            IniTextures = new IniTexStruct[10000]; // Memory alloc...

            // Get the starting line for sprites...
            int TextureLine = -1;
            for (int i = 0; i < Ini.NLines; i++)
            {
                Ini.Strings = Ini.Lines[i].Split();
                if (Ini.Strings[0] == "#Textures")
                {
                    TextureLine = i;
                    break;
                }
            }

            TextureLine++; // First sprite info

            NTextures = 0;
            for (int i = 0; ; i++)
            {
                if (TextureLine + i == Ini.NLines)
                    break;
                Ini.Strings = Ini.Lines[TextureLine + i].Split();
                if ((Ini.Strings[0].Length > 0) && (!Ini.Strings[0].StartsWith("#")))
                {
                    if (!GetTextureInfo())
                        return "Sorry, " + IniTextures[NTextures].Path + " does not exist !";
                }
                else
                    break; // On a trouvé la bonne ligne !
            }
            return "";
        }







        static string InitIni(string IniFile)
        {
            // Open the ini file
            FileStream ThisFile = null;
            StreamReader Reader = null;

            Ini.Lines = new string[10000]; // Ini line reader
            Ini.NLines = 0;

            try
            {
                ThisFile = new FileStream(System.AppDomain.CurrentDomain.BaseDirectory + IniFile, FileMode.Open);
                Reader = new StreamReader(ThisFile);
                // Reads the whole ini file
                while ((Ini.Lines[Ini.NLines] = Reader.ReadLine()) != null)
                {
                    Ini.NLines++;
                }
            }
            catch (Exception ex)
            {
                return "Error: " + ex.Message;
            }
            finally
            {
                if (Reader != null) Reader.Close();
                if (ThisFile != null) ThisFile.Close();
            }
            return "";
        }

        static public string IniParse(string IniFile)
        {
            // Extract the basic ini lines
            string res = InitIni(IniFile);
            if (res != "") return res;

            InitTranspColor(); // TranspColor infos in the ini file...
            InitSpriteInfos(); // Number of sprites, sorts the info too
            InitBgInfos(); // Number of Backgrounds; sorts the info too
            InitTextureInfos(); // Textures...

            return "";
        }
    }
}
