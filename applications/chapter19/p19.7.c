#include <gtk/gtk.h>

GtkWidget *label;

static gboolean release_resource( GtkWidget *widget,
                                   GdkEvent  *event,
                                   gpointer   data )
{
        gtk_main_quit ();
        return FALSE;
}

void close_dialog_handler(GtkWidget *widget,gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));	
}

void cb_set_font(GtkWidget *widget,gpointer data)
{
	GdkColor color;
	
	gchar *font_name=gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(data));
	
	PangoFontDescription *font_desc=pango_font_description_from_string(font_name);
	gtk_widget_modify_font(label,font_desc);
	pango_font_description_free(font_desc);	
	
	gtk_widget_destroy(GTK_WIDGET(data));
}

void cb_show_font_dialog(GtkWidget *widget,gpointer data)
{
	GtkWidget *font;
	
	font=gtk_font_selection_dialog_new("choose your prefer font type...");	
	gtk_widget_show(GTK_FONT_SELECTION_DIALOG(font)->apply_button);
	
	g_signal_connect(GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(font)->ok_button),
				"clicked",GTK_SIGNAL_FUNC(cb_set_font),(gpointer)font);
        g_signal_connect(GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(font)->apply_button),
				"clicked",G_CALLBACK(cb_set_font),(gpointer)font);
        g_signal_connect_swapped(GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(font)->cancel_button),
                                "clicked",G_CALLBACK(gtk_widget_destroy),(gpointer)font);

        g_signal_connect(GTK_OBJECT(font),"destroy",
                                G_CALLBACK(close_dialog_handler),(gpointer)font);
	
	gtk_widget_show(font);
}

int main(int argc,char *argv[])
{
        GtkWidget *dialog;
	GtkWidget *btn;

        gtk_init (&argc, &argv);
        
	dialog= gtk_dialog_new();
	gtk_window_set_title(GTK_WINDOW(dialog),"P19.7");
	gtk_widget_set_size_request(dialog,350,150);
	
	label=gtk_label_new("change font type and size");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,FALSE,FALSE,0);

	btn=gtk_button_new_with_label("Font...");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),btn,FALSE,FALSE,0);
       	g_signal_connect(G_OBJECT(btn),"clicked",
                                        GTK_SIGNAL_FUNC(cb_show_font_dialog),NULL);

        g_signal_connect(G_OBJECT(dialog),"delete_event",
					G_CALLBACK(release_resource),NULL);
        gtk_widget_show_all(dialog);
        gtk_main();
        return 0;
}
