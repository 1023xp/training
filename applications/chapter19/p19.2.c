#include <gtk/gtk.h>

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
        gtk_main_quit ();
        return FALSE;
}

int main(int argc,char *argv[])
{
        GtkWidget *dialog;
	GtkWidget *label;
	GtkWidget *btn1;
	GtkWidget *btn2;	

        gtk_init (&argc, &argv);
        
	dialog= gtk_dialog_new();
        gtk_window_set_title(GTK_WINDOW(dialog),"p19.1");
	gtk_widget_set_size_request(dialog,200,200);
	
	label=gtk_label_new("text sample");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,TRUE,TRUE,0);

	btn1=gtk_button_new_with_label("Ok");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),btn1,FALSE,FALSE,0);

	btn2=gtk_button_new_with_label("Cancel");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),btn2,FALSE,FALSE,0);

        g_signal_connect(G_OBJECT(dialog),"delete_event",G_CALLBACK(release_resource),NULL);
        gtk_widget_show_all(dialog);
        gtk_main();
        return 0;
}


