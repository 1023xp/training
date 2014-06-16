#include <gtk/gtk.h>

void cb_btn_press(GtkWidget *toggle_btn,gpointer data)
{
	gboolean btn_stat;

	btn_stat=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_btn));

	if(btn_stat==TRUE)
		g_print("stat is TRUE\n");
	else
		g_print("stat is FALSE\n");
}

gint release_resource(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	gtk_main_quit();
	return 0;
}

int main(int argc,char * argv[])
{
	GtkWidget *wnd;
	GtkWidget *toggle_btn;

	gtk_init(&argc,&argv);

	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"p17.8 toggle button");
	gtk_window_set_default_size(GTK_WINDOW(wnd),200,200);
	gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	toggle_btn=gtk_toggle_button_new_with_label("click me!");
	gtk_container_add(GTK_CONTAINER(wnd),toggle_btn);	

	g_signal_connect(G_OBJECT(toggle_btn),"clicked",G_CALLBACK(cb_btn_press),NULL);
        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);
	
	gtk_main();

	return 0;
}
