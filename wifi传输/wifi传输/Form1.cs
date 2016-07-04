using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace wifi传输
{
    public partial class Form1 : Form
    {
        List<IPAddress> IPAdd = new List<IPAddress>();
        List<EquiPmentData> Equi = new List<EquiPmentData>();
        int devSelect = -1;//选中的设备
        const int Port = 2333;//目标端口
        UInt16 cm;


        public Form1()
        {
            InitializeComponent();
        }


        UdpClient Search = new UdpClient(0);
        IPAddress lasttarget;
        Timer sendServing = new Timer();//发送服务

        Timer t = new Timer();

        uint RecvFlag;//接收解码成功标志位
        uint recvmode;
        UInt16 packlen;
        byte RecvCheck;
        const uint maxpacklen = 1024;
        byte[] packbuff = new byte[maxpacklen];
        uint count;
        uint recvcmd;//读到的CMD

        uint scannCount=0;//扫描次数
        int scannNum = 0;
        
        private void Form1_Load(object sender, EventArgs e)
        {

            t.Tick += new EventHandler((object s, EventArgs ex) =>
              {
                  getData(GetDev.Wave);
              });

            sendServing.Interval = 200;//超时重发
            sendServing.Tick += new EventHandler((object s, EventArgs ex) =>
            {

                if(scannBut==true)//扫描模式打开
                {
                    scannCount++;
                    lasttarget = new IPAddress(0xffffffff);
                    GetIPID();
                    lasttarget = new IPAddress(0xffffffff);
                    GetDevType();

                    if (scannCount >= 3)
                    {
                        scannBut = false;
                        扫描.Text = "扫描设备";
                        扫描.Enabled = true;
                        foreach(EquiPmentData bufEqui in Equi)
                        {
                            dataGridView1.Rows.Add(bufEqui.num, bufEqui.id, bufEqui.ip, bufEqui.type);
                        }
                    }
                }

                if (Equi.Count == 0) return;
                if (devSelect == -1) return;
                for(int i=0;i<recvarraydata.Length;i++)
                {
                    if(recvarraydata[i]!=null)
                    {
                        
                        Search.Send(recvarraydata[i], recvarraydata[i].Length, new IPEndPoint(Equi[devSelect].ip.Address, Port));
                    }
                }
                for(int i=0;i<sendarraydata.Length;i++)
                {
                    if(sendarraydata[i]!=null)
                    {
                        Search.Send(sendarraydata[i],sendarraydata[i].Length,new IPEndPoint(Equi[devSelect].ip.Address, Port));
                    }
                }
            });
            sendServing.Start();
            new Task(() =>
            {
                while (!this.IsDisposed)
                    try
                    {
                        IPEndPoint p = new IPEndPoint(IPAddress.Any, 0);
                        byte[] buff = Search.Receive(ref p);
                        lasttarget = p.Address;
                        string value = Encoding.Default.GetString(buff);
                        int len = buff.Length;
                        for(uint l=0;l< len;l++)
                        {
                            switch(recvmode)
                            {
                                case 0:
                                    if (buff[l] == 0xaa)
                                        recvmode = 1;
                                    break;
                                case 1:
                                    packlen = buff[l];
                                    recvmode = 2;
                                    break;
                                case 2:
                                    packlen <<= 8;
                                    packlen |= buff[l];
                                    RecvCheck = 0;
                                    count = 0;
                                    if (packlen > maxpacklen)
                                        recvmode = 0;
                                    else 
                                    recvmode = 3;
                                    break;
                                case 3:
                                    recvcmd = buff[l];
                                    if (packlen != 0)
                                        recvmode = 4;
                                    else
                                        recvmode = 6;
                                    break;
                                case 4:
                                    packbuff[count] = buff[l];
                                    RecvCheck += buff[l];
                                    count++;
                                    if (count >= packlen)
                                        recvmode = 5;
                                    break;
                                case 5:
                                    if(buff[l]==RecvCheck)
                                    {
                                        recvmode = 6;
                                    }else
                                    {
                                        recvmode = 0;
                                        RecvFlag = 0;
                                    }
                                    break;
                                case 6:
                                    if(buff[l]==0x55)
                                    {
                                        recvmode = 0;
                                        RecvFlag = 1;
                                    }else
                                    {
                                        recvmode = 0;
                                        RecvFlag = 0;
                                    }
                                    break;
                            }
                        }
                        if(scannBut==true)
                        {
                            int index = Equi.FindIndex(s => s.ip.Address == p.Address.Address);
                            if(index!=-1)
                            {
                                if (RecvFlag == 1)
                                {
                                    RecvFlag = 0;
                                    string s = Encoding.Default.GetString(packbuff, 0, packlen);
                                    Equi[index].type = s;
                                }
                            }
                            else
                            {
                                int pos = value.IndexOf("ID:");
                                if(pos!=-1)
                                {
                                    string str = "";
                                    for (int i = 0; i < value.Length; i++)
                                    {
                                        if (char.IsDigit(value, i))
                                        {
                                            str += value[i];
                                        }
                                    }
                                    EquiPmentData bufEqui = new EquiPmentData();
                                    bufEqui.id = str;
                                    bufEqui.ip = p.Address;
                                    bufEqui.num = scannNum++;
                                    Equi.Add(bufEqui);
                                }
                            }
                        }else
                        {
                            if(RecvFlag==1)//得到数据
                            {
                                RecvFlag = 0;
                                if((cmd)recvcmd==cmd.setdata)//确认序列号
                                {
                                    if(sendarrayid[packbuff[1]]==packbuff[0])//发送设置确认成功
                                    {
                                        sendarraydata[packbuff[1]] = null;
                                    } 
                                }else if((cmd)recvcmd==cmd.getdata)
                                {
                                    if(recvarrayid[packbuff[1]]==packbuff[0])
                                    {
                                        recvarraydata[packbuff[1]] = null;
                                        int index = Equi.FindIndex(se => se.ip.Address == p.Address.Address);
                                        if (index!=-1)
                                        {
                                            if(Equi[index].type.IndexOf("TestRx0")!=-1)
                                            {
                                                uint s = 2;
                                                cm = packbuff[s++];
                                                cm <<= 8;
                                                cm |= packbuff[s++];
                                            }
                                        }
                                    }
                                }
                                Invoke(new MethodInvoker(() =>
                                {
                                    wavetxtbox.Text = cm.ToString();
                                }));
                            }
                        }
                    }
                    catch { }
            }).Start();
        }
        

        bool bust = false;
        private void GetDataBut_Click(object sender, EventArgs e)
        {
            bust = !bust;
            if(bust==true)
            {
                GetDataBut.Text = "停止获取";
                //lasttarget = new IPAddress(0xffffffff);
                t.Interval = 200;
                t.Start();
            }
            else
            {
                GetDataBut.Text = "获取数据";
                t.Stop();
            }
        }
        /// <summary>
        /// 命令号
        /// </summary>
        public enum cmd
        {
            alive = 0,
            getdata,
            setdata,
        }
        /// <summary>
        /// 数据获取列表
        /// </summary>
        public enum GetDev
        {
            Wave = 0,
            Key,
            All,
        }
        GetDev Gdev;
        const uint maxGdevEntry = 10;
        /// <summary>
        /// 数据设置列表
        /// </summary>
        public enum SetDev
        {
            PID=0,
            P2,
            P3,
            P4,
        }
        SetDev Sdev;
        const uint maxSdevEntry = 10;
        private byte sendID;
        byte[][] sendarraydata = new byte[maxSdevEntry][];
        byte[] sendarrayid = new byte[maxSdevEntry];

        private byte recvID;
        byte[][] recvarraydata = new byte[maxGdevEntry][];
        byte[] recvarrayid = new byte[maxGdevEntry];
        /// <summary>
        /// 数据设置，带确认
        /// </summary>
        /// <param name="sdev"></param>
        /// <param name="list"></param>
        void setData(SetDev sdev,params object[] list)
        {
            List<byte> buff = new List<byte>();

            buff.Add(sendID);
            buff.Add((byte)sdev);

            foreach (object obj in list)
            {   
                Type typ = obj.GetType();
                if (typ == typeof(byte))
                {
                    buff.Add((byte)obj);
                }
                else if (typ == typeof(Int16))
                {
                    Int16 i = (Int16)obj;
                    buff.Add((byte)(i >> 8));
                    buff.Add((byte)(i));
                }
                else if (typ == typeof(int))
                {
                    int i = (int)obj;
                    buff.Add((byte)(i >> 24));//发送为大端模式
                    buff.Add((byte)(i >> 16));
                    buff.Add((byte)(i >> 8));
                    buff.Add((byte)(i));
                }
            }
            
            byte[] by = Encode(cmd.setdata, buff.ToArray());
            sendarrayid[(byte)sdev] = sendID++ ;
            sendarraydata[(byte)sdev] = by;
        }
        /// <summary>
        /// 发送获取数据命令，带确认
        /// </summary>
        /// <param name="gdev"></param>
        void getData(GetDev gdev)
        {
            byte[] buff = new byte[2];
            buff[0] = recvID;
            buff[1] = (byte)gdev;
            byte[] by = Encode(cmd.getdata,buff);
            recvarrayid[(byte)gdev] = recvID++;
            recvarraydata[(byte)gdev] = by;
        }
        

        /// <summary>
        /// 发送数据，不确认
        /// </summary>
        /// <param name="Cmd"></param>
        /// <param name="data"></param>
        void sendData(cmd Cmd, byte[] data =null)
        {
            byte[] d;
            d = Encode(Cmd, data);
            Search.Send(d, d.Length, new IPEndPoint(lasttarget.Address, Port));
        }
        /// <summary>
        /// 加码  将数据加入协议中
        /// </summary>
        /// <param name="Cmd"></param>
        /// <param name="data"></param>
        /// <returns></returns>
        byte[] Encode(cmd Cmd, byte[] data = null)
        {
            byte[] d = new byte[(data == null ? 0+5 : data.Length+6)];
            int i = 0;
            byte sum = 0;
            d[i++] = 0xaa;
            if (data != null)
            {
                d[i++] = (byte)(data.Length / 256);
                d[i++] = (byte)data.Length;
            }
            else
            {
                d[i++] = 0;
                d[i++] = 0;

            }
            d[i++] = (byte)Cmd;

            if (data != null)
            {
                foreach (byte b in data)
                {
                    sum += b;
                    d[i++] = b;
                }
                d[i++] = sum;
            }
            d[i++] = 0x55;

            return d;
        }
        //==================================================================================================================


        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            setData(SetDev.PID, (byte)trackBar1.Value, (byte)trackBar2.Value, (byte)trackBar3.Value);
            textBox1.Text = trackBar1.Value.ToString();
            textBox2.Text = trackBar2.Value.ToString();
            textBox3.Text = trackBar3.Value.ToString();
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            devSelect = dataGridView1.CurrentRow.Index;
        }
        bool scannBut = false;
        private void 扫描_Click(object sender, EventArgs e)
        {
            lasttarget = new IPAddress(0xffffffff);
            dataGridView1.Rows.Clear();
            scannBut = true;
            Equi.Clear();
            scannCount = 0;
            scannNum = 0;
            扫描.Text = "扫描中";
            扫描.Enabled = false;
            //GetIPID();
            //GetDevType();
        }

        void GetIPID()
        {
            string str = "print(\"ID:\"..node.chipid())";
            byte[] by = Encoding.Default.GetBytes(str);
            Search.Send(by, by.Length, new IPEndPoint(lasttarget.Address, Port));
        }
        void GetDevType()
        {
            sendData(cmd.alive);
        }
        void GetStaMac()
        {
            string str = "print(wifi.sta.getmac())";
        }
        void GetApMac()
        {
            string str = "print(wifi.ap.getmac())";

        }
    }
    class EquiPmentData
    {
        public int num { get; set; }
        public string id { get; set; }
        public IPAddress ip{ get; set; }
        public string type { get; set; }
    }

}
