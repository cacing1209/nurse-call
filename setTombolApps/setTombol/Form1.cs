namespace setTombol
{
    public partial class form_Setupbutton : Form
    {
        public form_Setupbutton()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(comboBox1.SelectedItem == null )
              {
                MessageBox.Show("Bed 1 belum dipilih");
                return;
            }
            MessageBox.Show("program has ben uploding");            
        }

        
        private void show_ListCombobox()
        {
            int total_ComboBOX = 72;
            for (int set = 0; set < 2; set++)
            {                
            for (int i = 1; i <= total_ComboBOX; i++)
            {
             String setupName = " KAMAR ";
                
                if (i <= 10 && set==1)
                {
                    setupName = " BED ";
                    comboBox2.Items.Add(setupName + i.ToString());
                    comboBox4.Items.Add(setupName + i.ToString());
                    comboBox6.Items.Add(setupName + i.ToString());
                    comboBox8.Items.Add(setupName + i.ToString());
                    comboBox10.Items.Add(setupName + i.ToString());
                    comboBox12.Items.Add(setupName + i.ToString());
                    comboBox14.Items.Add(setupName + i.ToString());
                    comboBox16.Items.Add(setupName + i.ToString());
                    comboBox18.Items.Add(setupName + i.ToString());
                    comboBox20.Items.Add(setupName + i.ToString());
                    comboBox22.Items.Add(setupName + i.ToString());
                    comboBox24.Items.Add(setupName + i.ToString());
                    comboBox26.Items.Add(setupName + i.ToString());
                    comboBox28.Items.Add(setupName + i.ToString());
                    comboBox30.Items.Add(setupName + i.ToString());
                    comboBox32.Items.Add(setupName + i.ToString());
                    comboBox34.Items.Add(setupName + i.ToString());
                    comboBox36.Items.Add(setupName + i.ToString());
                    comboBox38.Items.Add(setupName + i.ToString());
                    comboBox40.Items.Add(setupName + i.ToString());
                    comboBox42.Items.Add(setupName + i.ToString());
                    comboBox44.Items.Add(setupName + i.ToString());
                    comboBox46.Items.Add(setupName + i.ToString());
                    comboBox48.Items.Add(setupName + i.ToString());
                    comboBox50.Items.Add(setupName + i.ToString());
                    comboBox52.Items.Add(setupName + i.ToString());
                    comboBox54.Items.Add(setupName + i.ToString());
                    comboBox56.Items.Add(setupName + i.ToString());
                    comboBox58.Items.Add(setupName + i.ToString());
                    comboBox60.Items.Add(setupName + i.ToString());
                    comboBox62.Items.Add(setupName + i.ToString());
                    comboBox64.Items.Add(setupName + i.ToString());
                    comboBox66.Items.Add(setupName + i.ToString());
                    comboBox68.Items.Add(setupName + i.ToString());
                    comboBox70.Items.Add(setupName + i.ToString());
                    comboBox72.Items.Add(setupName + i.ToString());
                }

           
                comboBox1.Items.Add(setupName + i.ToString());
                comboBox3.Items.Add(setupName + i.ToString());
                comboBox5.Items.Add(setupName + i.ToString());
                comboBox7.Items.Add(setupName + i.ToString());
                comboBox9.Items.Add(setupName + i.ToString());
                comboBox11.Items.Add(setupName + i.ToString());
                comboBox13.Items.Add(setupName + i.ToString());
                comboBox15.Items.Add(setupName + i.ToString());
                comboBox17.Items.Add(setupName + i.ToString());
                comboBox19.Items.Add(setupName + i.ToString());
                comboBox21.Items.Add(setupName + i.ToString());
                comboBox23.Items.Add(setupName + i.ToString());
                comboBox25.Items.Add(setupName + i.ToString());
                comboBox27.Items.Add(setupName + i.ToString());
                comboBox29.Items.Add(setupName + i.ToString());
                comboBox31.Items.Add(setupName + i.ToString());
                comboBox33.Items.Add(setupName + i.ToString());
                comboBox35.Items.Add(setupName + i.ToString());
                comboBox37.Items.Add(setupName + i.ToString());
                comboBox39.Items.Add(setupName + i.ToString());
                comboBox41.Items.Add(setupName + i.ToString());
                comboBox43.Items.Add(setupName + i.ToString());
                comboBox45.Items.Add(setupName + i.ToString());
                comboBox47.Items.Add(setupName + i.ToString());
                comboBox49.Items.Add(setupName + i.ToString());
                comboBox51.Items.Add(setupName + i.ToString());
                comboBox53.Items.Add(setupName + i.ToString());
                comboBox55.Items.Add(setupName + i.ToString());
                comboBox57.Items.Add(setupName + i.ToString());
                comboBox59.Items.Add(setupName + i.ToString());
                comboBox61.Items.Add(setupName + i.ToString());
                comboBox63.Items.Add(setupName + i.ToString());
                comboBox65.Items.Add(setupName + i.ToString());
                comboBox67.Items.Add(setupName + i.ToString());
                comboBox69.Items.Add(setupName + i.ToString());
                comboBox71.Items.Add(setupName + i.ToString());
            }
            }



        }

        private void value_Equipment(int freeValue)
        {
            string[] setupInput = new string[36];

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            show_ListCombobox();
            

        }


        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

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
