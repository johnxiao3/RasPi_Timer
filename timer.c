#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *label1;
GtkWidget *drawingarea1;
GtkBuilder *builder;

int main(int argc, char *argv[]){
	gtk_init(&argc,&argv); // init Gtk
	
	//----------------------------------------------------------------
	// establish contact iwth xml code used to adjust widget settings
	//-----------------------------------------------------------------
	builder = gtk_builder_new_from_file("timer.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder,"window1"));
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_builder_connect_signals(builder,NULL);
	
	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder,"fixed1"));
	drawingarea1 = GTK_WIDGET(gtk_builder_get_object(builder,"drawingarea1"));
	label1 = GTK_WIDGET(gtk_builder_get_object(builder,"label1"));
	
	gtk_widget_show(window);
	
	gtk_main();
	
	
	return 1;
}
