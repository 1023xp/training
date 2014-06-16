#include <gtk/gtk.h>

static gboolean release_resource( GtkWidget *widget,GdkEvent  *event,gpointer   data )
{
  gtk_main_quit ();
  return FALSE;
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *wnd;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *button;
    GtkWidget *separator;
    GSList *group;
  
    gtk_init (&argc, &argv);    
      
    wnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(wnd),200,200);
    gtk_window_set_title (GTK_WINDOW (wnd), "radio buttons");
    gtk_container_set_border_width (GTK_CONTAINER (wnd), 0);

    box1 = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (wnd), box1);

    box2 = gtk_vbox_new (FALSE, 10);
    gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
    gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);

    button = gtk_radio_button_new_with_label (NULL, "radio button1");
    gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);

    group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));
    button = gtk_radio_button_new_with_label (group, "radio button2");
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
    gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);

    group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));
    button = gtk_radio_button_new_with_label (group, "radio button3");
    gtk_box_pack_start (GTK_BOX(box2), button, TRUE, TRUE, 0);

    separator = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);

    box2 = gtk_vbox_new (FALSE, 10);
    gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, TRUE, 0);
    gtk_widget_show (box2);

    button = gtk_check_button_new_with_label("check button1");
    gtk_container_add(GTK_CONTAINER(box2),button);

    button = gtk_check_button_new_with_label("check button2");
    gtk_container_add(GTK_CONTAINER(box2),button);

    button = gtk_check_button_new_with_label("check button3");
    gtk_container_add(GTK_CONTAINER(wnd),button);

    g_signal_connect(G_OBJECT (wnd), "delete_event",
                      G_CALLBACK(release_resource),
                      NULL);

    gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
    
    gtk_widget_show_all(wnd);    
 
    gtk_main ();

    return 0;
}
