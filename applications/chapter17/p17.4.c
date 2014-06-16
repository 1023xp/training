#include <gtk/gtk.h>

gint release_resource(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	gtk_main_quit();
	return 0;
}

int main(int argc,char * argv[])
{
	GtkWidget *wnd;
	GtkWidget *btn_box;
	GtkWidget *btn;

	gtk_init(&argc,&argv);

	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"p16.4 button box");
	gtk_window_set_default_size(GTK_WINDOW(wnd),400,50);
	gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	btn_box=gtk_hbutton_box_new();
	gtk_container_add(GTK_CONTAINER(wnd),btn_box);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(btn_box),GTK_BUTTONBOX_START);	

	btn=gtk_button_new_with_label("button 1");
	gtk_box_pack_start(GTK_BOX(btn_box),btn,FALSE,FALSE,0);

	btn=gtk_button_new_with_label("button 2");
	gtk_box_pack_end(GTK_BOX(btn_box),btn,FALSE,TRUE,0);

        btn=gtk_button_new_with_label("button 3");
        gtk_box_pack_start(GTK_BOX(btn_box),btn,FALSE,FALSE,0);

        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);
	
	gtk_main();

	return 0;
}
