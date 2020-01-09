#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "I2C.h"

GtkWidget *Text_SlaveAddress, *Text_Register, *Text_Data, *Combo_I2C, *Spin_NumberBytes;
unsigned char Read_Write = I2C_READ;

#define	HEX	16
#define	DEC	10
long int String_to_Int (const char Input[], int Base)
{
	int i=0;
	long int Number = 0;
	int Sign=1;
	if (Input[0]=='-')
	{
		Sign=-1;
		i++;
	}
	switch (Base)
	{
		case DEC:
			for (; Input[i]; i++)
				if ((Input[i]>='0') && (Input[i]<='9'))
					Number = Number * 10 + (Input [i] - '0');
		break;
		case HEX:
			for (; Input[i]; i++)
			{
				if ((Input[i]>='0') && (Input[i]<='9'))
					Number = Number * 16 + Input [i] - '0';
				if ((Input[i]>='A') && (Input[i]<='F'))
					Number = Number * 16 + (Input [i] - 'A' + 10);
				if ((Input[i]>='a') && (Input[i]<='f'))
					Number = Number * 16 + (Input [i] - 'a' + 10);
			}
		break;
	}
	return Sign*Number;
}

void Handler_TransferData (GtkWidget *wid, gpointer ptr)
{
	#define	MAX_NUMBER_BYTES	50
	unsigned char Data[MAX_NUMBER_BYTES];
	//printf ("Transfer started!\n");
	const char *Str_Address = gtk_entry_get_text (GTK_ENTRY (Text_SlaveAddress));
	const char *Str_Register = gtk_entry_get_text (GTK_ENTRY (Text_Register));
	const char *Str_Bytes = gtk_entry_get_text (GTK_ENTRY (Spin_NumberBytes));
	
	unsigned char Address = String_to_Int(Str_Address, HEX);
	unsigned char Register = String_to_Int(Str_Register, HEX);
	unsigned char Bytes = String_to_Int(Str_Bytes, DEC);
	
	//printf ("Address: %s (Hex) %d (Dec)\nRegister: %s (Hex) %d (Dec)\nBytes %s (Dec) %d (Dec)\n", Str_Address, Address, Str_Register, Register, Str_Bytes, Bytes);
	if (Read_Write == I2C_READ)
	{
		char Buff[4], Text[MAX_NUMBER_BYTES*4+1]="";
		I2C_Transfer (Address, I2C_READ, Register, Data, Bytes);
		for (int i=0; i<Bytes; i++)
		{
			sprintf (Buff, "%X ", Data[i]);
			strcat (Text, Buff);
		}
		gtk_entry_set_text  (GTK_ENTRY (Text_Data), Text);
	}
	else
	{
		const char *Text=gtk_entry_get_text (GTK_ENTRY (Text_Data));
		char Buff[4];
		int i=0, j=0, DataIndex=0;
		//printf ("%s\n", Text);
		for (i=0; ; i++)
		{
			if (((Text[i] >= '0') && (Text[i] <= '9')) ||
				((Text[i] >= 'A') && (Text[i] <= 'F')) ||
				((Text[i] >= 'a') && (Text[i] <= 'f')))
				Buff[j++] = Text[i];
			else
				if (j>0)	// invalid digit following a valid digit ==> end of number
				{
					Buff[j] = 0;
					Data[DataIndex] = String_to_Int (Buff, HEX);
					DataIndex++;
					j = 0;
				}

			if ((j==2) || (!Text[i]))	// if we have 2 consequtive valid digits or end of file --> calculate current
			{
				Buff[j] = 0;
				Data[DataIndex] = String_to_Int (Buff, HEX);
				DataIndex++;
				j = 0;
			}
			if (!Text[i])
				break;
		}
		for (i=DataIndex; i<Bytes; i++)
			Data[i] = 0;
		/*
		for (i=0; i<Bytes; i++)
			printf ("%X ", Data[i]);
		printf ("\n");
		*/
		I2C_Transfer (Address, I2C_WRITE, Register, Data, Bytes);
	}
}

