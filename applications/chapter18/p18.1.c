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
	GtkWidget *label;
	GtkWidget *wnd;
	char *label_text;

	gtk_init (&argc, &argv);
        wnd= gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
        gtk_window_set_title(GTK_WINDOW(wnd),"p18.1 label");
        gtk_window_set_default_size(GTK_WINDOW(wnd),200,100);
        gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	label=gtk_label_new("label text");
	gtk_container_add(GTK_CONTAINER(wnd),label);
	
	label_text=gtk_label_get_text(GTK_LABEL(label));
	g_print("label : %s\n",label_text);

	gtk_label_set_text(GTK_LABEL(label),"set label text");

        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

        gtk_widget_show_all(wnd);

	gtk_main();
	
	return 0;
}
