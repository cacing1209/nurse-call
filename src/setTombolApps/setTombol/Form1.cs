using System.Diagnostics;
using ArduinoPortApp;
namespace setTombol
{
    public partial class form_Setupbutton : Form
    {
    ComboBox[] myComboBox;
        public form_Setupbutton()
        {
            InitializeComponent();
            //ComboBox[] comboBoxes = new ComboBox[]
            myComboBox = new ComboBox[]
            {
       comboBox1, comboBox2, comboBox3, comboBox4, comboBox5, comboBox6, comboBox7, comboBox8, comboBox9, comboBox10,
comboBox11, comboBox12, comboBox13, comboBox14, comboBox15, comboBox16, comboBox17, comboBox18, comboBox19, comboBox20,
comboBox21, comboBox22, comboBox23, comboBox24, comboBox25, comboBox26, comboBox27, comboBox28, comboBox29, comboBox30,
comboBox31, comboBox32, comboBox33, comboBox34, comboBox35, comboBox36, comboBox37, comboBox38, comboBox39, comboBox40,
comboBox41, comboBox42, comboBox43, comboBox44, comboBox45, comboBox46, comboBox47, comboBox48, comboBox49, comboBox50,
comboBox51, comboBox52, comboBox53, comboBox54, comboBox55, comboBox56, comboBox57, comboBox58, comboBox59, comboBox60,
comboBox61, comboBox62, comboBox63, comboBox64, comboBox65, comboBox66, comboBox67, comboBox68, comboBox69, comboBox70,
comboBox71, comboBox72
        };
            string[] ports = SerialPortHelper.GetAvailablePorts();
            comboBoxPorts.Items.AddRange(ports);
        }

        private bool CheckComboBoxes()
        {
            for (int indexArrCombobox = 0; indexArrCombobox < myComboBox.Length; indexArrCombobox++)
            {
                if (myComboBox[indexArrCombobox].SelectedItem == null)
                {                    
                MessageBox.Show($"Bed {indexArrCombobox + 1} belum di pilih kocak");
                return false;
                }
            }
            MessageBox.Show("Semua Bed sudah dipilih!");
            return true;

        }


        private void button1_Click(object sender, EventArgs e)
        {
            if (!CheckComboBoxes())
            {
                MessageBox.Show("program ndak bisa di upload ..");
                return;
            }
            MessageBox.Show("program nah gitu lengkap..");
        }

        private void show_ListCombobox()
        {

            int total_ComboBOX = 30;
            String setupName = " KAMAR ";
            for (int setupComboBox = 0; setupComboBox < total_ComboBOX; setupComboBox++)
            {

            }
        }


        //private void show_ListCombobox()
        //{
        //    String setupName = " KAMAR ";
        //    int total_ComboBOX = 30;
        //    for (int set = 0; set < 2; set++)
        //    {
        //        for (int i = 1; i <= total_ComboBOX; i++)
        //        if (i <= 10 && set == 0)
        //        {
        //            setupName = " BED ";
        //            comboBox2.Items.Add(setupName + i.ToString());
        //            comboBox4.Items.Add(setupName + i.ToString());
        //            comboBox6.Items.Add(setupName + i.ToString());
        //            comboBox8.Items.Add(setupName + i.ToString());
        //            comboBox10.Items.Add(setupName + i.ToString());
        //            comboBox12.Items.Add(setupName + i.ToString());
        //            comboBox14.Items.Add(setupName + i.ToString());
        //            comboBox16.Items.Add(setupName + i.ToString());
        //            comboBox18.Items.Add(setupName + i.ToString());
        //            comboBox20.Items.Add(setupName + i.ToString());
        //            comboBox22.Items.Add(setupName + i.ToString());
        //            comboBox24.Items.Add(setupName + i.ToString());
        //            comboBox26.Items.Add(setupName + i.ToString());
        //            comboBox28.Items.Add(setupName + i.ToString());
        //            comboBox30.Items.Add(setupName + i.ToString());
        //            comboBox32.Items.Add(setupName + i.ToString());
        //            comboBox34.Items.Add(setupName + i.ToString());
        //            comboBox36.Items.Add(setupName + i.ToString());
        //            comboBox38.Items.Add(setupName + i.ToString());
        //            comboBox40.Items.Add(setupName + i.ToString());
        //            comboBox42.Items.Add(setupName + i.ToString());
        //            comboBox44.Items.Add(setupName + i.ToString());
        //            comboBox46.Items.Add(setupName + i.ToString());
        //            comboBox48.Items.Add(setupName + i.ToString());
        //            comboBox50.Items.Add(setupName + i.ToString());
        //            comboBox52.Items.Add(setupName + i.ToString());
        //            comboBox54.Items.Add(setupName + i.ToString());
        //            comboBox56.Items.Add(setupName + i.ToString());
        //            comboBox58.Items.Add(setupName + i.ToString());
        //            comboBox60.Items.Add(setupName + i.ToString());
        //            comboBox62.Items.Add(setupName + i.ToString());
        //            comboBox64.Items.Add(setupName + i.ToString());
        //            comboBox66.Items.Add(setupName + i.ToString());
        //            comboBox68.Items.Add(setupName + i.ToString());
        //            comboBox70.Items.Add(setupName + i.ToString());
        //            comboBox72.Items.Add(setupName + i.ToString());
        //        }
        //        else if (set == 1 && setupName == " KAMAR ")
        //        {



