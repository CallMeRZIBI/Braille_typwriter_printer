namespace Braille_typewriter_frontend
{
    partial class Popup
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            popupLabel = new Label();
            SuspendLayout();
            // 
            // popupLabel
            // 
            popupLabel.AutoSize = true;
            popupLabel.Location = new Point(26, 9);
            popupLabel.Name = "popupLabel";
            popupLabel.Size = new Size(49, 15);
            popupLabel.TabIndex = 0;
            popupLabel.Text = "dummy";
            // 
            // Popup
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(394, 206);
            Controls.Add(popupLabel);
            Name = "Popup";
            Text = "Popup";
            Load += Popup_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label popupLabel;
    }
}