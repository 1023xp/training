#include <gtk/gtk.h>

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
	gtk_main_quit ();
	return FALSE;
}

int main(int argc,char *argv[])
{
	GtkWidget *frame;
	GtkWidget *wnd;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *item;
	GtkWidget *list;	
	GList *item_list;

	gtk_init (&argc, &argv);
        wnd= gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
        gtk_window_set_title(GTK_WINDOW(wnd),"p18.4 frame");
        gtk_window_set_default_size(GTK_WINDOW(wnd),300,200);
        gtk_container_set_border_width(GTK_CONTAINER(wnd),15);

	vbox=gtk_vbox_new(TRUE,5);
	gtk_container_add(GTK_CONTAINER(wnd),vbox);

	hbox=gtk_hbox_new(TRUE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,0);

	frame=gtk_frame_new("FRAME 1");
	gtk_box_pack_start(GTK_BOX(hbox),frame,TRUE,TRUE,0);

	frame=gtk_frame_new("FRAME 2");
	gtk_box_pack_start(GTK_BOX(hbox),frame,TRUE,TRUE,0);
	gtk_frame_set_label_align(GTK_FRAME(frame),0.5,0);

	frame=gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(vbox),frame,TRUE,TRUE,0);
	
	list=gtk_list_new();
	item_list=NULL;
	item=gtk_list_item_new_with_label("list 1, one");
	item_list=g_list_append(item_list,item);
	
        item=gtk_list_item_new_with_label("list 2, two");
        item_list=g_list_append(item_list,item);

        item=gtk_list_item_new_with_label("list 3, three");
        item_list=g_list_append(item_list,item);

        item=gtk_list_item_new_with_label("list 4, four");
        item_list=g_list_append(item_list,item);	
	
	gtk_list_insert_items(GTK_LIST(list),item_list,0);

	gtk_container_add(GTK_CONTAINER(frame),list);

        g_signal_connect(G_OBJECT(wnd),"delete_event",G_CALLBACK(release_resource),NULL);

	gtk_widget_show_all(wnd);

	gtk_main();
	
	return 0;
}
