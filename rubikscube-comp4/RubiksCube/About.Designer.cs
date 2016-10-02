namespace RubiksCube
{
    partial class frmAbout
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
            this.aboutInfo = new System.Windows.Forms.Label();
            this.aboutTitle = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // aboutInfo
            // 
            this.aboutInfo.AutoSize = true;
            this.aboutInfo.Location = new System.Drawing.Point(12, 55);
            this.aboutInfo.Name = "aboutInfo";
            this.aboutInfo.Size = new System.Drawing.Size(194, 26);
            this.aboutInfo.TabIndex = 0;
            this.aboutInfo.Text = "COMP4 Project\r\nWritten by David Avedissian 2012-2013\r\nEmail: avedissian.david@gmail.com\r\n";
            // 
            // aboutTitle
            // 
            this.aboutTitle.AutoSize = true;
            this.aboutTitle.Location = new System.Drawing.Point(12, 9);
            this.aboutTitle.Name = "aboutTitle";
            this.aboutTitle.Size = new System.Drawing.Size(70, 13);
            this.aboutTitle.TabIndex = 1;
            this.aboutTitle.Text = "Rubik\'s Cube";
            // 
            // frmAbout
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(229, 108);
            this.Controls.Add(this.aboutTitle);
            this.Controls.Add(this.aboutInfo);
            this.Name = "frmAbout";
            this.Text = "About";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label aboutInfo;
        private System.Windows.Forms.Label aboutTitle;
    }
}