#include <gtk/gtk.h>

gint release_resource(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	gtk_main_quit();
	return 0;
}

int main(int argc,char * argv[])
{
	GtkWidget *wnd;
	GtkWidget *scrolled_wnd;
	GtkWidget *btn;
	GtkWidget *layout;
	
	int i;
	int j;
	char buffer[50];

	gtk_init(&argc,&argv);

	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"p17.7 layout");
	gtk_window_set_default_size(GTK_WINDOW(wnd),200,200);
	gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	scrolled_wnd=gtk_scrolled_window_new(NULL,NULL);
	gtk_container_add(GTK_CONTAINER(wnd),scrolled_wnd);
	
	layout=gtk_layout_new(NULL,NULL);
	gtk_container_add(GTK_CONTAINER(scrolled_wnd),layout);
	gtk_layout_set_size(GTK_LAYOUT(layout),1024,768);	

	for(i=0;i<1024;i+=150){	
		for(j=0;j<640;j+=50){
			sprintf(buffer,"button %d %d",i,j);
			btn=gtk_button_new_with_label(buffer);
			gtk_layout_put(GTK_LAYOUT(layout),btn,i,j);
		}
	}
        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);
	
	gtk_main();

	return 0;
}
