using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.IO;
using System.Windows.Forms;
using PAGfx.Common;

/*
 \¯¯¯¯\/¯¯¯|¯¯¯¯/¯¯¯¯/¯\/¯/   |¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
 | |_\ |/_||/¯¯´| |¯  \  /    | PAGfxConverter v0.8     |
 |  __/ _  |\_/¯/  ¯/ /  \    | By: Team PAlib          |
 |_/ |_/ |_|___/|_|¯ /_/\_\   | Last Modified: 05.03.06 |
                   Frontend   |_________________________|
*/

namespace PAGC_Frontend
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //
        }

        #region Toolbar Code

        //Button 1: Add item
        private void toolStripButton1_Click(object sender, EventArgs e) 
        {
            openFileDialog1.ShowDialog();
        }

        //Button 2: Remove item
        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            if (Get_Current_Grid().SelectedRows.Count == 1)
                Get_Current_Grid().Rows.Remove(Get_Current_Grid().SelectedRows[0]);
            //else
            //    Beep;
        }

        //Button 3 : Load INI
        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            Grid_LoadINI();
        }

        //Button 4: Save INI
        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            Grid_SaveINI();
        }

        //Button 5 : Save and Convert
        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            Grid_SaveINIConvert();
        }

        //Button 6: About
        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            Form frmAbout = new FormAbout();
            frmAbout.ShowDialog();
        }

        //Button 7: Update Row
        private void toolStripButton7_Click(object sender, EventArgs e)
        {
            if (Get_Current_Grid().SelectedRows.Count == 1)
                Grid_SaveINIConvertRow();
            //else
            //    Beep;
        }

        #endregion

        #region Grid routines (Grid_)

        // Add items
        private void Grid_AddFile(string File)
        {
            // Todo: Add some sort of extension check here!

            string[] SplitPath = File.Split(new char[] { '\\' });
            string ShortName = SplitPath[SplitPath.GetLength(0) - 1];

            switch (TabControl1.SelectedTab.Text)
            {
                case "Sprites":
                    Grid_Sprite.Rows.Add(ShortName, "256colors", LoadINI.ExtractName(ShortName), File);
                    break;
                case "Backgrounds":
                    Grid_BG.Rows.Add(ShortName, "TileBg", LoadINI.ExtractName(ShortName), File);
                    break;
                case "Textures":
                    Grid_Tex.Rows.Add(ShortName, "8bit", LoadINI.ExtractName(ShortName), File);
                    break;
            }
            DataGridView Grid = Get_Current_Grid();
            Grid.Rows[Grid.Rows.Count - 1].Selected = true;
            UpdatePreview(Grid);
            CheckImageDepth(Grid);
        }

        // Global grid change event
        private void Grid_CellValueChanged(object Grid, DataGridViewCellEventArgs e)
        {
            CheckImageDepth((DataGridView)Grid);
        }

        private void CheckImageDepth(DataGridView Grid)
        {
            if (Grid.Rows.Count == 0)
                return;
            switch (Grid.SelectedRows[0].Cells[1].Value.ToString())
            {
                case "16colors":
                    if (pictureBox1.Image.PixelFormat == System.Drawing.Imaging.PixelFormat.Format4bppIndexed)
                        MessageBox.Show("WARNING: Not a 16 colors image!");
                    break;
                case "16bit":
                    if (pictureBox1.Image.Palette.Entries.Length > 0)
                        MessageBox.Show("WARNING: Not a true color image!");
                    break;
                default:
                    if (pictureBox1.Image.PixelFormat != System.Drawing.Imaging.PixelFormat.Format8bppIndexed)
                        MessageBox.Show("WARNING: Not a 256 colors image!");
                    break;
            }
        }

        // Global grid click event
        private void Grid_RowChanged(object Grid, EventArgs e)
        {
            UpdatePreview((DataGridView)Grid);
        }

        private void UpdatePreview(DataGridView Grid)
        {
            if (Grid.SelectedRows.Count == 0)
            {
                pictureBox1.Image = null;
                lblPInfo.Text = "";
                return;
            }
            try
            {
                pictureBox1.Image = new Bitmap(Grid.SelectedRows[0].Cells[3].Value.ToString());
                lblPInfo.Text = "Info: " + pictureBox1.Image.Width.ToString() + "x" + pictureBox1.Image.Height.ToString();
                if (pictureBox1.Image.Palette.Entries.Length > 0)
                    lblPInfo.Text += " - " + pictureBox1.Image.Palette.Entries.Length.ToString() + " colors";
                else
                    lblPInfo.Text += " - full color";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
            }  
        }

        //Get the current grid
        private DataGridView Get_Current_Grid()
        {
            switch (TabControl1.SelectedTab.Text)
            {
                case "Sprites":
                    return Grid_Sprite;
                case "Backgrounds":
                    return Grid_BG;
                case "Textures":
                    return Grid_Tex;
            }
            return null;
        }

        //Get content for export
        private string Grid_GetContent(DataGridView Grid, bool All)
        {
            string Output = "";
            if (All)
                foreach (DataGridViewRow SelRow in Grid.Rows)
                {
                    // Add filename
                    string filename = SelRow.Cells[3].Value.ToString();
                    string colormode = SelRow.Cells[1].EditedFormattedValue.ToString();
                    string palette = (SelRow.Cells[2].Value != null) ? SelRow.Cells[2].Value.ToString() : "None";
                    // Final output
                    Output = Output + filename + " " + colormode + " " + palette + "\r\n";
                }
            else
            {
                DataGridViewRow SelRow = Grid.SelectedRows[0];
                if (SelRow != null)
                {
                    Output = SelRow.Cells[3].Value.ToString() + " " + SelRow.Cells[1].Value.ToString() + " ";
                    Output += (SelRow.Cells[2].Value != null) ? SelRow.Cells[2].Value.ToString() : "None" + "\r\n";
                }
            }
            return Output;
        }

        #endregion

        #region Drag and Drop Code
        
        private void Grid_DragCheck(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Copy;
            else
                e.Effect = DragDropEffects.None;
        }

        private void Grid_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
                foreach (string file in files)
                    Grid_AddFile(file);
            }
        }

        #endregion

        #region Save INI

        public string SaveINI(string IniName, bool All)
        {
            string Output_String = "";

            string ColorS = CmdTransCol.BackColor.ToString();
            ColorS = ColorS.Substring(7, ColorS.Length - 8);
            if (ColorS == "Lime") ColorS = "Green";

            Output_String = "#TranspColor " + ColorS + "\r\n"; 

            //Sprites
            if (All || TabControl1.SelectedTab.Text == "Sprites")
                Output_String = Output_String + "\r\n" + // Had to put \r\n as line break
                            "#Sprites :\r\n" +
                            Grid_GetContent(Grid_Sprite, All);

            //Backgrounds
            if (All || TabControl1.SelectedTab.Text == "Backgrounds")
                Output_String = Output_String + "\r\n" +
                            "#Backgrounds :\r\n" +
                            Grid_GetContent(Grid_BG, All);

            //Textures
            if (All || TabControl1.SelectedTab.Text == "Textures")
                Output_String = Output_String + "\r\n" +
                            "#Textures :\r\n" +
                            Grid_GetContent(Grid_Tex, All);

            //Save
            FileStream IniFile = null;
            BinaryWriter IniWriter = null;
            try
            {
                IniFile = new FileStream(System.AppDomain.CurrentDomain.BaseDirectory + IniName, FileMode.Create);
                IniWriter = new BinaryWriter(IniFile);
                IniWriter.Write(Encoding.ASCII.GetBytes(Output_String));
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
            }
            finally 
            {
                if (IniWriter != null) IniWriter.Close();
                if (IniFile != null) IniFile.Close();
            }

            return Output_String;
        }

        public void Grid_SaveINI()
        {
            string output = SaveINI("PAGfx.ini", true);
            MessageBox.Show(output, "Saved");
        }

        public void Grid_SaveINIConvert()
        {
            SaveINI("PAGfx.ini", true);
            System.Diagnostics.Process.Start("PAGfx.exe");
        }

        public void Grid_SaveINIConvertRow()
        {
            SaveINI("PAGfx_tmp.ini", false);
            System.Diagnostics.Process.Start("PAGfx.exe", "PAGfx_tmp.ini");
        }

        #endregion

        #region Load INI file

        public void Grid_LoadSprites()
        {
            // Load the sprites
            Grid_Sprite.Rows.Clear();
            for (int i = 0; i < LoadINI.NSprites; i++)
            {
                if (string.IsNullOrEmpty(LoadINI.IniSprites[i].PaletteName))
                    LoadINI.IniSprites[i].PaletteName = "None";
                if (File.Exists(LoadINI.IniSprites[i].Path))
                    Grid_Sprite.Rows.Add(LoadINI.IniSprites[i].Name, LoadINI.SpriteModes[LoadINI.IniSprites[i].ColorMode], LoadINI.IniSprites[i].PaletteName, LoadINI.IniSprites[i].Path);
                else
                    MessageBox.Show("File " + LoadINI.IniSprites[i].Path + " not found");
            }
        }

        public void Grid_LoadBgs()
        {
            // Load the backgrounds
            Grid_BG.Rows.Clear();
            for (int i = 0; i < LoadINI.NBackgrounds; i++)
            {
                if (string.IsNullOrEmpty(LoadINI.IniBgs[i].PaletteName))
                    LoadINI.IniBgs[i].PaletteName = "None";
                if (File.Exists(LoadINI.IniBgs[i].Path))
                    Grid_BG.Rows.Add(LoadINI.IniBgs[i].Name, LoadINI.BgModes[LoadINI.IniBgs[i].BgMode], LoadINI.IniBgs[i].PaletteName, LoadINI.IniBgs[i].Path);
                else
                    MessageBox.Show("File " + LoadINI.IniBgs[i].Path + " not found");
            }
        }

        public void Grid_LoadINI(){
            // Parses the ini file !
            string loaded = LoadINI.IniParse("PAGfx.ini");
            if (loaded == "")
                loaded = string.Format("Loaded {0} sprites and {1} backgrounds", LoadINI.NSprites, LoadINI.NBackgrounds);

            Grid_LoadTranspColor();
            Grid_LoadSprites();
            Grid_LoadBgs();
            Grid_Tex.Rows.Clear();

            UpdatePreview(Get_Current_Grid());

            MessageBox.Show(loaded, "Loaded");
        }

        void Grid_LoadTranspColor(){
            // Loads the correct transparent color...
            switch (LoadINI.TranspColor.Name)
            {
               case "White":
                   CmdTransCol.BackColor = System.Drawing.Color.White; break;
               case "Black":
                   CmdTransCol.BackColor = System.Drawing.Color.Black; break;
               case "Green":
                   CmdTransCol.BackColor = System.Drawing.Color.Lime; break;
               case "Magenta":
                   CmdTransCol.BackColor = System.Drawing.Color.Magenta; break;
               case "Fuschia":
                   CmdTransCol.BackColor = System.Drawing.Color.Magenta; break;
            }
        }

        #endregion

        // Transparant color button
        private void CmdTransCol_Click(object sender, EventArgs e)
        {
            ColorMenu.Show(CmdTransCol, new Point(10,20));
        }

        // Transparent color contextmenu...
        private void ColorMenu_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            switch (e.ClickedItem.Text)
            {
                case "White":
                    CmdTransCol.BackColor = System.Drawing.Color.White;
                    break;
                case "Black":
                    CmdTransCol.BackColor = System.Drawing.Color.Black;
                    break;
                case "Green":
                    CmdTransCol.BackColor = System.Drawing.Color.Lime;
                    break;
                case "Magenta":
                    CmdTransCol.BackColor = System.Drawing.Color.Magenta;
                    break;
                case "Fuschia": // ????
                    CmdTransCol.BackColor = System.Drawing.Color.Magenta;
                    break;
            }
        }

        //Open File dialog
        public void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            String[] files = openFileDialog1.FileNames;
            foreach (string file in files)
                Grid_AddFile(file);
        }

        private void TabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdatePreview(Get_Current_Grid());
        }
    }
}




