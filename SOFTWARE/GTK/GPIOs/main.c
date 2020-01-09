#include "gpio_lib.h"
#include <gtk/gtk.h>

struct _s_EXT_GPIO
{
	char EXT_Num, EXT_Pin, GPIO_Port, GPIO_Pin;
};
typedef	struct _s_EXT_GPIO EXT_GPIO;
EXT_GPIO GPIO_Data [] =
{
	// Ext 1
	{1,  1, 'N', -1},
	{1,  2, 'N', -1},
	{1,  3, 'N', -1},
	{1,  4, 'N', -1},
	{1,  5, 'G',  0},
	{1,  6, 'N', -1},
	{1,  7, 'G',  1},
	{1,  8, 'N', -1},
	{1,  9, 'G',  2},
	{1, 10, 'N', -1},
	{1, 11, 'G',  3},
	{1, 12, 'N', -1},
	{1, 13, 'G',  4},
	{1, 14, 'N', -1},
	{1, 15, 'G',  5},
	{1, 16, 'N', -1},
	{1, 17, 'G',  6},
	{1, 18, 'N', -1},
	{1, 19, 'G',  7},
	{1, 20, 'N', -1},
	{1, 21, 'G',  8},
	{1, 22, 'N', -1},
	{1, 23, 'G',  9},
	{1, 24, 'N', -1},
	{1, 25, 'G', 10},
	{1, 26, 'N', -1},
	{1, 27, 'G', 11},
	{1, 28, 'N', -1},
	{1, 29, 'N', -1},
	{1, 30, 'N', -1},
	{1, 31, 'N', -1},
	{1, 32, 'N', -1},
	{1, 33, 'N', -1},
	{1, 34, 'N', -1},
	{1, 35, 'N', -1},
	{1, 36, 'N', -1},
	{1, 37, 'N', -1},
	{1, 38, 'N', -1},
	{1, 39, 'N', -1},
	{1, 40, 'N', -1},

	// Ext 2
	{2,  1, 'N', -1},
	{2,  2, 'N', -1},
	{2,  3, 'N', -1},
	{2,  4, 'N', -1},
	{2,  5, 'B',  0},	// TWI0-SCK
	{2,  6, 'E',  0},
	{2,  7, 'B',  1},	// TWI0-SDA
	{2,  8, 'E',  1},
	{2,  9, 'I',  0},
	{2, 10, 'E',  2},
	{2, 11, 'I',  1},
	{2, 12, 'E',  3},
	{2, 13, 'I',  2},
	{2, 14, 'E',  4},
	{2, 15, 'I',  3},
	{2, 16, 'E',  5},
	{2, 17, 'I', 10},
	{2, 18, 'E',  6},
	{2, 19, 'I', 11},
	{2, 20, 'E',  7},
	{2, 21, 'C',  3},
	{2, 22, 'E',  8},
	{2, 23, 'C',  7},
	{2, 24, 'E',  9},
	{2, 25, 'C', 16},	// BWP/SDC2_RST
	{2, 26, 'E', 10},
	{2, 27, 'C', 17},
	{2, 28, 'E', 11},
	{2, 29, 'C', 18},
	{2, 30, 'I', 14},
	{2, 31, 'C', 23},
	{2, 32, 'I', 15},
	{2, 33, 'C', 24},
	{2, 34, 'B', 23},	// RX0
	{2, 35, 'N', -1},
	{2, 36, 'B', 22},	// TX0
	{2, 37, 'N', -1},
	{2, 38, 'N', -1},
	{2, 39, 'N', -1},
	{2, 40, 'N', -1},

	// Ext 3
	{3,  1, 'N', -1},
	{3,  2, 'N', -1},
	{3,  3, 'N', -1},
	{3,  4, 'N', -1},
	{3,  5, 'H',  0},
	{3,  6, 'B',  3},
	{3,  7, 'H',  2},
	{3,  8, 'B',  4},
	{3,  9, 'H',  7},
	{3, 10, 'B',  5},
	{3, 11, 'H',  9},
	{3, 12, 'B',  6},
	{3, 13, 'H', 10},
	{3, 14, 'B',  7},
	{3, 15, 'H', 11},
	{3, 16, 'B',  8},
	{3, 17, 'H', 12},
	{3, 18, 'B', 10},
	{3, 19, 'H', 13},
	{3, 20, 'B', 11},
	{3, 21, 'H', 14},
	{3, 22, 'B', 12},
	{3, 23, 'H', 15},
	{3, 24, 'B', 13},
	{3, 25, 'H', 16},
	{3, 26, 'B', 14},
	{3, 27, 'H', 17},
	{3, 28, 'B', 15},
	{3, 29, 'H', 18},
	{3, 30, 'B', 16},
	{3, 31, 'H', 19},
	{3, 32, 'B', 17},
	{3, 33, 'H', 20},
	{3, 34, 'H', 24},
	{3, 35, 'H', 21},
	{3, 36, 'H', 25},
	{3, 37, 'H', 22},
	{3, 38, 'H', 26},
	{3, 39, 'H', 23},
	{3, 40, 'H', 27},

	// UExt 1
	{4,  1, 'N', -1},
	{4,  2, 'N', -1},
	{4,  3, 'I', 12},
	{4,  4, 'I', 13},
	{4,  5, 'B', 20},
	{4,  6, 'B', 21},
	{4,  7, 'C', 22},
	{4,  8, 'C', 21},
	{4,  9, 'C', 20},
	{4, 10, 'C', 19},

	// UExt 2
	{5,  1, 'N', -1},
	{5,  2, 'N', -1},
	{5,  3, 'I', 20},
	{5,  4, 'I', 21},
	{5,  5, 'B', 18},
	{5,  6, 'B', 19},
	{5,  7, 'I', 19},
	{5,  8, 'I', 18},
	{5,  9, 'I', 17},
	{5, 10, 'I', 16}
};

