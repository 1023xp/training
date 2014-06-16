#include <gtk/gtk.h>

void cb_push_button(GtkWidget *widget,gpointer data)
{
	gtk_window_set_title(GTK_WINDOW(data),"receive push button signal!");
}

gint release_resource(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	gtk_main_quit();
	return 0;
}

int main(int argc,char* argv[])
{
	GtkWidget *wnd;
	GtkWidget *btn;

	gtk_init(&argc,&argv);

	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"catch signal and event!");
	gtk_window_set_default_size(GTK_WINDOW(wnd),250,200);

	btn=gtk_button_new_with_label("set title");
	gtk_container_add(GTK_CONTAINER(wnd),btn);

	g_signal_connect(G_OBJECT(btn),"clicked",G_CALLBACK(cb_push_button),wnd);
	g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);

	gtk_main();
	
	return 0;
}
