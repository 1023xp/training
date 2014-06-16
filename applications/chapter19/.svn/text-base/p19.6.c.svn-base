#include <gtk/gtk.h>

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
        gtk_main_quit ();
        return FALSE;
}

void close_msg_box_handler(GtkWidget *widget,gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));	
}

void cb_ok_handler(GtkWidget *widget,gpointer data)
{
	GtkWidget *msg_box;
	GtkWidget *label;
	GtkWidget *btn;
	GtkWidget *file_sel;
	const gchar *var;

	file_sel=GTK_WIDGET(data);	
	var=gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_sel));

	msg_box=gtk_dialog_new();
	gtk_window_set_title(GTK_WINDOW(msg_box),"notice");
	gtk_widget_set_size_request(msg_box,200,200);	

	if(var==NULL)
		label=gtk_label_new("the user didn't select any files");
	else
		label=gtk_label_new(var);

	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(msg_box)->vbox),label,TRUE,TRUE,0);
	
	btn=gtk_button_new_with_label("OK");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(msg_box)->action_area),btn,TRUE,TRUE,0);
	g_signal_connect(GTK_OBJECT(btn),"clicked",
					GTK_SIGNAL_FUNC(close_msg_box_handler),msg_box);
	
	btn=gtk_button_new_with_label("Cancel");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(msg_box)->action_area),btn,TRUE,TRUE,0);

	gtk_widget_show_all(msg_box);

}

int main(int argc,char *argv[])
{
        GtkWidget *file_sel;
	GtkWidget *btn;

        gtk_init (&argc, &argv);
        
	file_sel= gtk_file_selection_new("p19.6");
	gtk_file_selection_complete(file_sel,"/home/program/");
	gtk_file_selection_hide_fileop_buttons(file_sel);

	btn=gtk_button_new_with_label("button in button area");
	gtk_box_pack_start(GTK_BOX(GTK_FILE_SELECTION(file_sel)->button_area),
							btn,FALSE,FALSE,0);

	btn=gtk_button_new_with_label("button in action area");
	gtk_box_pack_start(GTK_BOX(GTK_FILE_SELECTION(file_sel)->action_area),
							btn,FALSE,FALSE,0);

        g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(file_sel)->ok_button),"clicked",
					GTK_SIGNAL_FUNC(cb_ok_handler),(gpointer)file_sel);

	
        g_signal_connect(G_OBJECT(file_sel),"delete_event",
					G_CALLBACK(release_resource),NULL);
        gtk_widget_show_all(file_sel);
        gtk_main();
        return 0;
}
