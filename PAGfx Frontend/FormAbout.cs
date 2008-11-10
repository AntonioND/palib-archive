using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PAGC_Frontend
{
    public partial class FormAbout : Form
    {
        public FormAbout()
        {
            InitializeComponent();
        }


        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.palib.com");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }

    }
}