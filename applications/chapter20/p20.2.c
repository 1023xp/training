#include <gtk/gtk.h>

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
        gtk_main_quit ();
        return FALSE;
}

void cb_button_click(GtkWidget *widget,gpointer data)
{
        g_print("%s\n",data);
}

GtkWidget *convert_xpm_format(GtkWidget *wnd,gchar *xpm_file)
{
	GtkWidget *pix_gtkwidget;
	GdkPixmap *pix;
	GdkBitmap *mask;
	GtkStyle *style;

	style=gtk_widget_get_style(wnd);
	pix=gdk_pixmap_create_from_xpm(wnd->window,&mask, 
			&style->bg[GTK_STATE_NORMAL],xpm_file);
	pix_gtkwidget=gtk_pixmap_new(pix,mask);
	gtk_widget_show(pix_gtkwidget);
	
	return(pix_gtkwidget);
}

int main(int argc,char *argv[])
{
        GtkWidget *wnd;
	GtkWidget *toolbar;
	GtkWidget *vbox;

        gtk_init (&argc, &argv);
        
	wnd= gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"P20.2 how to use toolbar");
	gtk_widget_set_size_request(wnd,350,150);
	gtk_container_set_border_width(GTK_CONTAINER(wnd),5);
	gtk_widget_show(wnd);

	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(wnd),vbox);
	gtk_widget_show(vbox);	
	
        toolbar=gtk_toolbar_new();
        gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar),GTK_ORIENTATION_HORIZONTAL);
        gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_ICONS);

	gtk_box_pack_start(GTK_BOX(vbox),toolbar,FALSE,FALSE,2);
        gtk_widget_show(toolbar);
	gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"click","click button",NULL,
                                        convert_xpm_format(vbox,"check-n.xpm"),
                                        (GtkSignalFunc)cb_button_click,"press the button");

        gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"click 2","click button 2",NULL,
                                        convert_xpm_format(vbox,"marble.xpm"),
                                        (GtkSignalFunc)cb_button_click,"press the button 2");



        g_signal_connect(G_OBJECT(wnd),"delete_event",
					G_CALLBACK(release_resource),NULL);
        gtk_main();
        return 0;
}
