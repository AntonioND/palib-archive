

//#define VERSION_LINUX


using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Drawing;
//using System.Windows.Forms;
using PAGfx.Common;

/*
 \¯¯¯¯\/¯¯¯|¯¯¯¯/¯¯¯¯/¯\/¯/   |¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
 | |_\ |/_||/¯¯´| |¯  \  /    | PAGfxConverter v0.9     |
 |  __/ _  |\_/¯/  ¯/ /  \    | By: Team PAlib          |
 |_/ |_/ |_|___/|_|¯ /_/\_\   | Last Modified: 29.09.06 |
                  Converter   |_________________________|
*/


namespace PAGfx.Main
{
    unsafe class Program
    {
        

        #region Variables

        static UInt16 PA_RGB(int r, int g, int b)
        {
            return (UInt16)((1 << 15) + r + (g << 5) + (b << 10));
        }
        static UInt16 PA_RGB8(int r, int g, int b)
        {
            return (UInt16)((1 << 15) + (r >> 3) + ((g >> 3) << 5) + ((b >> 3) << 10));
        }  // Avec le 1<<15, pour les palettes et fonds
        static UInt16 PA_RGB8b(int r, int g, int b)
        {
            return (UInt16)((r >> 3) + ((g >> 3) << 5) + ((b >> 3) << 10));
        } // Sans le 1<<15, pour les sprites

        static string BgPointers = "// Background Pointers :\r\n";

        // Data infos...
        struct PaletteStruct
        {
            public string Name;
            public UInt16[] Colors; // Toutes les couleurs
            public int[] OkColors; // Toutes les couleurs déjà utilisée seront à leur valeur normale, les autres à -1...
            public int NColors;  // Nombre de couleurs
        } ;
        struct SpriteStruct
        {
            public string Name;
            public UInt32[] Pixels; // Pour les pixels...
            public int ColorMode; // 0 pour 16 couleurs, 1 pour 256, 2 pour 16bit...
            public int NPalette; // Palette used
        } ;
        struct BgStruct
        {
            public string Name;
            public UInt32[] Tiles; // Tiles...
            public UInt32[] Map; // Map...
            public int BgMode; // 8bit, 16bit, TileBg, RotBg, LargeMap, InfiniteMap
            public int NPalette; // Palette used
            public int NTiles; // Number of tiles
        } ;
        public struct TexStruct
        {
            public string Name;
            public UInt32[] Pixels; // Pour les pixels...
            public int ColorMode; // 0 pour 16 couleurs, 1 pour 256, 2 pour 16bit...
            public int NPalette; // Palette used
        };

        static PaletteStruct[] Palette;
        static SpriteStruct Sprite;
        static BgStruct Background;
        static TexStruct Texture;

        static UInt16 NPalettes = 0; // Nombre de palettes

        static Bitmap readBitmap;

        static int swidth, sheight, width, height, x, y; // Variables temppour infos du fichier
        static UInt16[] buffer; // tableau temp

        static FileStream AllFileC; // Faire le fichier c avec tous les c...
        static FileStream AllFileH; // Faire le fichier c avec tous les c...
        static FileStream Log; // Log file...
        static BinaryWriter cwriter;
        static BinaryWriter hwriter;
        static BinaryWriter LogWriter;
        static BinaryWriter binWriter;
        static BinaryWriter infoWriter; // Will be used for Bg info

        static UInt32[,,] TilesInfo;

        static UInt32[,] NTilesInfo;
        /*struct TilesSortInfo
        {
            public int Next, Previous;
        }
        static TilesSortInfo[,] TilesSort;
        static int[,] FirstTile;
        static int[] CurrentTilePos;*/

        static byte oldmode;

        static int HFlip, VFlip; // For map tiles...

        static int KeepPal = 0; // keep palette or not. No by default...

        #endregion

        #region Sprites

        static void CreateSprite(int Spr)
        {
            readBitmap = new Bitmap(LoadINI.IniSprites[Spr].Path);
            Sprite.ColorMode = LoadINI.IniSprites[Spr].ColorMode;
            Sprite.Name = LoadINI.IniSprites[Spr].Name;

            PAGfxWrite("  " + Sprite.Name + " : " + LoadINI.SpriteModes[Sprite.ColorMode] + ", ");
            
            width = readBitmap.Width; height = readBitmap.Height;
            x = 0; y = 0;

            swidth = width; sheight = height;
            // On ajuste si sprite trop petit...
            if (!((swidth == 8) || (swidth == 16) || (swidth == 32) || (swidth == 64)))
            {
                if (swidth < 8) swidth = 8;
                else if (swidth < 16) swidth = 16;
                else if (swidth < 32) swidth = 32;
                else if (swidth < 64) swidth = 64;
            }
            // Adjust height to a multiple of 8...
            while ((sheight % 8)!=0) sheight++;

            PAGfxWrite(swidth+"x"+sheight+", ");

            if (Sprite.ColorMode != 2) // Uses palettes...
            {
                Sprite.NPalette = GetPal(LoadINI.IniSprites[Spr].PaletteName);
                PAGfxWrite("Pal : " + Palette[Sprite.NPalette].Name + "_Pal, ");
            }

            if (Sprite.ColorMode == 0) Create16CSprite(); // 16 colors
            if (Sprite.ColorMode == 1) Create256CSprite(); // 256 colors
            if (Sprite.ColorMode == 2) Create16BSprite(); // 16bit
            if (Sprite.ColorMode == 3) Create256CSprite(); // 256 colors, using the palette
            if (Sprite.ColorMode == 4) CreateSpritePal(); // Just create the palette...
            
        }


        static void CreateSpritePal()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            int palette = Sprite.NPalette; // Palette

            KeepPal = 1; // use the image's palette...
            Init8bitBuffer(palette);
            KeepPal = 0; // reset default state once that's done
            PAGfxWriteLine(" ");
        }


        static void Create256CSprite()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            Sprite.Pixels = new UInt32[swidth * sheight]; // Sprite final !
            int palette = Sprite.NPalette; // Palette

            int pixel = 0;
            if (Sprite.ColorMode == 3) KeepPal = 1; // use the image's palette...
            Init8bitBuffer(palette);
            KeepPal = 0; // reset default state once that's done

            for (y = 0; y < sheight; y += 8)// Blocs de 8x8
            {
                for (x = 0; x < swidth; x += 8)// Idem
                {
                    int tempy, pos;
                    for (tempy = 0; tempy < 8; tempy++) // On fait le bloc de 0...
                    {
                        pos = x + ((y + tempy) * swidth);
                        Sprite.Pixels[pixel] = buffer[pos];
                        Sprite.Pixels[pixel + 1] = buffer[pos + 1];
                        Sprite.Pixels[pixel + 2] = buffer[pos + 2];
                        Sprite.Pixels[pixel + 3] = buffer[pos + 3];
                        Sprite.Pixels[pixel + 4] = buffer[pos + 4];
                        Sprite.Pixels[pixel + 5] = buffer[pos + 5];
                        Sprite.Pixels[pixel + 6] = buffer[pos + 6];
                        Sprite.Pixels[pixel + 7] = buffer[pos + 7];
                        pixel += 8;
                    }
                }
            }


            // Et enfin, on écrit le fichier !!
            FileStream spritefile, binfile; // Permet de faire le fichier
            BinaryWriter spritewriter;
            string tempstring, binstring;

            tempstring = Sprite.Name + ".c";
            binstring = "bin/" + Sprite.Name + "_Sprite.bin";

            spritefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            spritewriter = new BinaryWriter(spritefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans

            


            spritewriter.Write(Encoding.UTF8.GetBytes("//Sprite created using PAGfx\r\n//This sprite uses " + Palette[palette].Name + "_Pal\r\n\r\n"));

            int maxj = swidth * sheight;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            tempstring = "const unsigned char " + Sprite.Name + "_Sprite[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned char " + Sprite.Name + "_Sprite[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Sprite.Name + "_Sprite[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));


            WriteData8bit(spritewriter, binWriter, tempstring, Sprite.Pixels, maxj);

            PAGfxWriteLine("-> " + Sprite.Name + "_Sprite");

