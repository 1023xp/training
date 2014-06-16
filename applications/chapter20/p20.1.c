#include <gtk/gtk.h>

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
        gtk_main_quit ();
        return FALSE;
}

void cb_active_open(GtkWidget *widget,gpointer data)
{
	g_print("choose open\n");
}

void cb_active_save(GtkWidget *widget,gpointer data)
{
	g_print("choose save\n");
}

void cb_active_close(GtkWidget *widget,gpointer data)
{
	g_print("choose close\n");
}

int main(int argc,char *argv[])
{
        GtkWidget *wnd;
	GtkWidget *menu;
	GtkWidget *menu_bar;
	GtkWidget *menu_item;
	GtkWidget *top_item;
	GtkWidget *vbox;

        gtk_init (&argc, &argv);
        
	wnd= gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(wnd),"P20.1 how to use menu");
	gtk_widget_set_size_request(wnd,350,150);
	gtk_container_set_border_width(GTK_CONTAINER(wnd),5);

	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(wnd),vbox);
	gtk_widget_show(vbox);	
	
	menu_bar=gtk_menu_bar_new();
	gtk_box_pack_start(GTK_BOX(vbox),menu_bar,FALSE,FALSE,2);
	gtk_widget_show(menu_bar);	

	menu=gtk_menu_new();
	menu_item=gtk_menu_item_new_with_label("Open");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menu_item);
	g_signal_connect(G_OBJECT(menu_item),"activate",
				GTK_SIGNAL_FUNC(cb_active_open),NULL);
	gtk_widget_show(menu_item);

       	menu_item=gtk_menu_item_new_with_label("Save"); 
       	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menu_item);
        g_signal_connect(G_OBJECT(menu_item),"activate",
				GTK_SIGNAL_FUNC(cb_active_save),NULL);
        gtk_widget_show(menu_item);

       	menu_item=gtk_menu_item_new_with_label("Close"); 
       	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menu_item);
        g_signal_connect(G_OBJECT(menu_item),"activate",
				GTK_SIGNAL_FUNC(cb_active_close),NULL);
        gtk_widget_show(menu_item);

	top_item=gtk_menu_item_new_with_label("File");
	gtk_widget_show(top_item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(top_item),menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar),top_item);
	
        g_signal_connect(G_OBJECT(wnd),"delete_event",
					G_CALLBACK(release_resource),NULL);
        gtk_widget_show(wnd);
        gtk_main();
        return 0;
}
