namespace Braille_typewriter_frontend
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            print_btn = new Button();
            userPaylaod = new TextBox();
            portNameDropDown = new ComboBox();
            Port_label = new Label();
            baudRate_label = new Label();
            baudRateValue = new TextBox();
            SerialCommunication = new TextBox();
            apply = new Button();
            SuspendLayout();
            // 
            // print_btn
            // 
            print_btn.Location = new Point(617, 5);
            print_btn.Name = "print_btn";
            print_btn.Size = new Size(179, 111);
            print_btn.TabIndex = 0;
            print_btn.Text = "Tisk";
            print_btn.UseVisualStyleBackColor = true;
            print_btn.Click += button1_Click;
            // 
            // userPaylaod
            // 
            userPaylaod.Location = new Point(6, 5);
            userPaylaod.Multiline = true;
            userPaylaod.Name = "userPaylaod";
            userPaylaod.Size = new Size(605, 111);
            userPaylaod.TabIndex = 1;
            userPaylaod.TextChanged += userPaylaod_TextChanged;
            // 
            // portNameDropDown
            // 
            portNameDropDown.FormattingEnabled = true;
            portNameDropDown.Location = new Point(675, 122);
            portNameDropDown.Name = "portNameDropDown";
            portNameDropDown.Size = new Size(121, 23);
            portNameDropDown.TabIndex = 2;
            portNameDropDown.SelectedIndexChanged += portNameDropDown_SelectedIndexChanged;
            portNameDropDown.Click += portNameDropDown_Click;
            // 
            // Port_label
            // 
            Port_label.AutoSize = true;
            Port_label.Location = new Point(631, 125);
            Port_label.Name = "Port_label";
            Port_label.Size = new Size(32, 15);
            Port_label.TabIndex = 3;
            Port_label.Text = "Port:";
            // 
            // baudRate_label
            // 
            baudRate_label.AutoSize = true;
            baudRate_label.Location = new Point(631, 160);
            baudRate_label.Name = "baudRate_label";
            baudRate_label.Size = new Size(63, 15);
            baudRate_label.TabIndex = 4;
            baudRate_label.Text = "Baud Rate:";
            // 
            // baudRateValue
            // 
            baudRateValue.Location = new Point(706, 156);
            baudRateValue.Name = "baudRateValue";
            baudRateValue.Size = new Size(90, 23);
            baudRateValue.TabIndex = 5;
            baudRateValue.Text = "9600";
            baudRateValue.TextChanged += baudRateValue_TextChanged;
            // 
            // SerialCommunication
            // 
            SerialCommunication.Location = new Point(6, 122);
            SerialCommunication.Multiline = true;
            SerialCommunication.Name = "SerialCommunication";
            SerialCommunication.ReadOnly = true;
            SerialCommunication.Size = new Size(605, 316);
            SerialCommunication.TabIndex = 6;
            // 
            // apply
            // 
            apply.Location = new Point(713, 185);
            apply.Name = "apply";
            apply.Size = new Size(75, 23);
            apply.TabIndex = 7;
            apply.Text = "apply";
            apply.UseVisualStyleBackColor = true;
            apply.Click += apply_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(apply);
            Controls.Add(SerialCommunication);
            Controls.Add(baudRateValue);
            Controls.Add(baudRate_label);
            Controls.Add(Port_label);
            Controls.Add(portNameDropDown);
            Controls.Add(userPaylaod);
            Controls.Add(print_btn);
            Name = "Form1";
            Text = "Form1";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button print_btn;
        private TextBox userPaylaod;
        private ComboBox portNameDropDown;
        private Label Port_label;
        private Label baudRate_label;
        private TextBox baudRateValue;
        private TextBox SerialCommunication;
        private Button apply;
    }
}