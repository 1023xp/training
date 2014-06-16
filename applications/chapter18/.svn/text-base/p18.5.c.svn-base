#include <gtk/gtk.h>


gboolean progress_timeout_handler(gpointer data)
{
 	gdouble new_val;
	GtkWidget *bar=(GtkWidget *)data;
 
   	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (bar)) + 0.01;
      
      	if (new_val > 1.0)
		new_val = 0.0;
      
      	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(bar),new_val);
    
  	return TRUE;
} 

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
	g_source_remove(*(guint*)data);
	gtk_main_quit();

        return FALSE;
}

void cb_clicked_handler(GtkWidget *widget,gpointer data)
{
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR((GtkWidget*)data),
							GTK_PROGRESS_RIGHT_TO_LEFT);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR((GtkWidget*)data),NULL);
}


int main( int  argc,
          char *argv[])
{
    	GtkWidget *wnd;
    	GtkWidget *vbox;
	GtkWidget *bar1;
	GtkWidget *btn;
	guint timer_id;

        gtk_init (&argc, &argv);
        
	wnd= gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(wnd),"p18.5 progress bar");
        gtk_window_set_default_size(GTK_WINDOW(wnd),300,200);
        gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

        vbox=gtk_vbox_new(TRUE,5);
        gtk_container_add(GTK_CONTAINER(wnd),vbox);	

	bar1=gtk_progress_bar_new();
	gtk_box_pack_start(GTK_BOX(vbox),bar1,TRUE,TRUE,0);	
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(bar1),
						GTK_PROGRESS_LEFT_TO_RIGHT);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(bar1),"bar1");
	
	timer_id=g_timeout_add(100,progress_timeout_handler,bar1);

        btn=gtk_button_new_with_label("change move direction");
        gtk_box_pack_start(GTK_BOX(vbox),btn,TRUE,TRUE,0);

        g_signal_connect(G_OBJECT(btn),"clicked",GTK_SIGNAL_FUNC(cb_clicked_handler),(gpointer)bar1);	
	
 	g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),(gpointer)&timer_id);
 	
 	 gtk_widget_show_all(wnd);
    	
	gtk_main();
    
   	return 0;
}