        //            comboBox1.Items.Add(setupName + i.ToString());
        //            comboBox3.Items.Add(setupName + i.ToString());
        //            comboBox5.Items.Add(setupName + i.ToString());
        //            comboBox7.Items.Add(setupName + i.ToString());
        //            comboBox9.Items.Add(setupName + i.ToString());
        //            comboBox11.Items.Add(setupName + i.ToString());
        //            comboBox13.Items.Add(setupName + i.ToString());
        //            comboBox15.Items.Add(setupName + i.ToString());
        //            comboBox17.Items.Add(setupName + i.ToString());
        //            comboBox19.Items.Add(setupName + i.ToString());
        //            comboBox21.Items.Add(setupName + i.ToString());
        //            comboBox23.Items.Add(setupName + i.ToString());
        //            comboBox25.Items.Add(setupName + i.ToString());
        //            comboBox27.Items.Add(setupName + i.ToString());
        //            comboBox29.Items.Add(setupName + i.ToString());
        //            comboBox31.Items.Add(setupName + i.ToString());
        //            comboBox33.Items.Add(setupName + i.ToString());
        //            comboBox35.Items.Add(setupName + i.ToString());
        //            comboBox37.Items.Add(setupName + i.ToString());
        //            comboBox39.Items.Add(setupName + i.ToString());
        //            comboBox41.Items.Add(setupName + i.ToString());
        //            comboBox43.Items.Add(setupName + i.ToString());
        //            comboBox45.Items.Add(setupName + i.ToString());
        //            comboBox47.Items.Add(setupName + i.ToString());
        //            comboBox49.Items.Add(setupName + i.ToString());
        //            comboBox51.Items.Add(setupName + i.ToString());
        //            comboBox53.Items.Add(setupName + i.ToString());
        //            comboBox55.Items.Add(setupName + i.ToString());
        //            comboBox57.Items.Add(setupName + i.ToString());
        //            comboBox59.Items.Add(setupName + i.ToString());
        //            comboBox61.Items.Add(setupName + i.ToString());
        //            comboBox63.Items.Add(setupName + i.ToString());
        //            comboBox65.Items.Add(setupName + i.ToString());
        //            comboBox67.Items.Add(setupName + i.ToString());
        //            comboBox69.Items.Add(setupName + i.ToString());
        //            comboBox71.Items.Add(setupName + i.ToString());
        //        }
        //    }
        //}



        private void Form1_Load(object sender, EventArgs e)
        {
            show_ListCombobox();
           
        }
        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label33_Click(object sender, EventArgs e)
        {

        }
        private void value_Equipment(int freeValue)
        {
            string[] setupInput = new string[36];

        }



        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }


        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void checkbox_kamarMandi_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged_1(object sender, EventArgs e)
        {

        }
    }
}