int Pin_Number (int Port, int Pin)
{
	if ((Port >= 65) && (Port <= 90))	// if Port is a letter 'A'-'Z'
		Port = Port - 65;
	if ((Port >= 97) && (Port <= 122))	// if Port is a letter 'a'-'z'
		Port = Port - 97;
	return (Port<<5) | Pin;	// Port*32 + Pin
}

GtkWidget *Combo_Port, *Combo_Pin;
int Global_Port=-1, Global_Pin=-1;

int Check_Valid_Pin ()
{
	if (Global_Port < 0) 
	{
		printf ("Select Port\n");
		return 0;
	}
	if (Global_Pin < 0) 
	{
		printf ("Select Pin\n");
		return 0;
	}
	return 1;
}

void Handler_end_program (GtkWidget *wid, gpointer ptr)
{
	gtk_main_quit ();
}

void Handler_Pin_Config_Input (GtkWidget *wid, gpointer ptr)
{
	if (Check_Valid_Pin ())
		sunxi_gpio_set_cfgpin(Pin_Number (Global_Port, Global_Pin), SUNXI_GPIO_INPUT);
}

void Handler_Pin_Config_Output (GtkWidget *wid, gpointer ptr)
{
	if (Check_Valid_Pin ())
		sunxi_gpio_set_cfgpin(Pin_Number (Global_Port, Global_Pin), SUNXI_GPIO_OUTPUT);
}

void Handler_Pin_Out (GtkWidget *wid, gpointer ptr)
{
	if (Check_Valid_Pin ())
		sunxi_gpio_output (Pin_Number(Global_Port, Global_Pin), gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wid)));
}

void Handler_Pin_In (GtkWidget *wid, gpointer ptr)
{
	if (Check_Valid_Pin ())
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ptr), sunxi_gpio_input (Pin_Number(Global_Port, Global_Pin)));
}

void Handler_Toggle_Pin (GtkWidget *wid, gpointer ptr)
{
	if (Check_Valid_Pin ())
		sunxi_gpio_output (Pin_Number(Global_Port, Global_Pin), !sunxi_gpio_input(Pin_Number(Global_Port, Global_Pin)));
}

void Handler_Port_Selected (GtkWidget *wid, gpointer ptr)
{
	Global_Port = gtk_combo_box_get_active (GTK_COMBO_BOX (wid))-1;
}

void Handler_Pin_Selected (GtkWidget *wid, gpointer ptr)
{
	Global_Pin = gtk_combo_box_get_active (GTK_COMBO_BOX (wid))-1;
}

void Handler_GPIO_Array (GtkWidget *wid, gpointer ptr)
{
	//char EXT_Num, EXT_Pin, GPIO_Port, GPIO_Pin;
	EXT_GPIO Temp = *(EXT_GPIO*)ptr;
	printf ("GPIO-%d.%d, ", Temp.EXT_Num, Temp.EXT_Pin);
	if (Temp.GPIO_Port == 'N')
	{
		printf ("Not GPIO\n");
		gtk_combo_box_set_active (GTK_COMBO_BOX (Combo_Port), 0);
		gtk_combo_box_set_active (GTK_COMBO_BOX (Combo_Pin), 0);
	}
	else
	{
		printf ("P%c%d\n", Temp.GPIO_Port, Temp.GPIO_Pin);
		Global_Port=Temp.GPIO_Port;
		Global_Pin=Temp.GPIO_Pin;

		gtk_combo_box_set_active (GTK_COMBO_BOX (Combo_Port), Global_Port-'A'+1);
		gtk_combo_box_set_active (GTK_COMBO_BOX (Combo_Pin), Global_Pin+1);
	}
}

GtkWidget *CreateRadio (GtkWidget *tbl, int Row, int Column, GSList **group, char *szLabel)
{
    GtkWidget *radio;
    radio = gtk_radio_button_new_with_label (*group, szLabel);
    *group = gtk_radio_button_group (GTK_RADIO_BUTTON (radio));
	gtk_table_attach_defaults (GTK_TABLE (tbl), radio, Row, Row+1, Column, Column+1);
    gtk_widget_show (radio);
    return (radio);
}

