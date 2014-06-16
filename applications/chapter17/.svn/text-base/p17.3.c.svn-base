#include <gtk/gtk.h>


gint release_resource(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	gtk_main_quit();
	return 0;
}

int main(int argc,char * argv[])
{
	GtkWidget *wnd;
	GtkWidget *box;
	GtkWidget *btn;

	gtk_init(&argc,&argv);

	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"Using VBox");
	gtk_window_set_default_size(GTK_WINDOW(wnd),300,200);
	gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	box=gtk_vbox_new(TRUE,5);

	gtk_container_add(GTK_CONTAINER(wnd),box);

	btn=gtk_button_new_with_label("BUTTON ONE");

	gtk_box_pack_start(GTK_BOX(box),btn,FALSE,TRUE,0);

	btn=gtk_button_new_with_label("BUTTON TWO");

	gtk_box_pack_start(GTK_BOX(box),btn,FALSE,TRUE,0);

        btn=gtk_button_new_with_label("BUTTON THREE");

        gtk_box_pack_start(GTK_BOX(box),btn,FALSE,TRUE,0);

        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);
	
	gtk_main();

	return 0;
}
