#include <gtk/gtk.h>

void entry_changed_handler(GtkEntry *entry,gpointer data)
{
	g_print("user begin change the entry content\n");
}

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
	gtk_main_quit ();
	return FALSE;
}

int main(int argc,char *argv[])
{
	GtkWidget *entry1;
	GtkWidget *entry2;
	GtkWidget *wnd;
	GtkWidget *vbox;

	gtk_init (&argc, &argv);
        wnd= gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
        gtk_window_set_title(GTK_WINDOW(wnd),"p18.2 entry");
        gtk_window_set_default_size(GTK_WINDOW(wnd),200,100);
        gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(wnd),vbox);

	entry1=gtk_entry_new_with_max_length(20);
	gtk_box_pack_start(GTK_BOX(vbox),entry1,TRUE,TRUE,0);

	gtk_entry_set_text(GTK_ENTRY(entry1),"Entry example");

	entry2=gtk_entry_new_with_max_length(30);
	gtk_box_pack_start(GTK_BOX(vbox),entry2,TRUE,TRUE,0);
        gtk_entry_set_text(GTK_ENTRY(entry2),"Entry example disable input");
	
	gtk_editable_set_editable(GTK_EDITABLE(entry2),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(entry2),FALSE);

        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	g_signal_connect(G_OBJECT(entry1),"changed",GTK_SIGNAL_FUNC(entry_changed_handler),NULL);
	gtk_widget_show_all(wnd);

	gtk_main();
	
	return 0;
}
