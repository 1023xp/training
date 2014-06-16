#include <gtk/gtk.h>

gint release_resource(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	gtk_main_quit();
	return 0;
}

int main(int argc,char * argv[])
{
	GtkWidget *wnd;
	GtkWidget *table;
	GtkWidget *btn;

	gtk_init(&argc,&argv);

	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"p17.5 table");
	gtk_window_set_default_size(GTK_WINDOW(wnd),200,200);
	gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	table=gtk_table_new(3,3,TRUE);
	gtk_container_add(GTK_CONTAINER(wnd),table);

	btn=gtk_button_new_with_label("button 1");
	gtk_table_attach(GTK_TABLE(table),btn,0,3,0,1,GTK_FILL,GTK_FILL,0,0);

	btn=gtk_button_new_with_label("button 2");
        gtk_table_attach(GTK_TABLE(table),btn,0,1,1,3,GTK_FILL,GTK_FILL,0,0);

        btn=gtk_button_new_with_label("button 3");
        gtk_table_attach_defaults(GTK_TABLE(table),btn,2,3,2,3);

        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);
	
	gtk_main();

	return 0;
}