            spritefile.Close(); // Libère la mémoire
            spritewriter.Close();
            binfile.Close();
            binWriter.Close();
        }

        static void Create16CSprite()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            Sprite.Pixels = new UInt32[swidth * sheight]; // Sprite final !
            int palette = Sprite.NPalette; // Palette

            int pixel = 0;

            Init8bitBuffer(palette);

            for (y = 0; y < sheight; y += 8)// Blocs de 8x8
            {
                for (x = 0; x < swidth; x += 8)// Idem
                {
                    int tempy, pos;
                    for (tempy = 0; tempy < 8; tempy++) // On fait le bloc de 0...
                    {
                        pos = x + ((y + tempy) * swidth);
                        Sprite.Pixels[pixel] = (UInt16)(buffer[pos] + (buffer[pos + 1] << 4));
                        Sprite.Pixels[pixel + 1] = (UInt16)(buffer[pos + 2] + (buffer[pos + 3] << 4));
                        Sprite.Pixels[pixel + 2] = (UInt16)(buffer[pos + 4] + (buffer[pos + 5] << 4));
                        Sprite.Pixels[pixel + 3] = (UInt16)(buffer[pos + 6] + (buffer[pos + 7] << 4));
                        if ((buffer[pos] >= 16) || (buffer[pos + 1] >= 16) || (buffer[pos+2] >= 16) || (buffer[pos + 3] >= 16)||
                            (buffer[pos+4] >= 16) || (buffer[pos + 5] >= 16) || (buffer[pos + 6] >= 16) || (buffer[pos + 7] >= 16))
                        {
                            Console.WriteLine("WARNING : Image has more than 16 colors...");
                            Console.ReadKey();
                        }
                        pixel += 4;
                    }
                }
            }

            // Et enfin, on écrit le fichier !!
            FileStream spritefile, binfile; // Permet de faire le fichier
            BinaryWriter spritewriter;
            string tempstring, binstring;

            tempstring = Sprite.Name + ".c";
            binstring = "bin/" + Sprite.Name + "_Sprite.bin";

            spritefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            spritewriter = new BinaryWriter(spritefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans
            
            spritewriter.Write(Encoding.UTF8.GetBytes("//Sprite created using PAGfx\r\n//This sprite uses " + Palette[palette].Name + "_Pal\r\n\r\n"));

            int maxj = (swidth * sheight) >> 1;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            tempstring = "const unsigned char " + Sprite.Name + "_Sprite[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned char " + Sprite.Name + "_Sprite[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Sprite.Name + "_Sprite[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));

            // On écrit dans le fichier
            WriteData8bit(spritewriter, binWriter, tempstring, Sprite.Pixels, maxj); // On écrit les données

            PAGfxWriteLine("-> " + Sprite.Name + "_Sprite");

            spritefile.Close(); // Libère la mémoire
            spritewriter.Close();
            binfile.Close();
            binWriter.Close();
        }


        static void Create16BSprite()
        {
            swidth = width; // pas le choix pour le 16 bit...
            Sprite.Pixels = new UInt32[swidth * sheight]; // Sprite final !

            UInt16 TempColor;

            // On vide la mémoire du sprite...
            for (y = height; y < sheight; y++)
                for (x = 0; x < width; x++)
                    Sprite.Pixels[x + (y * swidth)] = 0;
            for (y = 0; y < sheight; y++)
                for (x = width; x < swidth; x++)
                    Sprite.Pixels[x + (y * swidth)] = 0;

            // On va tout d'abord copier le sprite dans un tableau, puis on fera les tiles
            for (y = 0; y < height; y++)
            {
                for (x = 0; x < width; x++)
                { // Pour chaque pixel, on extrait les valeurs RGB...
                    Color c = readBitmap.GetPixel(x, y);
                    TempColor = (UInt16)PA_RGB8(c.R, c.G, c.B);
                 //   Console.WriteLine((TempColor) + " vs " + LoadINI.TranspColor.Color);
                    if (LoadINI.TranspColor.Color != TempColor) Sprite.Pixels[x + (y * swidth)] = (TempColor );
                    else Sprite.Pixels[x + (y * swidth)] = 0;
                }
               // Console.ReadKey();
            }
            
            // Et enfin, on écrit le fichier !!
            FileStream spritefile, binfile; // Permet de faire le fichier
            BinaryWriter spritewriter;
            string tempstring, binstring;

            tempstring = Sprite.Name + ".c";
            binstring = "bin/" + Sprite.Name + "_Sprite.bin";

            spritefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            spritewriter = new BinaryWriter(spritefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans


            spritewriter.Write(Encoding.UTF8.GetBytes("//Sprite created using PAGfx\r\n//16bit sprite\r\n\r\n"));

            int maxj = swidth * sheight;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned short " + Sprite.Name + "_Sprite[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned short " + Sprite.Name + "_Sprite[" + maxj + "] __attribute__ ((aligned (4))) ;  // 16bit sprite\r\n"));

            WriteData16bit(spritewriter, binWriter, tempstring, Sprite.Pixels, maxj);

            //Sprite.Pixels = new UInt16[256]; // nombre de pixels
            string temp = Sprite.Name + " : " + width + "x" + height + ", 16bit";

            PAGfxWriteLine("-> " + Sprite.Name + "_Sprite");

            spritefile.Close(); // Libère la mémoire
            spritewriter.Close();
            binfile.Close();
            binWriter.Close();
        }

        #endregion


        #region Textures

        static void CreateTexture(int Tex)
        {
            readBitmap = new Bitmap(LoadINI.IniTextures[Tex].Path);
            Texture.ColorMode = LoadINI.IniTextures[Tex].ColorMode;
            Texture.Name = LoadINI.IniTextures[Tex].Name;

            PAGfxWrite("  " + Texture.Name + " : " + LoadINI.TextureModes[Texture.ColorMode] + ", ");

            width = readBitmap.Width; height = readBitmap.Height;
            x = 0; y = 0;

            swidth = width; sheight = height;
            // On ajuste si sprite trop petit...
     /*       if (!((swidth == 8) || (swidth == 16) || (swidth == 32) || (swidth == 64)))
            {
                if (swidth < 8) swidth = 8;
                else if (swidth < 16) swidth = 16;
                else if (swidth < 32) swidth = 32;
                else if (swidth < 64) swidth = 64;
            }
            // Adjust height to a multiple of 8...
            while ((sheight % 8) != 0) sheight++;*/

            PAGfxWrite(swidth + "x" + sheight + ", ");

            if (Texture.ColorMode != 2) // Uses palettes...
            {
                Texture.NPalette = GetPal(LoadINI.IniTextures[Tex].PaletteName);
                PAGfxWrite("Pal : " + Palette[Texture.NPalette].Name + "_Pal, ");
            }

            if (Texture.ColorMode == 0) Create16CTexture(); // 16 colors
            if (Texture.ColorMode == 1) Create256CTexture(); // 256 colors
            if (Texture.ColorMode == 2) Create16BTexture(); // 16bit
            if (Texture.ColorMode == 3) Create4CTexture(); // 256 colors, using the palette
            if (Texture.ColorMode == 4) CreateA3I5Texture();
            if (Texture.ColorMode == 5) CreateA5I3Texture();
            //if (Texture.ColorMode == 4) CreateSpritePal(); // Just create the palette...

        }


        static void CreateTexturePal()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            int palette = Texture.NPalette; // Palette

            KeepPal = 1; // use the image's palette...
            Init8bitBuffer(palette);
            KeepPal = 0; // reset default state once that's done
            PAGfxWriteLine(" ");
        }


        static void Create256CTexture()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            Texture.Pixels = new UInt32[swidth * sheight]; // Sprite final !
            int palette = Texture.NPalette; // Palette

            Init8bitBuffer(palette);
            KeepPal = 0; // reset default state once that's done
            int pos = 0;
            for (y = 0; y < sheight; y ++)
            {
                for (x = 0; x < swidth; x ++)// Idem
                {
                    Texture.Pixels[pos] = buffer[pos];
                    pos++;
                }
            }


            // Et enfin, on écrit le fichier !!
            FileStream texturefile, binfile; // Permet de faire le fichier
            BinaryWriter texturewriter;
            string tempstring, binstring;

            tempstring = Texture.Name + ".c";
            binstring = "bin/" + Texture.Name + "_Texture.bin";

            texturefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            texturewriter = new BinaryWriter(texturefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans




            texturewriter.Write(Encoding.UTF8.GetBytes("//Texture created using PAGfx\r\n//This texture uses " + Palette[palette].Name + "_Pal\r\n\r\n"));

            int maxj = swidth * sheight;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));


            WriteData8bit(texturewriter, binWriter, tempstring, Texture.Pixels, maxj);

            PAGfxWriteLine("-> " + Texture.Name + "_Texture");

            texturefile.Close(); // Libère la mémoire
            texturewriter.Close();
            binfile.Close();
            binWriter.Close();
        }


        static void CreateA3I5Texture()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            Texture.Pixels = new UInt32[swidth * sheight]; // Sprite final !
            int palette = Texture.NPalette; // Palette

            Init8bitBuffer(palette);
            KeepPal = 0; // reset default state once that's done
            int pos = 0;
            int alpha = 0;
            
            for (y = 0; y < sheight; y ++)
            {
                for (x = 0; x < swidth; x ++)// Idem
                {
                 //   Console.WriteLine((readBitmap.GetPixel(x, y).R) >> 5);
                    alpha = (readBitmap.GetPixel(x, y).A) >> 5; // 0-7
                    Texture.Pixels[pos] = (uint)(buffer[pos] + (alpha << 5));
                    
                    if (buffer[pos] >= 32)
                    {
                        Console.WriteLine("WARNING : Image has more than 32 colors...");
                        Console.ReadKey();
                    }
                    pos++;
                }
            }


            // Et enfin, on écrit le fichier !!
            FileStream texturefile, binfile; // Permet de faire le fichier
            BinaryWriter texturewriter;
            string tempstring, binstring;

            tempstring = Texture.Name + ".c";
            binstring = "bin/" + Texture.Name + "_Texture.bin";

            texturefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            texturewriter = new BinaryWriter(texturefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans




            texturewriter.Write(Encoding.UTF8.GetBytes("//Texture created using PAGfx\r\n//This texture uses " + Palette[palette].Name + "_Pal\r\n\r\n"));

            int maxj = swidth * sheight;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));


            WriteData8bit(texturewriter, binWriter, tempstring, Texture.Pixels, maxj);

            PAGfxWriteLine("-> " + Texture.Name + "_Texture");

            texturefile.Close(); // Libère la mémoire
            texturewriter.Close();
            binfile.Close();
            binWriter.Close();
        }



        static void CreateA5I3Texture()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            Texture.Pixels = new UInt32[swidth * sheight]; // Sprite final !
            int palette = Texture.NPalette; // Palette

            Init8bitBuffer(palette);
            KeepPal = 0; // reset default state once that's done
            int pos = 0;
            int alpha;
            for (y = 0; y < sheight; y++)
            {
                for (x = 0; x < swidth; x++)// Idem
                {
                    alpha = (readBitmap.GetPixel(x, y).A) >> 3; // 0-7
                    Texture.Pixels[pos] = (uint)(buffer[pos] + (alpha << 3));
                    if (buffer[pos] >= 32)
                    {
                        Console.WriteLine("WARNING : Image has more than 8 colors...");
                        Console.ReadKey();
                    }
                    pos++;
                }
            }


            // Et enfin, on écrit le fichier !!
            FileStream texturefile, binfile; // Permet de faire le fichier
            BinaryWriter texturewriter;
            string tempstring, binstring;

            tempstring = Texture.Name + ".c";
            binstring = "bin/" + Texture.Name + "_Texture.bin";

            texturefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            texturewriter = new BinaryWriter(texturefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans




            texturewriter.Write(Encoding.UTF8.GetBytes("//Texture created using PAGfx\r\n//This texture uses " + Palette[palette].Name + "_Pal\r\n\r\n"));

            int maxj = swidth * sheight;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));


            WriteData8bit(texturewriter, binWriter, tempstring, Texture.Pixels, maxj);

            PAGfxWriteLine("-> " + Texture.Name + "_Texture");

            texturefile.Close(); // Libère la mémoire
            texturewriter.Close();
            binfile.Close();
            binWriter.Close();
        }


        

        static void Create16CTexture()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le texture
            Texture.Pixels = new UInt32[swidth * sheight]; // Texture final !
            int palette = Texture.NPalette; // Palette

            int pixel = 0;

            Init8bitBuffer(palette);
            int pos = 0;
          //  Console.Write(swidth + " * " + sheight);
            for (y = 0; y < sheight; y ++)// Blocs de 8x8
            {
                for (x = 0; x < swidth; x +=2)// Idem
                {
                    Texture.Pixels[pixel] = (UInt16)(buffer[pos] + (buffer[pos + 1] << 4));
                    if ((buffer[pos] >= 16) || (buffer[pos + 1] >= 16))
                    {
                        Console.WriteLine("WARNING : Image has more than 16 colors...");
                        Console.ReadKey();
                    }
                    pixel ++;
                    pos += 2;
                    
          //          Console.Write(Texture.Pixels[pixel--] + ", ");
                }
            }

            // Et enfin, on écrit le fichier !!
            FileStream texturefile, binfile; // Permet de faire le fichier
            BinaryWriter texturewriter;
            string tempstring, binstring;

            tempstring = Texture.Name + ".c";
            binstring = "bin/" + Texture.Name + "_Texture.bin";

            texturefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            texturewriter = new BinaryWriter(texturefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans

            texturewriter.Write(Encoding.UTF8.GetBytes("//Texture created using PAGfx\r\n//This texture uses " + Palette[palette].Name + "_Pal\r\n\r\n"));

            int maxj = (swidth * sheight) >> 1;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));

            // On écrit dans le fichier
            WriteData8bit(texturewriter, binWriter, tempstring, Texture.Pixels, maxj); // On écrit les données

            PAGfxWriteLine("-> " + Texture.Name + "_Texture");

            texturefile.Close(); // Libère la mémoire
            texturewriter.Close();
            binfile.Close();
            binWriter.Close();
        }

        static void Create4CTexture()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le texture
            Texture.Pixels = new UInt32[swidth * sheight/4]; // Texture final !
            int palette = Texture.NPalette; // Palette

            int pixel = 0;

            Init8bitBuffer(palette);
            int pos = 0;
            //  Console.Write(swidth + " * " + sheight);
            for (y = 0; y < sheight; y++)// Blocs de 8x8
            {
                for (x = 0; x < swidth; x += 4)// Idem
                {
                    Texture.Pixels[pixel] = (byte)(buffer[pos] + (buffer[pos + 1] << 2) + (buffer[pos + 2] << 4)  + (buffer[pos + 3] << 6));
                    if ((buffer[pos] >= 4) || (buffer[pos + 1] >= 4) || (buffer[pos+2] >= 4) || (buffer[pos + 3] >= 4))
                    {
                        Console.WriteLine("WARNING : Image has more than 4 colors...");
                        Console.ReadKey();
                    }
                    pixel++;
                    pos += 4;

                    //          Console.Write(Texture.Pixels[pixel--] + ", ");
                }
            }

            // Et enfin, on écrit le fichier !!
            FileStream texturefile, binfile; // Permet de faire le fichier
            BinaryWriter texturewriter;
            string tempstring, binstring;

            tempstring = Texture.Name + ".c";
            binstring = "bin/" + Texture.Name + "_Texture.bin";

            texturefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            texturewriter = new BinaryWriter(texturefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans

            texturewriter.Write(Encoding.UTF8.GetBytes("//Texture created using PAGfx\r\n//This texture uses " + Palette[palette].Name + "_Pal\r\n\r\n"));

            int maxj = (swidth * sheight) >> 2;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));

            // On écrit dans le fichier
            WriteData8bit(texturewriter, binWriter, tempstring, Texture.Pixels, maxj); // On écrit les données

            PAGfxWriteLine("-> " + Texture.Name + "_Texture");

            texturefile.Close(); // Libère la mémoire
            texturewriter.Close();
            binfile.Close();
            binWriter.Close();
        }


        static void Create16BTexture()
        {
            swidth = width; // pas le choix pour le 16 bit...
            Texture.Pixels = new UInt32[swidth * sheight]; // Texture final !

            UInt16 TempColor;

            // On vide la mémoire du texture...
            for (y = height; y < sheight; y++)
                for (x = 0; x < width; x++)
                    Texture.Pixels[x + (y * swidth)] = 0;
            for (y = 0; y < sheight; y++)
                for (x = width; x < swidth; x++)
                    Texture.Pixels[x + (y * swidth)] = 0;

            // On va tout d'abord copier le texture dans un tableau, puis on fera les tiles
            for (y = 0; y < height; y++)
            {
                for (x = 0; x < width; x++)
                { // Pour chaque pixel, on extrait les valeurs RGB...
                    Color c = readBitmap.GetPixel(x, y);
                    TempColor = (UInt16)PA_RGB8(c.R, c.G, c.B);
                    //   Console.WriteLine((TempColor) + " vs " + LoadINI.TranspColor.Color);
                    if (LoadINI.TranspColor.Color != TempColor) Texture.Pixels[x + (y * swidth)] = (TempColor);
                    else Texture.Pixels[x + (y * swidth)] = 0;
                }
                // Console.ReadKey();
            }

            // Et enfin, on écrit le fichier !!
            FileStream texturefile, binfile; // Permet de faire le fichier
            BinaryWriter texturewriter;
            string tempstring, binstring;

            tempstring = Texture.Name + ".c";
            binstring = "bin/" + Texture.Name + "_Texture.bin";

            texturefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            texturewriter = new BinaryWriter(texturefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans


            texturewriter.Write(Encoding.UTF8.GetBytes("//Texture created using PAGfx\r\n//16bit texture\r\n\r\n"));

            int maxj = swidth * sheight;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned short " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned short " + Texture.Name + "_Texture[" + maxj + "] __attribute__ ((aligned (4))) ;  // 16bit texture\r\n"));

            WriteData16bit(texturewriter, binWriter, tempstring, Texture.Pixels, maxj);

            //Texture.Pixels = new UInt16[256]; // nombre de pixels
            string temp = Texture.Name + " : " + width + "x" + height + ", 16bit";

            PAGfxWriteLine("-> " + Texture.Name + "_Texture");

            texturefile.Close(); // Libère la mémoire
            texturewriter.Close();
            binfile.Close();
            binWriter.Close();
        }

        #endregion



        #region Backgrounds



        static void CreateBg(int Bg)
        {
            //if (Background.BgMode != 1) // If palettes are used
            
            

            readBitmap = new Bitmap(LoadINI.IniBgs[Bg].Path);
            
            Background.BgMode = LoadINI.IniBgs[Bg].BgMode;
            Background.Name = LoadINI.IniBgs[Bg].Name;


            PAGfxWrite("  " + Background.Name + " : " + LoadINI.BgModes[Background.BgMode] + ", ");

            width = readBitmap.Width; height = readBitmap.Height;
            x = 0; y = 0;

            swidth = width; sheight = height;

            if (Background.BgMode == 6) Background.BgMode = 2; // Go to Tiled Bg mode...

            if (Background.BgMode == 2) // 256 color tiled modes
            { // If tiled Bg too small...        
                if (swidth <= 256) swidth = 256; // Minimum 256...
                else if ((sheight > 256) && (swidth < 512)) swidth = 512; // Minimum 192...
                if (sheight <= 192) sheight = 192; // Minimum 192...
                if ((swidth > 256) && (sheight <= 256)) sheight = 256; // Minimum 256 if map if larger

                //8 pixel padding...
                while ((swidth & 7) != 0) swidth++;
                while ((sheight & 7) != 0) sheight++;
            }

            if (Background.BgMode == 3)// RotBg
            {
                if ((swidth <= 128) && (sheight <= 128))
                {
                    swidth = 128; sheight = 128;
                }
                else if ((swidth <= 256) && (sheight <= 256))
                {
                    swidth = 256; sheight = 256;
                }
                else if ((swidth <= 512) && (sheight <= 512))
                {
                    swidth = 512; sheight = 512;
                }
                else
                {
                    swidth = 1024; sheight = 1024;
                }

            }


            PAGfxWrite(swidth + "x" + sheight + ", ");

            if (Background.BgMode != 1) // Uses palettes...
            {
                Background.NPalette = GetPal(LoadINI.IniBgs[Bg].PaletteName);
                PAGfxWrite("Pal : " + Palette[Background.NPalette].Name + "_Pal, ");
            }

            oldmode = (byte)Background.BgMode;

            if (Background.BgMode == 0) Create8bBg();     // 8bit
            else if (Background.BgMode == 1) Create16bBg();     // 16bit
            else if (Background.BgMode == 2) CreateTiledBg();  // Tiled or EasyBg
            else if (Background.BgMode == 3) CreateTiledBg();   // RotBg
            else if (Background.BgMode == 4) CreateTiledBg();   // LargeMap
            else if (Background.BgMode == 5) CreateTiledBg();   // Infinite
            else if (Background.BgMode == 7) CreateTiledBg();  // 16 colors font
            else if (Background.BgMode == 8) CreateTiledBg();  // 8bit font
            else if (Background.BgMode == 9) CreateTiledBg();  // 1bit font
        }

        static void Create16bBg()
        {
            Background.Tiles = new UInt32[swidth * sheight]; // Sprite final !

            UInt16 TempColor;

            // On vide la mémoire du fond...
            for (y = height; y < sheight; y++)
                for (x = 0; x < width; x++)
                    Background.Tiles[x + (y * swidth)] = 1 << 15;
            for (y = 0; y < sheight; y++)
                for (x = width; x < swidth; x++)
                    Background.Tiles[x + (y * swidth)] = 1 << 15;

            // On va tout d'abord copier le sprite dans un tableau, puis on fera les tiles
            for (y = 0; y < height; y++)
            {
                for (x = 0; x < width; x++)
                { // Pour chaque pixel, on extrait les valeurs RGB...
                    Color c = readBitmap.GetPixel(x, y);
                    TempColor = (UInt16)PA_RGB8(c.R, c.G, c.B);
                    Background.Tiles[x + (y * swidth)] = TempColor;
                }
            }

            // Et enfin, on écrit le fichier !!
            FileStream tilefile, binfile; // Permet de faire le fichier
            BinaryWriter tilewriter;
            string tempstring, binstring;

            tempstring = Background.Name + ".c";
            binstring = "bin/" + Background.Name + "_Bitmap.bin";

            tilefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            tilewriter = new BinaryWriter(tilefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans

            tilewriter.Write(Encoding.UTF8.GetBytes("// 16bit Bitmap created using PAGfx\r\n\r\n"));

            int maxj = swidth * sheight;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned short " + Background.Name + "_Bitmap[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned short " + Background.Name + "_Bitmap[" + maxj + "] __attribute__ ((aligned (4))) ;  // 16bit bitmap\r\n\r\n"));

            WriteData16bit(tilewriter, binWriter, tempstring, Background.Tiles, maxj);

            //Sprite.Pixels = new UInt16[256]; // nombre de pixels
            string temp = Background.Name + " : " + width + "x" + height + ", 16bit";

            PAGfxWriteLine("-> " + Background.Name + "_Bitmap");

            tilefile.Close(); // Libère la mémoire
            tilewriter.Close();
            binfile.Close(); // Libère la mémoire
            binWriter.Close();
        }

        static void Create8bBg()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            Background.Tiles = new UInt32[swidth * sheight]; // Sprite final !

            int palette = Background.NPalette; // Palette

            int pixel = 0;

            Init8bitBuffer(palette);

            for (y = 0; y < sheight; y ++)// Blocs de 8x8
            {
                for (x = 0; x < swidth; x ++)// Idem
                {
                    Background.Tiles[pixel] = buffer[x + (y * swidth)];
                    pixel++;
                }
            }

            // Et enfin, on écrit le fichier !!
            FileStream tilefile, binfile; // Permet de faire le fichier
            BinaryWriter tilewriter;
            string tempstring, binstring;

            tempstring = Background.Name + ".c";
            binstring = "bin/" + Background.Name + "_Bitmap.bin";

            tilefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            tilewriter = new BinaryWriter(tilefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans

            tilewriter.Write(Encoding.UTF8.GetBytes("// 8bit Bitmap converted using PAGfx\r\n//This bitmap uses " + Palette[palette].Name + "_Pal\r\n\r\n"));

            int maxj = swidth * sheight;
            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
            tempstring = "const unsigned char " + Background.Name + "_Bitmap[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));
            tempstring = "const unsigned char " + Background.Name + "_Bitmap[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Background.Name + "_Bitmap[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n\r\n"));

            WriteData8bit(tilewriter, binWriter, tempstring, Background.Tiles, maxj);

            PAGfxWriteLine("-> "+Background.Name + "_Bitmap");

            tilefile.Close(); // Libère la mémoire
            tilewriter.Close();
            binfile.Close(); // Libère la mémoire
            binWriter.Close();
        }


        #region TileSys

        static Int32 CompareNorm(int CurrentPos, int firstcol)
        {
            int tile = 0;
            Int32 TileNumber = -1;
            Int32 OkTile = -1; // wrong number by default
            Int32 temppixel;
            bool ok = false;
            int BigY;
            for (tile = 0; (tile < NTilesInfo[0, firstcol]) && !ok; tile++)
            {
                ok = true; // true by default, proove it wrong
                TileNumber = (Int32)TilesInfo[0, firstcol, tile];
                //OkTile = (Int32)TileNumber; // still running

                // Writes the tile...
                temppixel = TileNumber * 8 * 8; // Taille des blocs...
                int tempx2 = 0, tempy2 = 0;
                int i, j;
                for (i = 0; (i < 8) && ok; i++) // On fait le bloc de 0...
                {
                    tempx2 = 0;
                    BigY = (tempy2 * swidth);
                    for (j = 0; (j < 8) && ok; j++)
                    {
                        ok = (Background.Tiles[temppixel] == buffer[CurrentPos + tempx2 + BigY]);
                        temppixel++;
                        tempx2++;
                    }
                    tempy2++;
                }
            }
            if (ok == false) OkTile = -1;
            else OkTile = TileNumber;
            return OkTile; // tells if same thing or not
        }

        static Int32 CompareHFlip(int CurrentPos, int firstcol)
        {
            int tile = 0;
            Int32 TileNumber = -1;
            Int32 OkTile = -1; // wrong number by default
            Int32 temppixel;
            bool ok = false;
            int BigY;
            for (tile = 0; (tile < NTilesInfo[1, firstcol]) && !ok; tile++)
            //for (TileNumber = 0; (TileNumber < Background.NTiles) && !ok; TileNumber++)
            {
                ok = true; // true by default, proove it wrong
                TileNumber = (Int32)TilesInfo[1, firstcol, tile];

                // Writes the tile...
                temppixel = TileNumber * 8 * 8; // Taille des blocs...
                int tempx2 = 7, tempy2 = 0;
                int i, j;
                for (i = 0; (i < 8) && ok; i++) // On fait le bloc de 0...
                {
                    tempx2 = 7;
                    BigY = (tempy2 * swidth);
                    for (j = 0; (j < 8) && ok; j++)
                    {
                        ok = (Background.Tiles[temppixel] == buffer[CurrentPos + tempx2 + BigY]);
                        temppixel++;
                        tempx2--;
                    }
                    tempy2++;
                }
            }
            if (ok == false) OkTile = -1;
            else OkTile = TileNumber;
            return OkTile; // tells if same thing or not
        }


        static Int32 CompareVFlip(int CurrentPos, int firstcol)
        {
            int tile = 0;
            Int32 TileNumber = -1;
            Int32 OkTile = -1; // wrong number by default
            Int32 temppixel;
            bool ok = false;
            int tempx2 = 0, tempy2 = 7;
            int i, j;
            int BigY;
            for (tile = 0; (tile < NTilesInfo[2, firstcol]) && !ok; tile++)
            //for (TileNumber = 0; (TileNumber < Background.NTiles) && !ok; TileNumber++)
            {
                ok = true; // true by default, proove it wrong
                TileNumber = (Int32)TilesInfo[2, firstcol, tile];

                // Writes the tile...
                temppixel = TileNumber * 8 * 8; // Taille des blocs...
                tempx2 = 0;
                tempy2 = 7;

                for (i = 0; (i < 8) && ok; i++) // On fait le bloc de 0...
                {
                    tempx2 = 0;
                    BigY = (tempy2 * swidth);
                    for (j = 0; (j < 8) && ok; j++)
                    {
                        ok = (Background.Tiles[temppixel] == buffer[CurrentPos + tempx2 + BigY]);
                        temppixel++;
                        tempx2++;
                    }
                    tempy2--;
                }
//                if (OkTile != -1) ok = true; 
            }
            if (ok == false) OkTile = -1;
            else OkTile = TileNumber;
            return OkTile; // tells if same thing or not
        }


        static Int32 CompareHVFlip(int CurrentPos, int firstcol)
        {
            int tile = 0;
            Int32 TileNumber = -1;
            Int32 OkTile = -1; // wrong number by default
            Int32 temppixel;
            bool ok = false;
            int BigY;
            for (tile = 0; (tile < NTilesInfo[3, firstcol]) && !ok; tile++)
            {
                ok = true; // true by default, proove it wrong
                TileNumber = (Int32)TilesInfo[3, firstcol, tile];
                //OkTile = (Int32)TileNumber; // still running

                // Writes the tile...
                temppixel = TileNumber * 8 * 8; // Taille des blocs...
                int tempx2 = 7, tempy2 = 7;
                int i, j;
                for (i = 0; (i < 8) && ok; i++) // On fait le bloc de 0...
                {
                    tempx2 = 7;
                    BigY = (tempy2 * swidth);
                    for (j = 0; (j < 8) && ok; j++)
                    {
                        ok = (Background.Tiles[temppixel] == buffer[CurrentPos + tempx2 + BigY]);
                        temppixel++;
                        tempx2--;
                    }
                    tempy2--;
                }
            }
            if (ok == false) OkTile = -1;
            else OkTile = TileNumber;
            return OkTile; // tells if same thing or not
        }


        static Int32 AddNewTile(int Pos, int pixel)
        {
            int tile = Background.NTiles;

            // No flip tile
            TilesInfo[0, buffer[Pos], NTilesInfo[0, buffer[Pos]]] = (UInt32)tile; // New tile in the array
        //    if (FirstTile[0, buffer[Pos]] == -1) FirstTile[0, buffer[Pos]] = tile;

         //   TilesSort[0, CurrentTilePos[0]].Next = tile;
        //    TilesSort[0, tile].Previous = CurrentTilePos[0]; // Nothing after it yet
            NTilesInfo[0, buffer[Pos]]++; // One more tile in that category...
            //   TileSortPos[0, buffer[Pos]]++; // One more...
            // Hflip tile
            TilesInfo[1, buffer[Pos + 7], NTilesInfo[1, buffer[Pos + 7]]] = (UInt32)tile;
            NTilesInfo[1, buffer[Pos + 7]]++;
            Pos = x + ((y + 7) * swidth);
            // Vflip tile
            TilesInfo[2, buffer[Pos], NTilesInfo[2, buffer[Pos]]] = (UInt32)tile;
            NTilesInfo[2, buffer[Pos]]++;
            // HVflip tile
            TilesInfo[3, buffer[Pos + 7], NTilesInfo[3, buffer[Pos + 7]]] = (UInt32)tile;
            NTilesInfo[3, buffer[Pos + 7]]++;

            int tempy;
            // Writes the tile...
            for (tempy = 0; tempy < 8; tempy++) // On fait le bloc de 0...
            {
                Pos = x + ((y + tempy) * swidth);
                Background.Tiles[pixel] = buffer[Pos];
                Background.Tiles[pixel + 1] = buffer[Pos + 1];
                Background.Tiles[pixel + 2] = buffer[Pos + 2];
                Background.Tiles[pixel + 3] = buffer[Pos + 3];
                Background.Tiles[pixel + 4] = buffer[Pos + 4];
                Background.Tiles[pixel + 5] = buffer[Pos + 5];
                Background.Tiles[pixel + 6] = buffer[Pos + 6];
                Background.Tiles[pixel + 7] = buffer[Pos + 7];
                pixel += 8;
            }



            Background.NTiles++;
            HFlip = 0; VFlip = 0; // par défaut, pas de flip
            return tile;

        }

        #endregion

        static int CalcPourcentage(int pourcentage, DateTime CurrentTime, int oldseconds)
        {
            Console.CursorLeft -= pourcentage.ToString().Length + 4;
            pourcentage = y * 100 / (height - 8);
            pourcentage = (pourcentage * pourcentage) / 100;
            Console.Write(pourcentage + "%...");
            if (pourcentage > 4)
            {
                DateTime CurrentTime2 = DateTime.Now;
                TimeSpan TimeLeft = CurrentTime2.Subtract(CurrentTime);
                int seconds = (int)TimeLeft.TotalSeconds;
                if (oldseconds != seconds)  // Every second...
                {
                    seconds = seconds * (100 - pourcentage) / pourcentage;
                    string Time = " Time Left : " + seconds + " seconds   ";
                    Console.Write(Time);
                    Console.CursorLeft -= Time.Length;
                }
            }
            return pourcentage;
        }


        static void CreateTiledBg()
        {
            buffer = new UInt16[swidth * sheight]; // Tableau pour remplir le sprite
            Background.Tiles = new UInt32[swidth * sheight]; // Final background!

              //      TilesInfo[,,] // first col of tiles
            int i, j;
            for (i = 0; i < 4; i++)
                for (j = 0; j < 256; j++)
                    NTilesInfo[i, j] = 0; // NO tiles by default

            int palette = Background.NPalette; // Palette
            /*
            for (i = 0; i < 256; i++)
            {
                FirstTile[0, i] = -1;  // No tiles in it...
                FirstTile[1, i] = -1;
                FirstTile[2, i] = -1;
                FirstTile[3, i] = -1;
            }*/

            int tempheight = sheight >> 3; int tempwidth = swidth >> 3; 
            Background.Map = new UInt32[tempwidth * tempheight]; // Final background!

            int pixel = 0;
            Background.NTiles = 0; // A la base, pas de tiles...

            Init8bitBuffer(palette);

            // Map layout
            int position = 0;

            int Pos;

#if (!VERSION_LINUX)
            int pourcentage = 0;
            int oldseconds = 0;
            bool dopourcentage = false;

            DateTime CurrentTime = DateTime.Now;

            if (swidth * sheight > 512 * 512)
            {
                dopourcentage = true;
                Console.WriteLine(" ");
                Console.Write("Optimising tiles : 0%...");
            }
#endif

            for (y = 0; y < sheight; y += 8)// Blocs de 8x8
            {
                for (x = 0; x < swidth; x += 8)// Idem
                {
                    //int tempy;

                    int tile = -1;
                    Pos = x + (y * swidth);
                    
                    HFlip = 0; VFlip = 0; // No flip first
                    tile = CompareNorm(Pos, buffer[Pos]);
                    if ((tile == -1) && (Background.BgMode != 3) && (Background.BgMode != 7) && (Background.BgMode != 8) && (Background.BgMode != 9)) // Flip inactivated for rotbg and fonts...
                    {
                        HFlip = 1; VFlip = 0; // Now we'll test for HFlip
                        tile = CompareHFlip(Pos, buffer[Pos]);
                        if (tile == -1)
                        {
                            HFlip = 0; VFlip = 1;
                            tile = CompareVFlip(Pos, buffer[Pos]);
                            if (tile == -1)
                            {
                                HFlip = 1; VFlip = 1;
                                tile = CompareHVFlip(Pos, buffer[Pos]);
                            }
                        }

                    }
                    if (tile == -1) // new tile...
                    {
                        tile = AddNewTile(Pos, pixel);
                        pixel += 64;

                    }

                    // Check for infinite or normal mode...
                 //   if (Background.BgMode == 5) Background.Map[position] = (UInt32)(tile + (HFlip << 29) + (VFlip << 30));   // Last 2 bits for flips...
                    Background.Map[position] = (UInt32)(tile + (HFlip << 29) + (VFlip << 30));   // Last 2 bits for flips...
                 //   else Background.Map[position] = (UInt16)(tile + (HFlip << 10) + (VFlip<< 11)); 
                    position++;

                }
#if (!VERSION_LINUX)
                // Manage pourcentage...
                if (dopourcentage)
                {
                    pourcentage = CalcPourcentage(pourcentage, CurrentTime, oldseconds);
                }
#endif
            }
#if (!VERSION_LINUX)
            if (dopourcentage)
            {
                Console.CursorLeft -= 7;
                Console.WriteLine(" 100% !                                      ");
            }
#endif


            // Check if TiledBg->LargeMap->InfiniteMap ?
                // Switch to LargeMap if too big...
            if ((Background.BgMode == 2) && ((swidth > 512) || (sheight > 512))) Background.BgMode = 4; 
                // Switch to Infinite if needed
            if ((Background.BgMode == 4) && (Background.NTiles >= 1024)) Background.BgMode = 5;

            if (oldmode != Background.BgMode)
            {
                string[] BgModes = new string[10];
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
                PAGfxWrite("Converted to " + BgModes[Background.BgMode] + ", ");
            }


            // Et enfin, on écrit le fichier !!
            FileStream tilefile, binfile, infofile; // Permet de faire le fichier
            BinaryWriter tilewriter;
            string tempstring, binstring, infostring;

            tempstring = Background.Name + ".c";
            binstring = "bin/" + Background.Name + "_Map.bin";
            infostring = "bin/" + Background.Name + "_Info.bin";

            tilefile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
            tilewriter = new BinaryWriter(tilefile);  // Permet d'écrire dedans
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans

            tilewriter.Write(Encoding.UTF8.GetBytes("//Background converted using Mollusk's PAGfx Converter\r\n//This Background uses " + Palette[palette].Name + "_Pal\r\n\r\n"));
    //        tilewriter.Write(Encoding.UTF8.GetBytes("const int "+ Background.Name + "_Width = " + swidth + ";\r\n"));
    //        tilewriter.Write(Encoding.UTF8.GetBytes("const int "+ Background.Name + "_Height = " + sheight + ";\r\n\r\n"));

            
            tilewriter.Write(Encoding.UTF8.GetBytes("const int " + Background.Name + "_Info[3] = {" + Background.BgMode+ ", " + swidth + ", " + sheight + "}; // BgMode, Width, Height\r\n\r\n"));


            infofile = new FileStream(infostring, FileMode.Create); // Créé le fichier
            infoWriter = new BinaryWriter(infofile);  // Permet d'écrire dedans
            infoWriter.Write((UInt32)(Background.BgMode));  // First : BgMode
            infoWriter.Write((UInt32)(swidth));  // Second : Width
            infoWriter.Write((UInt32)(sheight));  // Third : Height


            // Write the font data if needed...
            if ((Background.BgMode == 7) || (Background.BgMode == 8) || (Background.BgMode == 9)) 
            {
                CreateFontSizes(tilewriter, hwriter);
            }









            // Write the map.....
            int maxj = tempwidth * tempheight;

            string namestring = "#include " + '"' + tempstring + '"' + "\r\n";

            cwriter.Write(Encoding.UTF8.GetBytes(namestring));

      //      hwriter.Write(Encoding.UTF8.GetBytes("extern const int " + Background.Name + "_Width, "+ Background.Name + "_Height;\r\n"));
            hwriter.Write(Encoding.UTF8.GetBytes("extern const int " + Background.Name + "_Info[3]; // BgMode, Width, Height\r\n"));

            if ((Background.BgMode == 3))// Rotbg, en char au lieu de short
            {
                hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Background.Name + "_Map[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));
                tempstring = "const unsigned char " + Background.Name + "_Map[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            }
            else if (Background.BgMode == 5)
            {
                hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned int " + Background.Name + "_Map[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));
                tempstring = "const unsigned int " + Background.Name + "_Map[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            }
            else { // Tiled/Largemap, convert to u16 format
                uint flip;
                for (position = 0; position < maxj; position++)
                {
                    flip = Background.Map[position] >> 29;
                    Background.Map[position] = (Background.Map[position] & 1023) + (flip << 10);
                }
                hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned short " + Background.Name + "_Map[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));
                tempstring = "const unsigned short " + Background.Name + "_Map[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
            }

            int mapsize = maxj;


            if (Background.BgMode == 3){ // RotBg
                WriteData8bit(tilewriter, binWriter, tempstring, Background.Map, maxj);
            }
            else if ((Background.BgMode == 4) || (Background.BgMode == 7) || (Background.BgMode == 8) || (Background.BgMode == 7) || (Background.BgMode == 9)) // LargeMap and fonts
            {
                WriteData16bit(tilewriter, binWriter, tempstring, Background.Map, maxj);
            }
            else if (Background.BgMode == 5) // InfiniteMap
            {
                WriteData32bit(tilewriter, binWriter, tempstring, Background.Map, maxj);
            }
            else if (Background.BgMode == 2) // Si TiledBg, optimiser pour DMACopy...
            {
                tilewriter.Write(Encoding.UTF8.GetBytes(tempstring));
                j = 0;
                int xlimit = 256; if (swidth > 256) xlimit = 512;
                int ylimit = 256; if (sheight > 256) ylimit = 512;
                int xtemp, ytemp;

                int limitx = xlimit >> 3; int limity = ylimit >> 3;

                Console.WriteLine(limitx + ", " + limity);
                for (ytemp = 0; ytemp < limity; ytemp += 32)
                {
                    for (xtemp = 0; xtemp < limitx; xtemp += 32)
                    {
                        for (y = ytemp; (y < 32 + ytemp) && (y < tempheight); y++)
                        {
                            //Console.WriteLine(y + "/" + tempheight);
                            for (x = xtemp; (x < 32 + xtemp) && (x < tempwidth); x++)
                            {
                                j = x + (y * limitx);
                                if (j < maxj - 1)
                                {
                                    //Console.Write(j+", ");
                                    tilewriter.Write(Encoding.UTF8.GetBytes(Background.Map[j] + ", "));
                                    binWriter.Write((short)(Background.Map[j] & 65535));
                                    if (((j + 1) & 15) == 0) tilewriter.Write(Encoding.UTF8.GetBytes("\r\n"));
                                }
                            }
                        }
                    }
                }
                tilewriter.Write(Encoding.UTF8.GetBytes(Background.Map[maxj - 1] + "\r\n};\r\n\r\n"));
                binWriter.Write((short)(Background.Map[maxj - 1] & 65535));
            }

            // Close Map files, on to Tile files...
            binfile.Close(); // Libère la mémoire
            binWriter.Close();

            // On to the tiles...
            binstring = "bin/" + Background.Name + "_Tiles.bin";
            binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
            binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans

            // Write the tiles.....
            maxj = Background.NTiles*8*8;
             

            if (Background.BgMode == 7)
            {
                hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Background.Name + "_Tiles[" + maxj/2 + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));

                tempstring = "const unsigned char " + Background.Name + "_Tiles[" + maxj/2 + "] __attribute__ ((aligned (4))) = {\r\n";
                WriteData4bit(tilewriter, binWriter, tempstring, Background.Tiles, maxj);
                maxj = maxj >> 1; // only half the space needed
            }
            else if (Background.BgMode == 9)
            {
                hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Background.Name + "_Tiles[" + maxj/8 + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));

                tempstring = "const unsigned char " + Background.Name + "_Tiles[" + maxj/8 + "] __attribute__ ((aligned (4))) = {\r\n";
                WriteData1bit(tilewriter, binWriter, tempstring, Background.Tiles, maxj);
            }
            else
            {
                hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned char " + Background.Name + "_Tiles[" + maxj + "] __attribute__ ((aligned (4))) ;  // Pal : " + Palette[palette].Name + "_Pal\r\n"));

                tempstring = "const unsigned char " + Background.Name + "_Tiles[" + maxj + "] __attribute__ ((aligned (4))) = {\r\n";
                WriteData8bit(tilewriter, binWriter, tempstring, Background.Tiles, maxj);
            }

            PAGfxWriteLine(Background.NTiles + " tiles -> " + Background.Name + "_Tiles and " + Background.Name + "_Map");
            if ((Background.BgMode == 3) && (Background.NTiles > 256)) // RotBg with more than 256 tiles
            {
                PAGfxWriteLine("WARNING ! A RotBg cannot have more than 256 tiles, and your background has " + Background.NTiles + " tiles... Please rework it to have less tiles.. Press any key to continue");
                Console.ReadKey();
            }

            // Write the background pointer
            hwriter.Write(Encoding.UTF8.GetBytes("extern PAGfx_struct " + Background.Name + "; // background pointer\r\n\r\n"));

            BgPointers = BgPointers + "PAGfx_struct " + Background.Name + " = {(void*)" + Background.Name + "_Map, " + mapsize + ", (void*)" + Background.Name + "_Tiles, " + maxj + ", (void*)" + Background.Name + "_Pal, (int*)" + Background.Name + "_Info };\r\n";
            //cwriter.Write(Encoding.UTF8.GetBytes("PAGfx_struct " + Background.Name + " = {(void*)" + Background.Name + "_Map, " + mapsize + ", (void*)" + Background.Name + "_Tiles, " + maxj + ", (void*)" + Background.Name + "_Pal, (int*)" + Background.Name + "_Info };\r\n"));


            tilefile.Close(); // Libère la mémoire
            tilewriter.Close();
            binfile.Close(); // Libère la mémoire
            binWriter.Close();
        }


        static void CreateFontSizes(BinaryWriter tilewriter, BinaryWriter hwriter){

            int blocksize = 8;
            int x, y;
            int size = 0; // font height
            if (width == 512) blocksize = 16; // big blocks...
            int[] letterwidth = new int[256];
            int lettersize;
            int i;
            for (i = blocksize-1; i > 0; i--)
            {
                for (x = 0; (x < swidth) && (size == 0); x ++)
                {
                    for (y = 0; (y < sheight) && (size == 0); y += blocksize)
                    {
                        if (buffer[x + (y + i) * swidth] != 0) size = i+1;

                    }
                }
            }

            Console.WriteLine("Height : " + size);
            //tilewriter.Write(Encoding.UTF8.GetBytes("//Font Sizes : \r\nconst char " + Background.Name + "_Height = " + size + ";\r\n\r\n"));

            int bigx, bigy;
            int letter = 0;
            for (bigy = 0; (bigy < sheight); bigy += blocksize)
            {
                for (bigx = 0; (bigx < swidth); bigx += blocksize)
                {
                    lettersize = 0; // no letter, basically...
                    for (x = blocksize - 1; (x > -1)&&(lettersize == 0); x--)
                    {
                        for (y = blocksize - 1; (y > -1) && (lettersize == 0); y--)
                        {
                            if (buffer[x + bigx + (y + bigy) * swidth] != 0) lettersize = x+2;
                        }
                    }

                    letterwidth[letter] = lettersize;
                   // Console.Write(lettersize + ", ");
                    letter++;// next letter
                }
            }
            letterwidth[32] = letterwidth[33]; // space size

            // First, we'll look for the font height...



            tilewriter.Write(Encoding.UTF8.GetBytes("const char " + Background.Name + "_Sizes[256] = {\r\n"));

            int widthentry = 0;
            string entries;
            for (y = 0; y < 8; y++)
            {
                entries = "   ";
                for (x = 0; x < 31; x++)
                {
                    entries += letterwidth[widthentry] + ", ";
                    widthentry++;
                }
                if (y < 7) entries += letterwidth[widthentry] + ", \r\n";
                else entries += letterwidth[widthentry] + "\r\n}; \r\n\r\n";
                widthentry++;
                tilewriter.Write(Encoding.UTF8.GetBytes(entries));
            }



            hwriter.Write(Encoding.UTF8.GetBytes("#define " + Background.Name + "_Height " + size + "\r\n"));
            hwriter.Write(Encoding.UTF8.GetBytes("extern const char " + Background.Name + "_Sizes[256];\r\n"));
           // Console.ReadKey();
        }


        #endregion

        #region Pal&Data

        // Gets the palette, sprite, and Bg infos
        static int GetPal(string PalName)
        {
            int PalNumber = 0;
            bool PalExists = false; // You have to create it by default...
            for (int i = 0; (i < NPalettes) && !PalExists; i++)
            {
                if (PalName == Palette[i].Name)
                {
                    PalExists = true; // The palette exists...
                    PalNumber = i; // Palette number !
                }
            }
            if (!PalExists) // You have to create the palette...
            {
                PalNumber = NPalettes; // New palette number
                Palette[NPalettes].Name = PalName;
                Palette[NPalettes].NColors = 1; // Only one color, the transp color...
                Palette[NPalettes].Colors = new UInt16[256]; // 256 colors...
                Palette[NPalettes].OkColors = new int[32 * 32 * 32 * 2]; // All colors
                for (int j = 0; j < 32 * 32 * 32 * 2; j++) Palette[PalNumber].OkColors[j] = -1;
                Palette[NPalettes].OkColors[LoadINI.TranspColor.Color] = 0; // First color...
                Palette[NPalettes].Colors[0] = LoadINI.TranspColor.Color;   // First color...
                NPalettes++; // one more palette !
                //PAGfxWriteLine("New palette : " + PalName);
            }
            return PalNumber;
        }

        static void InitAllFileCH()
        {
            AllFileC = new FileStream("all_gfx.c", FileMode.Create); // Créé le fichier
            AllFileH = new FileStream("all_gfx.h", FileMode.Create); // Créé le fichier

            cwriter = new BinaryWriter(AllFileC);  // Permet d'écrire dedans
            hwriter = new BinaryWriter(AllFileH);  // Permet d'écrire dedans

            cwriter.Write(Encoding.UTF8.GetBytes("//Gfx converted using Mollusk's PAGfx Converter\r\n\r\n//This file contains all the .c, for easier inclusion in a project\r\n\r\n#ifdef __cplusplus\r\nextern "+'"'+"C"+'"'+" {\r\n#endif\r\n\r\n"));
            cwriter.Write(Encoding.UTF8.GetBytes("#include " + '"' + "all_gfx.h" + '"' + "\r\n\r\n"));
            hwriter.Write(Encoding.UTF8.GetBytes("//Gfx converted using Mollusk's PAGfx Converter\r\n\r\n//This file contains all the .h, for easier inclusion in a project\r\n\r\n#ifndef ALL_GFX_H\r\n#define ALL_GFX_H\r\n\r\n"));
            hwriter.Write(Encoding.UTF8.GetBytes("#ifndef PAGfx_struct\r\n    typedef struct{\r\n    void *Map;\r\n    int MapSize;\r\n    void *Tiles;\r\n    int TileSize;\r\n    void *Palette;\r\n    int *Info;\r\n} PAGfx_struct;\r\n#endif\r\n\r\n"));
        }


        static void WriteData1bit(BinaryWriter Writer, BinaryWriter binWriter, string tempstring, UInt32[] Data, int maxj)
        {
            Writer.Write(Encoding.UTF8.GetBytes(tempstring));
            byte value = 0;
            UInt32 j;
            for (j = 0; j < maxj - 8; j += 8) // On ajoute les données
            {
                value = 0;
                for (int i = 0; i < 8; i++) value += (byte)(Data[j + i] << i);
                Writer.Write(Encoding.UTF8.GetBytes(value + ", "));
                binWriter.Write((byte)(value));
                if (((j + 8) & 127) == 0) Writer.Write(Encoding.UTF8.GetBytes("\r\n"));
            }
            value = 0;
            for (int i = 0; i < 8; i++) value += (byte)(Data[j + i] << i);
            Writer.Write(Encoding.UTF8.GetBytes(value + "\r\n};\r\n\r\n"));
            binWriter.Write((byte)(value));
        }


        static void WriteData4bit(BinaryWriter Writer, BinaryWriter binWriter, string tempstring, UInt32[] Data, int maxj)
        {
            Writer.Write(Encoding.UTF8.GetBytes(tempstring));

            for (UInt32 j = 0; j < maxj - 2; j+=2) // On ajoute les données
            {
                Writer.Write(Encoding.UTF8.GetBytes(Data[j] + (Data[j + 1] << 4) + ", "));
                binWriter.Write((byte)(Data[j]+(Data[j+1]<<4)));
                if (((j + 2) & 15) == 0) Writer.Write(Encoding.UTF8.GetBytes("\r\n"));
            }
            Writer.Write(Encoding.UTF8.GetBytes(Data[maxj - 2] + (Data[maxj - 1] << 4) + "\r\n};\r\n\r\n"));
            //  reduce = (byte)(Data[maxj - 1]);
            //   reduce2 = reduce;
            binWriter.Write((byte)(Data[maxj - 2] + (Data[maxj - 1] << 4)));
        }

        static void WriteData8bit(BinaryWriter Writer, BinaryWriter binWriter, string tempstring, UInt32[] Data, int maxj)
        {
            Writer.Write(Encoding.UTF8.GetBytes(tempstring));
          //  byte reduce;
           // uint reduce2;
            for (UInt32 j = 0; j < maxj - 1; j++) // On ajoute les données
            {
                Writer.Write(Encoding.UTF8.GetBytes(Data[j] + ", "));
         //       reduce = (byte)(Data[j]);
               // reduce2 = reduce;
                binWriter.Write((byte)(Data[j]));
                //Console.WriteLine("after bin");   
              //  string test = Encoding.UTF8.GetBytes(Data[j]);
            /*    if (j == 1908)
                {
                    Console.WriteLine("1908 : " + Data[j] + " vs " + test);
                    Console.ReadKey();
            }*/
                if (((j + 1) & 15) == 0) Writer.Write(Encoding.UTF8.GetBytes("\r\n"));
            }
            Writer.Write(Encoding.UTF8.GetBytes(Data[maxj - 1] + "\r\n};\r\n\r\n"));
          //  reduce = (byte)(Data[maxj - 1]);
         //   reduce2 = reduce;
            binWriter.Write((byte)(Data[maxj - 1]));
        }

        static void WriteData16bit(BinaryWriter Writer, BinaryWriter binWriter, string tempstring, UInt32[] Data, int maxj)
        {
            Writer.Write(Encoding.UTF8.GetBytes(tempstring));
            for (UInt32 j = 0; j < maxj - 1; j++) // On ajoute les données
            {
                Writer.Write(Encoding.UTF8.GetBytes(Data[j] + ", "));
                binWriter.Write((short)(Data[j] & 65535));
                if (((j + 1) & 15) == 0) Writer.Write(Encoding.UTF8.GetBytes("\r\n"));
            }
            Writer.Write(Encoding.UTF8.GetBytes(Data[maxj - 1] + "\r\n};\r\n\r\n"));
            binWriter.Write((short)(Data[maxj - 1] & 65535));
        }

        static void WriteData32bit(BinaryWriter Writer, BinaryWriter binWriter, string tempstring, UInt32[] Data, int maxj)
        {
            Writer.Write(Encoding.UTF8.GetBytes(tempstring));
            for (UInt32 j = 0; j < maxj - 1; j++) // On ajoute les données
            {
                Writer.Write(Encoding.UTF8.GetBytes(Data[j] + ", "));
                binWriter.Write(Data[j]);
                if (((j + 1) & 15) == 0) Writer.Write(Encoding.UTF8.GetBytes("\r\n"));
            }
            Writer.Write(Encoding.UTF8.GetBytes(Data[maxj - 1] + "\r\n};\r\n\r\n"));
            binWriter.Write(Data[maxj - 1]);
        }

