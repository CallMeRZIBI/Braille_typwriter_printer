using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Braille_typewriter_frontend
{
    public partial class Popup : Form
    {
        public Popup()
        {
            InitializeComponent();
        }

        private void Popup_Load(object sender, EventArgs e)
        {
        }

        public void DisplayText(string text)
        {
            popupLabel.Text = text;
        }
    }
}