void main (int argc, char *argv[])
{
	// GPIOs init
	sunxi_gpio_init();

	// GTK init
	gtk_init (&argc, &argv);
	GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (win, "delete_event", G_CALLBACK (Handler_end_program), NULL);

	// Components
	GtkWidget *Table_GPIO1 = gtk_table_new (3, 2, FALSE);

	// Port/Pin selection (Dropdown/Combo)
	char Buff[5];
	Combo_Port = gtk_combo_box_text_new ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (Combo_Port), "Port");
	for (char c = 'A'; c<='I'; c++)
	{
		sprintf (Buff, "P%c", c);
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (Combo_Port), Buff);
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX (Combo_Port), 0);
	g_signal_connect (Combo_Port, "changed", G_CALLBACK (Handler_Port_Selected), NULL);

	Combo_Pin = gtk_combo_box_text_new ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (Combo_Pin), "Pin");
	for (int i = 0; i<=31; i++)
	{
		sprintf (Buff, "%d", i);
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (Combo_Pin), Buff);
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX (Combo_Pin), 0);
	g_signal_connect (Combo_Pin, "changed", G_CALLBACK (Handler_Pin_Selected), NULL);

	// Pin direction
	GtkWidget *Button_Input = gtk_button_new_with_label ("Input");
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO1), Button_Input, 0, 1, 1, 2);
	g_signal_connect (Button_Input, "clicked", G_CALLBACK (Handler_Pin_Config_Input), NULL);

	GtkWidget *Button_Output = gtk_button_new_with_label ("Output");
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO1), Button_Output, 1, 2, 1, 2);
	g_signal_connect (Button_Output, "clicked", G_CALLBACK (Handler_Pin_Config_Output), NULL);
	
	// Toggle Pin (Check Button)
	GtkWidget *Check_Pin_Level = gtk_check_button_new_with_label ("1/0");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (Check_Pin_Level), TRUE);
	g_signal_connect (Check_Pin_Level, "toggled", G_CALLBACK (Handler_Pin_Out), NULL);
	
	// Read Input (Button)
	GtkWidget *Button_Read_Pin_Level = gtk_button_new_with_label ("Read");
	g_signal_connect (Button_Read_Pin_Level, "clicked", G_CALLBACK (Handler_Pin_In), Check_Pin_Level);

	// Array of buttons	(GPIOs)
	#define	PADDING 3
	GtkWidget *Table_GPIO2 = gtk_table_new (21, 3*PADDING+2, TRUE);
	GtkWidget *Label_EXT_GPIO1 = gtk_label_new ("GPIO-1");
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO2), Label_EXT_GPIO1, PADDING*0, PADDING*0+2, 0, 1);
	GtkWidget *Label_EXT_GPIO2 = gtk_label_new ("GPIO-2");
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO2), Label_EXT_GPIO2, PADDING*1, PADDING*1+2, 0, 1);
	GtkWidget *Label_EXT_GPIO3 = gtk_label_new ("GPIO-3");
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO2), Label_EXT_GPIO3, PADDING*2, PADDING*2+2, 0, 1);
	GtkWidget *Label_EXT_GPIO4 = gtk_label_new ("UEXT-1");
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO2), Label_EXT_GPIO4, PADDING*3, PADDING*3+2, 0, 1);
	GtkWidget *Label_EXT_GPIO5 = gtk_label_new ("UEXT-2");
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO2), Label_EXT_GPIO5, PADDING*3, PADDING*3+2, 10, 11);

    GtkWidget *RadioButton_GPIO;
    GSList *RadioGroup_GPIO_Array = NULL;
	// GPIO-1,2,3
	for (int i=0; i<120; i++)
	{
		char Label[5]="";
		sprintf (Label, "%2.d", (i%40)+1);
		RadioButton_GPIO = CreateRadio (Table_GPIO2, (i/40)*PADDING+(i%2), (i/2)%20+1, &RadioGroup_GPIO_Array, Label);
		gtk_signal_connect (GTK_OBJECT (RadioButton_GPIO), "clicked", GTK_SIGNAL_FUNC (Handler_GPIO_Array), &GPIO_Data[i]);
	}

	// UEXT 1,2
	for (int i=0; i<20; i++)
	{
		char Label[5]="";
		sprintf (Label, "%2.d", (i%10)+1);
		RadioButton_GPIO = CreateRadio (Table_GPIO2, 3*PADDING+(i%2), (i/2)%10+(i/10)*5+1, &RadioGroup_GPIO_Array, Label);
		gtk_signal_connect (GTK_OBJECT (RadioButton_GPIO), "clicked", GTK_SIGNAL_FUNC (Handler_GPIO_Array), &GPIO_Data[i+120]);
	}

	// Adding to the table
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO1), Combo_Port, 0, 1, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO1), Combo_Pin, 1, 2, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO1), Button_Read_Pin_Level, 0, 1, 2, 3);
	gtk_table_attach_defaults (GTK_TABLE (Table_GPIO1), Check_Pin_Level, 1, 2, 2, 3);

	GtkWidget *box = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX (box), Table_GPIO1, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (box), Table_GPIO2, TRUE, TRUE, 0);
	gtk_container_add (GTK_CONTAINER (win), box);

	// GTK function
	gtk_widget_show_all (win);
	gtk_main ();
}
