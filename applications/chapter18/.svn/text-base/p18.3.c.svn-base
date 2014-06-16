#include <gtk/gtk.h>

void cb_clicked_handler(GtkWidget *widget,gpointer data)
{
	char *var;
	var=NULL;	

	var=gtk_entry_get_text(GTK_ENTRY(((GtkCombo*)data)->entry));
	
	if(var!=NULL)
		g_print("the choice is %s\n",var);
}

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
	gtk_main_quit ();
	return FALSE;
}

int main(int argc,char *argv[])
{
	GtkWidget *combo1;
	GtkWidget *wnd;
	GtkWidget *vbox;
	GList *list=NULL;
	GtkWidget *btn;	

	gtk_init (&argc, &argv);
        wnd= gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
        gtk_window_set_title(GTK_WINDOW(wnd),"p18.3 combo");
        gtk_window_set_default_size(GTK_WINDOW(wnd),200,100);
        gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(wnd),vbox);

	combo1=gtk_combo_new();
	gtk_box_pack_start(GTK_BOX(vbox),combo1,TRUE,TRUE,0);

	list=g_list_append(list,"first item");
	list=g_list_append(list,"second item");
	list=g_list_append(list,"third item");
	list=g_list_append(list,"fourth item");	
	
	gtk_combo_set_popdown_strings(GTK_COMBO(combo1),list);
	
	btn=gtk_button_new_with_label("what you select");
	gtk_box_pack_start(GTK_BOX(vbox),btn,TRUE,TRUE,0);

	g_signal_connect(G_OBJECT(btn),"clicked",GTK_SIGNAL_FUNC(cb_clicked_handler),(gpointer)combo1);
	
        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);

	gtk_main();
	
	return 0;
}
