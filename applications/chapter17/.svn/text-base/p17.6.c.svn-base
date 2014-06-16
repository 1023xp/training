#include <gtk/gtk.h>

gint release_resource(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	gtk_main_quit();
	return 0;
}

int main(int argc,char * argv[])
{
	GtkWidget *wnd;
	GtkWidget *fixed;
	GtkWidget *btn;

	gtk_init(&argc,&argv);

	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"p17.6 fixed");
	gtk_window_set_default_size(GTK_WINDOW(wnd),200,200);
	gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	fixed=gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(wnd),fixed);

	btn=gtk_button_new_with_label("button 1");
	gtk_fixed_put(GTK_FIXED(fixed),btn,2,2);

	btn=gtk_button_new_with_label("button 2");
        gtk_fixed_put(GTK_FIXED(fixed),btn,100,20);
	
        btn=gtk_button_new_with_label("button 3");
        gtk_fixed_put(GTK_FIXED(fixed),btn,150,140);

        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);
	
	gtk_main();

	return 0;
}
