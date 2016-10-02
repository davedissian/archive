using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace RubiksCube
{
    public partial class frmAbout : Form
    {
        public frmAbout()
        {
            InitializeComponent();
            aboutTitle.Font = new Font(aboutTitle.Font.FontFamily.Name, 16);
        }
    }
}
