#include <gtk/gtk.h>

int main(int argc,char* argv[])
{
	GtkWidget *wnd;
	gtk_init(&argc,&argv);
	
	wnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(wnd,"P16.2 program");
	gtk_widget_set_usize(wnd,300,300);

	gtk_widget_show(wnd);

	gtk_main();
	return 0;
}
