namespace wifi传输
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
            this.wavetxtbox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.GetDataBut = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.trackBar3 = new System.Windows.Forms.TrackBar();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.编号 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ID码 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.IP地址 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.设备类型 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.扫描 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // wavetxtbox
            // 
            this.wavetxtbox.Location = new System.Drawing.Point(56, 6);
            this.wavetxtbox.Name = "wavetxtbox";
            this.wavetxtbox.Size = new System.Drawing.Size(100, 21);
            this.wavetxtbox.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "超声波:";
            // 
            // GetDataBut
            // 
            this.GetDataBut.Location = new System.Drawing.Point(5, 33);
            this.GetDataBut.Name = "GetDataBut";
            this.GetDataBut.Size = new System.Drawing.Size(75, 23);
            this.GetDataBut.TabIndex = 2;
            this.GetDataBut.Text = "获取数据";
            this.GetDataBut.UseVisualStyleBackColor = true;
            this.GetDataBut.Click += new System.EventHandler(this.GetDataBut_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(292, 6);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(42, 21);
            this.textBox1.TabIndex = 4;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(292, 33);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(42, 21);
            this.textBox2.TabIndex = 5;
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(292, 60);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(42, 21);
            this.textBox3.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(171, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 12);
            this.label2.TabIndex = 7;
            this.label2.Text = "P:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(171, 33);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(17, 12);
            this.label3.TabIndex = 8;
            this.label3.Text = "I:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(171, 60);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(17, 12);
            this.label4.TabIndex = 9;
            this.label4.Text = "D:";
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(185, 6);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(101, 45);
            this.trackBar1.TabIndex = 10;
            this.trackBar1.Value = 8;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // trackBar2
            // 
            this.trackBar2.Location = new System.Drawing.Point(185, 33);
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Size = new System.Drawing.Size(101, 45);
            this.trackBar2.TabIndex = 11;
            this.trackBar2.Value = 3;
            this.trackBar2.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // trackBar3
            // 
            this.trackBar3.Location = new System.Drawing.Point(185, 60);
            this.trackBar3.Name = "trackBar3";
            this.trackBar3.Size = new System.Drawing.Size(101, 45);
            this.trackBar3.TabIndex = 12;
            this.trackBar3.Value = 1;
            this.trackBar3.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.编号,
            this.ID码,
            this.IP地址,
            this.设备类型});
            this.dataGridView1.Location = new System.Drawing.Point(5, 106);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(327, 150);
            this.dataGridView1.TabIndex = 13;
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
            // 扫描
            // 
            this.扫描.Location = new System.Drawing.Point(5, 77);
            this.扫描.Name = "扫描";
            this.扫描.Size = new System.Drawing.Size(75, 23);
            this.扫描.TabIndex = 14;
            this.扫描.Text = "扫描设备";
            this.扫描.UseVisualStyleBackColor = true;
            this.扫描.Click += new System.EventHandler(this.扫描_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(338, 261);
            this.Controls.Add(this.扫描);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.trackBar3);
            this.Controls.Add(this.trackBar2);
            this.Controls.Add(this.trackBar1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.GetDataBut);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.wavetxtbox);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox wavetxtbox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button GetDataBut;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.TrackBar trackBar2;
        private System.Windows.Forms.TrackBar trackBar3;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button 扫描;
        private System.Windows.Forms.DataGridViewTextBoxColumn 编号;
        private System.Windows.Forms.DataGridViewTextBoxColumn ID码;
        private System.Windows.Forms.DataGridViewTextBoxColumn IP地址;
        private System.Windows.Forms.DataGridViewTextBoxColumn 设备类型;
    }
}

