namespace PixelArtTool {
    partial class Main {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.lblResult = new System.Windows.Forms.Label();
            this.tbxPath = new System.Windows.Forms.TextBox();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.ofdOpen = new System.Windows.Forms.OpenFileDialog();
            this.lblSource = new System.Windows.Forms.Label();
            this.btnPaste = new System.Windows.Forms.Button();
            this.IPBox = new System.Windows.Forms.TextBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.numSize = new System.Windows.Forms.NumericUpDown();
            this.btnClose = new System.Windows.Forms.Button();
            this.dgvColors = new System.Windows.Forms.DataGridView();
            this.ColorColumn = new System.Windows.Forms.DataGridViewImageColumn();
            this.RGBColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.BlockColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.DataColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.btnSet = new System.Windows.Forms.Button();
            this.tbxID = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tbxData = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.btnAdd = new System.Windows.Forms.Button();
            this.cdChooseColor = new System.Windows.Forms.ColorDialog();
            this.pnlColor = new System.Windows.Forms.Panel();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnLoad = new System.Windows.Forms.Button();
            this.sfdSave = new System.Windows.Forms.SaveFileDialog();
            this.imgResult = new PixelArtTool.Controls.InterpolatedPictureBox();
            this.imgSource = new PixelArtTool.Controls.InterpolatedPictureBox();
            this.ofdLoad = new System.Windows.Forms.OpenFileDialog();
            this.btnClear = new System.Windows.Forms.Button();
            this.tbxR = new System.Windows.Forms.TextBox();
            this.tbxG = new System.Windows.Forms.TextBox();
            this.tbxB = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.numSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvColors)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.imgResult)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.imgSource)).BeginInit();
            this.SuspendLayout();
            // 
            // lblResult
            // 
            this.lblResult.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lblResult.AutoSize = true;
            this.lblResult.Location = new System.Drawing.Point(283, 37);
            this.lblResult.Name = "lblResult";
            this.lblResult.Size = new System.Drawing.Size(40, 13);
            this.lblResult.TabIndex = 1;
            this.lblResult.Text = "Result:";
            // 
            // tbxPath
            // 
            this.tbxPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbxPath.Location = new System.Drawing.Point(12, 12);
            this.tbxPath.Name = "tbxPath";
            this.tbxPath.Size = new System.Drawing.Size(400, 20);
            this.tbxPath.TabIndex = 2;
            // 
            // btnBrowse
            // 
            this.btnBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBrowse.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnBrowse.Location = new System.Drawing.Point(418, 11);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(30, 23);
            this.btnBrowse.TabIndex = 3;
            this.btnBrowse.Text = "...";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.LoadImage);
            // 
            // ofdOpen
            // 
            this.ofdOpen.Filter = "PNG files|*.png";
            this.ofdOpen.Title = "Select an image...";
            // 
            // lblSource
            // 
            this.lblSource.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lblSource.AutoSize = true;
            this.lblSource.Location = new System.Drawing.Point(137, 37);
            this.lblSource.Name = "lblSource";
            this.lblSource.Size = new System.Drawing.Size(44, 13);
            this.lblSource.TabIndex = 1;
            this.lblSource.Text = "Source:";
            // 
            // btnPaste
            // 
            this.btnPaste.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.btnPaste.Enabled = false;
            this.btnPaste.Location = new System.Drawing.Point(12, 449);
            this.btnPaste.Name = "btnPaste";
            this.btnPaste.Size = new System.Drawing.Size(437, 23);
            this.btnPaste.TabIndex = 4;
            this.btnPaste.Text = "Paste";
            this.btnPaste.UseVisualStyleBackColor = true;
            this.btnPaste.Click += new System.EventHandler(this.ConvertImage);
            // 
            // IPBox
            // 
            this.IPBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.IPBox.Location = new System.Drawing.Point(35, 479);
            this.IPBox.Name = "IPBox";
            this.IPBox.PasswordChar = '•';
            this.IPBox.Size = new System.Drawing.Size(316, 20);
            this.IPBox.TabIndex = 2;
            // 
            // btnConnect
            // 
            this.btnConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnConnect.Location = new System.Drawing.Point(357, 478);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(91, 22);
            this.btnConnect.TabIndex = 5;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.Connect);
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 482);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(20, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "IP:";
            // 
            // numSize
            // 
            this.numSize.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.numSize.Location = new System.Drawing.Point(373, 161);
            this.numSize.Maximum = new decimal(new int[] {
            128,
            0,
            0,
            0});
            this.numSize.Name = "numSize";
            this.numSize.Size = new System.Drawing.Size(68, 20);
            this.numSize.TabIndex = 8;
            this.numSize.Value = new decimal(new int[] {
            128,
            0,
            0,
            0});
            // 
            // btnClose
            // 
            this.btnClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClose.Enabled = false;
            this.btnClose.Location = new System.Drawing.Point(418, 40);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(30, 23);
            this.btnClose.TabIndex = 9;
            this.btnClose.Text = "X";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.CloseImage);
            // 
            // dgvColors
            // 
            this.dgvColors.AllowUserToAddRows = false;
            this.dgvColors.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvColors.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvColors.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ColorColumn,
            this.RGBColumn,
            this.BlockColumn,
            this.DataColumn});
            this.dgvColors.Location = new System.Drawing.Point(12, 213);
            this.dgvColors.Name = "dgvColors";
            this.dgvColors.Size = new System.Drawing.Size(437, 204);
            this.dgvColors.TabIndex = 11;
            // 
            // ColorColumn
            // 
            this.ColorColumn.FillWeight = 30F;
            this.ColorColumn.HeaderText = "";
            this.ColorColumn.Name = "ColorColumn";
            this.ColorColumn.ReadOnly = true;
            this.ColorColumn.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.ColorColumn.Width = 30;
            // 
            // RGBColumn
            // 
            this.RGBColumn.HeaderText = "RGB";
            this.RGBColumn.Name = "RGBColumn";
            this.RGBColumn.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // BlockColumn
            // 
            this.BlockColumn.HeaderText = "Block ID";
            this.BlockColumn.Name = "BlockColumn";
            this.BlockColumn.ReadOnly = true;
            this.BlockColumn.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // DataColumn
            // 
            this.DataColumn.HeaderText = "Block Data ID";
            this.DataColumn.Name = "DataColumn";
            this.DataColumn.ReadOnly = true;
            this.DataColumn.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // btnSet
            // 
            this.btnSet.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnSet.Location = new System.Drawing.Point(37, 420);
            this.btnSet.Name = "btnSet";
            this.btnSet.Size = new System.Drawing.Size(33, 24);
            this.btnSet.TabIndex = 13;
            this.btnSet.Text = "Set";
            this.btnSet.UseVisualStyleBackColor = true;
            this.btnSet.Click += new System.EventHandler(this.SetColor);
            // 
            // tbxID
            // 
            this.tbxID.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbxID.Location = new System.Drawing.Point(249, 423);
            this.tbxID.MaxLength = 3;
            this.tbxID.Name = "tbxID";
            this.tbxID.Size = new System.Drawing.Size(30, 20);
            this.tbxID.TabIndex = 14;
            this.tbxID.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(192, 426);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(51, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Block ID:";
            // 
            // tbxData
            // 
            this.tbxData.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbxData.Location = new System.Drawing.Point(362, 423);
            this.tbxData.MaxLength = 2;
            this.tbxData.Name = "tbxData";
            this.tbxData.Size = new System.Drawing.Size(25, 20);
            this.tbxData.TabIndex = 14;
            this.tbxData.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(293, 426);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(63, 13);
            this.label5.TabIndex = 15;
            this.label5.Text = "Block Data:";
            // 
            // btnAdd
            // 
            this.btnAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnAdd.Location = new System.Drawing.Point(393, 421);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(56, 23);
            this.btnAdd.TabIndex = 16;
            this.btnAdd.Text = "Add";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.AddEntry);
            // 
            // pnlColor
            // 
            this.pnlColor.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.pnlColor.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.pnlColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pnlColor.Location = new System.Drawing.Point(12, 421);
            this.pnlColor.Name = "pnlColor";
            this.pnlColor.Size = new System.Drawing.Size(22, 22);
            this.pnlColor.TabIndex = 18;
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(12, 187);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(117, 23);
            this.btnSave.TabIndex = 19;
            this.btnSave.Text = "Save Color Config";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.SaveColorConfiguration);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(332, 187);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(117, 23);
            this.btnLoad.TabIndex = 19;
            this.btnLoad.Text = "Load Color Config";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.LoadColorConfiguration);
            // 
            // sfdSave
            // 
            this.sfdSave.Filter = "Color Configurations|*.pxl";
            this.sfdSave.Title = "Where do you want to save your color configuration?";
            // 
            // imgResult
            // 
            this.imgResult.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.imgResult.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            this.imgResult.Location = new System.Drawing.Point(239, 53);
            this.imgResult.Name = "imgResult";
            this.imgResult.Size = new System.Drawing.Size(128, 128);
            this.imgResult.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.imgResult.TabIndex = 10;
            this.imgResult.TabStop = false;
            // 
            // imgSource
            // 
            this.imgSource.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.imgSource.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            this.imgSource.Location = new System.Drawing.Point(94, 53);
            this.imgSource.Name = "imgSource";
            this.imgSource.Size = new System.Drawing.Size(128, 128);
            this.imgSource.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.imgSource.TabIndex = 10;
            this.imgSource.TabStop = false;
            // 
            // ofdLoad
            // 
            this.ofdLoad.Filter = "Color Configurations|*.pxl";
            this.ofdLoad.Title = "Where is your color configuration located?";
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(172, 187);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(117, 23);
            this.btnClear.TabIndex = 19;
            this.btnClear.Text = "Clear All Colors";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.ClearColorConfiguration);
            // 
            // tbxR
            // 
            this.tbxR.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbxR.Location = new System.Drawing.Point(82, 423);
            this.tbxR.MaxLength = 3;
            this.tbxR.Name = "tbxR";
            this.tbxR.Size = new System.Drawing.Size(30, 20);
            this.tbxR.TabIndex = 14;
            this.tbxR.Text = "255";
            this.tbxR.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbxR.TextChanged += new System.EventHandler(this.RGBManuallySet);
            // 
            // tbxG
            // 
            this.tbxG.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbxG.Location = new System.Drawing.Point(118, 423);
            this.tbxG.MaxLength = 3;
            this.tbxG.Name = "tbxG";
            this.tbxG.Size = new System.Drawing.Size(30, 20);
            this.tbxG.TabIndex = 14;
            this.tbxG.Text = "255";
            this.tbxG.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbxG.TextChanged += new System.EventHandler(this.RGBManuallySet);
            // 
            // tbxB
            // 
            this.tbxB.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbxB.Location = new System.Drawing.Point(154, 423);
            this.tbxB.MaxLength = 3;
            this.tbxB.Name = "tbxB";
            this.tbxB.Size = new System.Drawing.Size(30, 20);
            this.tbxB.TabIndex = 14;
            this.tbxB.Text = "255";
            this.tbxB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.tbxB.TextChanged += new System.EventHandler(this.RGBManuallySet);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(461, 508);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.pnlColor);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tbxData);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.tbxB);
            this.Controls.Add(this.tbxG);
            this.Controls.Add(this.tbxR);
            this.Controls.Add(this.tbxID);
            this.Controls.Add(this.btnSet);
            this.Controls.Add(this.dgvColors);
            this.Controls.Add(this.imgResult);
            this.Controls.Add(this.imgSource);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.numSize);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.btnPaste);
            this.Controls.Add(this.btnBrowse);
            this.Controls.Add(this.IPBox);
            this.Controls.Add(this.tbxPath);
            this.Controls.Add(this.lblSource);
            this.Controls.Add(this.lblResult);
            this.Name = "Main";
            this.Text = "Minecraft Wii U Pixel Art Tool";
            this.Load += new System.EventHandler(this.FormLoad);
            ((System.ComponentModel.ISupportInitialize)(this.numSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvColors)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.imgResult)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.imgSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label lblResult;
        private System.Windows.Forms.TextBox tbxPath;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.OpenFileDialog ofdOpen;
        private System.Windows.Forms.Label lblSource;
        private System.Windows.Forms.Button btnPaste;
        private System.Windows.Forms.TextBox IPBox;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numSize;
        private System.Windows.Forms.Button btnClose;
        private PixelArtTool.Controls.InterpolatedPictureBox imgSource;
        private PixelArtTool.Controls.InterpolatedPictureBox imgResult;
        private System.Windows.Forms.DataGridView dgvColors;
        private System.Windows.Forms.Button btnSet;
        private System.Windows.Forms.TextBox tbxID;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tbxData;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.ColorDialog cdChooseColor;
        private System.Windows.Forms.Panel pnlColor;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.SaveFileDialog sfdSave;
        private System.Windows.Forms.OpenFileDialog ofdLoad;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.DataGridViewImageColumn ColorColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn RGBColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn BlockColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn DataColumn;
        private System.Windows.Forms.TextBox tbxR;
        private System.Windows.Forms.TextBox tbxG;
        private System.Windows.Forms.TextBox tbxB;
    }
}

