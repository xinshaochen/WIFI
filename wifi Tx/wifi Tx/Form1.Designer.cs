namespace wifi_Tx
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.扫描 = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.编号 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ID码 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.IP地址 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.设备类型 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.getDataBut = new System.Windows.Forms.Button();
            this.ch4Adcval = new System.Windows.Forms.TextBox();
            this.ch3Adcval = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // 扫描
            // 
            this.扫描.Location = new System.Drawing.Point(5, 158);
            this.扫描.Name = "扫描";
            this.扫描.Size = new System.Drawing.Size(75, 23);
            this.扫描.TabIndex = 16;
            this.扫描.Text = "扫描设备";
            this.扫描.UseVisualStyleBackColor = true;
            this.扫描.Click += new System.EventHandler(this.扫描_Click);
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.编号,
            this.ID码,
            this.IP地址,
            this.设备类型});
            this.dataGridView1.Location = new System.Drawing.Point(5, 2);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(327, 150);
            this.dataGridView1.TabIndex = 15;
            this.dataGridView1.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellContentClick);
            // 
            // 编号
            // 
            this.编号.HeaderText = "编号";
            this.编号.Name = "编号";
            this.编号.Width = 30;
            // 
            // ID码
            // 
            this.ID码.HeaderText = "ID码";
            this.ID码.Name = "ID码";
            this.ID码.Width = 80;
            // 
            // IP地址
            // 
            this.IP地址.HeaderText = "IP地址";
            this.IP地址.Name = "IP地址";
            this.IP地址.Width = 90;
            // 
            // 设备类型
            // 
            this.设备类型.HeaderText = "设备类型";
            this.设备类型.Name = "设备类型";
            this.设备类型.Width = 90;
            // 
            // getDataBut
            // 
            this.getDataBut.Location = new System.Drawing.Point(338, 2);
            this.getDataBut.Name = "getDataBut";
            this.getDataBut.Size = new System.Drawing.Size(75, 23);
            this.getDataBut.TabIndex = 17;
            this.getDataBut.Text = "获取数据";
            this.getDataBut.UseVisualStyleBackColor = true;
            this.getDataBut.Click += new System.EventHandler(this.getDataBut_Click);
            // 
            // ch4Adcval
            // 
            this.ch4Adcval.Location = new System.Drawing.Point(454, 31);
            this.ch4Adcval.Name = "ch4Adcval";
            this.ch4Adcval.Size = new System.Drawing.Size(100, 21);
            this.ch4Adcval.TabIndex = 18;
            // 
            // ch3Adcval
            // 
            this.ch3Adcval.Location = new System.Drawing.Point(454, 4);
            this.ch3Adcval.Name = "ch3Adcval";
            this.ch3Adcval.Size = new System.Drawing.Size(100, 21);
            this.ch3Adcval.TabIndex = 18;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(419, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 19;
            this.label1.Text = "CH3:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(419, 34);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 19;
            this.label2.Text = "CH4:";
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(347, 58);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(200, 200);
            this.pictureBox1.TabIndex = 21;
            this.pictureBox1.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(559, 262);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ch3Adcval);
            this.Controls.Add(this.ch4Adcval);
            this.Controls.Add(this.getDataBut);
            this.Controls.Add(this.扫描);
            this.Controls.Add(this.dataGridView1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button 扫描;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn 编号;
        private System.Windows.Forms.DataGridViewTextBoxColumn ID码;
        private System.Windows.Forms.DataGridViewTextBoxColumn IP地址;
        private System.Windows.Forms.DataGridViewTextBoxColumn 设备类型;
        private System.Windows.Forms.Button getDataBut;
        private System.Windows.Forms.TextBox ch4Adcval;
        private System.Windows.Forms.TextBox ch3Adcval;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}

