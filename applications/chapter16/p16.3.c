#include <gtk/gtk.h>

gint release_resource(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	gtk_main_quit();
	return FALSE;
}

int main(int argc,char* argv[])
{
	GtkWidget *wnd;
	gtk_init(&argc,&argv);
	
	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(wnd,"P16.3 program");
	gtk_widget_set_usize(wnd,300,300);
	
	g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show(wnd);

	gtk_main();
	return 0;
}
