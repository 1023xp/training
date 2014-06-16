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
        GtkWidget *file_sel;

        gtk_init (&argc, &argv);
        
	file_sel= gtk_file_selection_new("p19.3");

        g_signal_connect(G_OBJECT(file_sel),"delete_event",G_CALLBACK(release_resource),NULL);
        gtk_widget_show_all(file_sel);
        gtk_main();
        return 0;
}