#if (!VERSION_LINUX)
        // For paletted sprites/bg
        static void Init8bitBuffer(int palette) // S'occupe de faire tous les buffers 8bit
        {
            UInt16 TempColor;
            // On vide la mémoire du sprite...
            for (y = height; y < sheight; y++)
                for (x = 0; x < width; x++ )
                    buffer[x + (y * swidth)] = 0;
            for (y = 0; y < sheight; y++) 
                for (x = width; x < swidth; x++)  
                    buffer[x + (y*swidth)] = 0;


            Rectangle rect = new Rectangle(0, 0, readBitmap.Width, readBitmap.Height);

            int pixel = 0;
            int col1;
            int col2;


            if (KeepPal == 0)  // Optimize the palette...
            {
                System.Drawing.Imaging.BitmapData bmpData = readBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format16bppRgb555); // directly in 555 pixel format !

                UInt16* ptr = (UInt16*)bmpData.Scan0.ToPointer();

                // On va tout d'abord copier les données dans un tableau, puis on fera les tiles
                for (y = 0; y < height; y++)
                {
                    for (x = 0; x < width; x++)
                    { // Pour chaque pixel, on extrait les valeurs RGB...
                        TempColor = *(UInt16*)(ptr + pixel);
                        col1 = (TempColor >> 10) & 31;
                        col2 = (TempColor >> 5) & 31;
                        TempColor = (ushort)((col1) + ((col2) << 5) + ((TempColor & 31) << 10) | (1 << 15));

                        if (Palette[palette].OkColors[TempColor] != -1)
                        { // La couleur est déjà là...
                            buffer[x + (y * swidth)] = (byte)Palette[palette].OkColors[TempColor];
                        }
                        else // La couleur n'existe pas encore dans la palette !
                        {
                            buffer[x + (y * swidth)] = (byte)AddColorToPalette(palette, TempColor);
                        }
                        pixel++;
                    }

                    if ((width & 1) != 0) pixel++;
                }

                readBitmap.UnlockBits(bmpData);

            }
            else  // Use the image palette...
            {
                bool ok = true;

                System.Drawing.Imaging.BitmapData bmpData = readBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);

                if (readBitmap.PixelFormat == System.Drawing.Imaging.PixelFormat.Format8bppIndexed) 
                {
                    readBitmap.UnlockBits(bmpData);
                    bmpData = readBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format8bppIndexed); // directly in 555 pixel format !
                }

                else if (readBitmap.PixelFormat == System.Drawing.Imaging.PixelFormat.Format4bppIndexed)
                {
                    readBitmap.UnlockBits(bmpData);
                    bmpData = readBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format4bppIndexed);
                }
                else
                {
                    Console.WriteLine("Your image is NOT a paletted image... Use the normal 256colors command....");
                    Console.ReadKey();
                    ok = false;
                }


                if (ok)
                {
                    byte* ptr = (byte*)bmpData.Scan0.ToPointer();

                    int i, r, g, b;

                    for (i = 0; i < readBitmap.Palette.Entries.Length; i++)  // Get palette entries
                    {
                        r = readBitmap.Palette.Entries[i].R>>3;
                        g = readBitmap.Palette.Entries[i].G>>3;
                        b = readBitmap.Palette.Entries[i].B>>3;
                        Palette[palette].Colors[i] = PA_RGB(r, g, b);
                    }

                    for (y = 0; y < height; y++)
                    {
                        for (x = 0; x < width; x++)
                        {
                            buffer[x + (y * swidth)] = *(byte*)(ptr + pixel);
                            pixel++;
                        }

                        if ((width & 1) != 0) pixel++;
                    }

                    Palette[palette].NColors = readBitmap.Palette.Entries.Length;
                }
                
            }

        }