void Handler_Config_Read  (GtkWidget *wid, gpointer ptr)
{
	Read_Write = I2C_READ;
	//printf ("%d\n", Read_Write);
}

void Handler_Config_Write  (GtkWidget *wid, gpointer ptr)
{
	Read_Write = I2C_WRITE;
	//printf ("%d\n", Read_Write);
}

void Handler_end_program (GtkWidget *wid, gpointer ptr)
{
	gtk_main_quit ();
}

void Handler_I2C_Selected (GtkWidget *wid, gpointer ptr)
{
	I2C[9] = gtk_combo_box_get_active (GTK_COMBO_BOX (wid))+'0';
	//printf ("%s\n", I2C);
}

int main (int argc, char *argv[])
{
	// GTK init
	gtk_init (&argc, &argv);
	GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (win, "delete_event", G_CALLBACK (Handler_end_program), NULL);

	GtkWidget *Label_I2C = gtk_label_new ("I2C");
	GtkWidget *Label_SlaveAddress = gtk_label_new ("Slave Address");
	GtkWidget *Label_Register = gtk_label_new ("Register");
	GtkWidget *Label_Data = gtk_label_new ("Data");
	GtkWidget *Label_NumberBytes = gtk_label_new ("Number of Bytes");

	GtkWidget *Button_Transfer = gtk_button_new_with_label ("Transfer");
	g_signal_connect (Button_Transfer, "clicked", G_CALLBACK (Handler_TransferData), NULL);
	Text_SlaveAddress = gtk_entry_new ();
	Text_Register = gtk_entry_new ();
	Text_Data = gtk_entry_new ();

	GtkObject *Adj_NumberBytes = gtk_adjustment_new (1, 1, 100, 1, 0, 0);
	Spin_NumberBytes = gtk_spin_button_new (GTK_ADJUSTMENT (Adj_NumberBytes), 0, 0);

	Combo_I2C = gtk_combo_box_text_new ();
	for (int i = 0; i<=5; i++)
	{
		char Buff[5];
		sprintf (Buff, "I2C%d", i);
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (Combo_I2C), Buff);
	}

	gtk_combo_box_set_active (GTK_COMBO_BOX (Combo_I2C), 0);
	g_signal_connect (Combo_I2C, "changed", G_CALLBACK (Handler_I2C_Selected), NULL);

	// I2C Write/Read
	GtkWidget *Button_Read = gtk_radio_button_new_with_label (NULL, "Read");
	GSList *group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (Button_Read));
	GtkWidget *Button_Write = gtk_radio_button_new_with_label (group, "Write");
	g_signal_connect (Button_Read, "clicked", G_CALLBACK (Handler_Config_Read), NULL);
	g_signal_connect (Button_Write, "clicked", G_CALLBACK (Handler_Config_Write), NULL);

	GtkWidget *Table_Widgets = gtk_table_new (7, 2, TRUE);
	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Label_I2C, 0, 1, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Combo_I2C, 1, 2, 0, 1);

	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Button_Read, 0, 1, 1, 2);
	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Button_Write, 1, 2, 1, 2);

	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Label_SlaveAddress, 0, 1, 2, 3);
	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Text_SlaveAddress, 1, 2, 2, 3);

	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Label_Register, 0, 1, 3, 4);
	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Text_Register, 1, 2, 3, 4);

	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Label_NumberBytes, 0, 1, 4, 5);
	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Spin_NumberBytes, 1, 2, 4, 5);

	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Label_Data, 0, 1, 5, 6);
	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Text_Data, 1, 2, 5, 6);

	gtk_table_attach_defaults (GTK_TABLE (Table_Widgets), Button_Transfer, 0, 2, 6, 7);
	gtk_container_add (GTK_CONTAINER (win), Table_Widgets);

	// GTK function
	gtk_widget_show_all (win);
	gtk_main ();
}