#else
        static void Init8bitBuffer(int palette) // S'occupe de faire tous les buffers 8bit
        {
            UInt16 TempColor;
            // On vide la mémoire du sprite...
            for (y = height; y < sheight; y++)
                for (x = 0; x < width; x++)
                    buffer[x + (y * swidth)] = 0;
            for (y = 0; y < sheight; y++)
                for (x = width; x < swidth; x++)
                    buffer[x + (y * swidth)] = 0;

            int pixel = 0;
            int col1;
            int col2;

            // On va tout d'abord copier les données dans un tableau, puis on fera les tiles
            for (y = 0; y < height; y++)
            {
                for (x = 0; x < width; x++)
                { // Pour chaque pixel, on extrait les valeurs RGB...
                    //TempColor = readBitmap.GetPixel(x, y);
                    
                    col2 = readBitmap.GetPixel(x, y).G >> 3;
                    col1 = readBitmap.GetPixel(x, y).R >> 3;
                    TempColor = (ushort)((col1) + ((col2) << 5) + (((readBitmap.GetPixel(x, y).B >> 3)) << 10) | (1 << 15));

                    if (Palette[palette].OkColors[TempColor] != -1)
                    { // La couleur est déjà là...
                        buffer[x + (y * swidth)] = (byte)Palette[palette].OkColors[TempColor];
                    }
                    else // La couleur n'existe pas encore dans la palette !
                    {
                        buffer[x + (y * swidth)] = (byte)AddColorToPalette(palette, TempColor);
                    }
                    pixel++;
                }

                if ((width & 1) != 0) pixel++;
            }

        }
#endif

        static UInt16 AddColorToPalette(int palette, UInt16 color)
        {
            try
            {
                Palette[palette].OkColors[color] = Palette[palette].NColors;
                Palette[palette].Colors[Palette[palette].NColors] = color;
                Palette[palette].NColors++; // Une couleur de plus
                return ((byte)Palette[palette].OkColors[color]); 
            }
            catch
            {
                PAGfxWriteLine("\r\n Error : Palette " + Palette[palette].Name + " has more than 256 colors. Please use some different palettes or reduce your file's color depth");
                PAGfxWriteLine("Press any key to exit...");
                Console.ReadKey();
                //Application.Exit();
            }

            //Palette[palette].Colors[Palette[palette].NColors] = color;

            return 0;
        }

        static void WritePalettes(){
            FileStream palfile, binfile; // Permet de faire le fichier
            BinaryWriter palwriter; 
            string tempstring, binstring;
            byte[] tempbyte;
            

            for (uint i = 0; i < NPalettes; i++)
            { // pour chaque palette désirée
                tempstring = Palette[i].Name + ".pal.c";
                binstring = "bin/" + Palette[i].Name + "_Pal.bin";
                string namestring = "#include " + '"' + tempstring + '"' + "\r\n";
                cwriter.Write(Encoding.UTF8.GetBytes(namestring));
                hwriter.Write(Encoding.UTF8.GetBytes("extern const unsigned short " + Palette[i].Name + "_Pal[" + Palette[i].NColors + "] __attribute__ ((aligned (4))) ;\r\n"));

                palfile = new FileStream(tempstring, FileMode.Create); // Créé le fichier
                palwriter = new BinaryWriter(palfile);  // Permet d'écrire dedans
                binfile = new FileStream(binstring, FileMode.Create); // Créé le fichier
                binWriter = new BinaryWriter(binfile);  // Permet d'écrire dedans

                palwriter.Write(Encoding.UTF8.GetBytes("//Palette created using Mollusk's PAGfxConverter\r\n\r\n"));

                tempstring = "const unsigned short " + Palette[i].Name + "_Pal[" + Palette[i].NColors + "] __attribute__ ((aligned (4))) = {\r\n";
                PAGfxWrite("  " + Palette[i].Name + "_Pal, " + Palette[i].NColors + " colors\r\n");

                for (int j = 0; j < Palette[i].NColors; j++) // On ajoute les couleurs...
                {
                    tempstring = tempstring + Palette[i].Colors[j];
                    binWriter.Write((short)(Palette[i].Colors[j] & 65535));
                    if (j < Palette[i].NColors - 1) tempstring = tempstring + ", ";
                }
                palwriter.Write(Encoding.UTF8.GetBytes(tempstring));

                if ((Palette[i].NColors % 2) != 0) binWriter.Write((short)0);

                // palwriter.Write(" ");
                tempbyte = Encoding.UTF8.GetBytes("};\r\n");
                palwriter.Write(tempbyte);
                palwriter.Close();
                palfile.Close();
                binfile.Close();
                binWriter.Close();
            }
        }

        static void StringInCH(string type)
        {
            cwriter.Write(Encoding.UTF8.GetBytes("\r\n// "+type+" files : \r\n"));
            hwriter.Write(Encoding.UTF8.GetBytes("\r\n// "+type+" files : \r\n"));
        }

        static public void PAGfxWrite(string temp) // Writes the Log and on screen
        {
            Console.Write(temp);
            LogWriter.Write(Encoding.UTF8.GetBytes(temp));
        }

        static public void PAGfxWriteLine(string temp) // Writes the Log and on screen
        {
            Console.WriteLine(temp);
            LogWriter.Write(Encoding.UTF8.GetBytes(temp + "\r\n"));
        }

        #endregion

        static void Main(string[] args)
        {
            DateTime Time = DateTime.Now;
            
            string FileName = "PAGfx.ini";
            if (args.Length > 0)
                FileName = args[0].Trim();

            Log = new FileStream("PAGfx.log", FileMode.Create); // Créé le fichier
            LogWriter = new BinaryWriter(Log);  // Permet d'écrire dedans

            PAGfxWrite("PAGfx Converter, by Mollusk     www.palib.com\r\n");
            PAGfxWrite("If you have suggestions, problems, or anything, please mail me at mollusk@palib.info\r\n\n");

            PAGfxWrite(string.Format("Converting {0}\r\n", FileName));
          //  Console.ReadKey();
            string res = LoadINI.IniParse(FileName);
          //  Console.ReadKey();
            if (res != "")
            {
                PAGfxWrite("\n -> " + res + "\r\n\nPress any key to end the program");
                Console.ReadKey();
                return;
            }

            KeepPal = 0; // optimize palettes by default

            Palette = new PaletteStruct[10000]; // Memory alloc for palettes

            InitAllFileCH(); // Inits the all_gfx.c and .h files

            PAGfxWrite("Transparent Color : " + LoadINI.TranspColor.Name + "\r\n");

            //Create Bin directory
            DirectoryInfo directory = new DirectoryInfo("bin");
            directory.Create();

            if (LoadINI.NSprites > 0)
            {
                StringInCH("Sprite");
                PAGfxWriteLine("\r\n" + LoadINI.NSprites + " sprites :");
                for (int i = 0; i < LoadINI.NSprites; i++) CreateSprite(i); // Create all the sprites !
            }

            if (LoadINI.NBackgrounds > 0)
            {
                TilesInfo = new UInt32[4, 256, 100000];
                //TilesSort = new TilesSortInfo[4, 1000000];
                //TilesSort = new TilesSortInfo[4, 1000000];
                // FirstTile = new int[4, 256];
                // CurrentTilePos = new int[4];

                NTilesInfo = new UInt32[4, 256];
                StringInCH("Background");
                PAGfxWriteLine("\r\n" + LoadINI.NBackgrounds + " backgrounds :");
                for (int i = 0; i < LoadINI.NBackgrounds; i++) CreateBg(i); // Create all the backgrouds !
            }
            if (LoadINI.NTextures > 0)
            {
                StringInCH("Sprite");
                PAGfxWriteLine("\r\n" + LoadINI.NTextures + " textures :");
                for (int i = 0; i < LoadINI.NTextures; i++) CreateTexture(i); // Create all the textures !
            }   
         
            StringInCH("Palette");
            PAGfxWriteLine("\r\n"+NPalettes + " palettes :");
            WritePalettes(); // On écrit les palettes

            hwriter.Write(Encoding.UTF8.GetBytes("\r\n\r\n#endif\r\n\r\n"));
            cwriter.Write(Encoding.UTF8.GetBytes("\r\n" + BgPointers + "\r\n\r\n#ifdef __cplusplus\r\n}\r\n#endif\r\n\r\n"));


            DateTime Time2 = DateTime.Now;
            TimeSpan TotalTime = Time2.Subtract(Time);
            PAGfxWriteLine("\r\nConverted in " + TotalTime.Minutes + " minutes " + TotalTime.Seconds +" seconds");

            PAGfxWriteLine("\r\nFinished !");
        }
    }
}
