
#define _GNU_SOURCE

#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <time.h>
#include <sys/mman.h>

GtkWidget		*window;
GtkWidget		*fixed1;
GtkWidget		*draw1;
GtkBuilder		*builder; 

gboolean	timer_handler();
int		cpuUtil, cpu[100];
void		on_destroy(); 


int main(int argc, char *argv[]) {
	for(int i = 0; i< 100; i++ ) cpu[i] = 0;

	gtk_init(&argc, &argv); // init Gtk

	builder = gtk_builder_new_from_file ("timer.glade");
 
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    gtk_builder_connect_signals(builder, NULL);

	fixed1		= GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	draw1		= GTK_WIDGET(gtk_builder_get_object(builder, "drawingarea1"));

	g_object_unref(builder);

	gtk_window_set_keep_above (GTK_WINDOW(window), TRUE);
	gtk_widget_show(window);
    g_timeout_add_seconds(1, (GSourceFunc) timer_handler, NULL);

	gtk_main();

	return EXIT_SUCCESS;
}
void on_destroy() {
	gtk_main_quit();
}

gboolean timer_handler() {
	static long time1 = 0, time2 = 0;
	static int flag = 0;
	char line[128], dummy[32];

	FILE *p1 = popen("head -1 /proc/stat","r");

	fgets(line,128,p1);
	sscanf(line,"%s %ld", dummy, &time2);
	fclose(p1);
	
	if (! flag) {
		flag = 1;
		time1 = time2;
		return TRUE;
	}

	cpuUtil = time2 - time1;

	
	for (int i = 99; i > 0; i--) cpu[i] = cpu[i-1];

	cpu[0] = cpuUtil/8;
	printf("percent of cpu utilization = %ld%%\n", cpuUtil );
	time1 = time2;
	gtk_widget_queue_draw(draw1);
	return TRUE;
}

gboolean on_drawingarea1_draw (GtkWidget *widget, cairo_t *cr, gpointer data) {

	guint width, height;

	width = gtk_widget_get_allocated_width (widget);   // of draw window
	height = gtk_widget_get_allocated_height (widget); // of draw window

	cairo_set_line_width(cr, 2.0);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // xxx
	cairo_move_to(cr,(double)1,(double)1);
	cairo_line_to(cr,(double)width,(double)1);
	cairo_line_to(cr,(double)width,(double)height);
	cairo_line_to(cr,(double)1,(double)height);
	cairo_line_to(cr,(double)1,(double)1);

	cairo_stroke (cr);
	
	
	for (int i = 0; i < 99; i++) {
		cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // red
		cairo_move_to (cr, (double) ((i * 4) + 10), (double) (2 * (100 - cpu[i]) + 10) );
		cairo_line_to (cr, (double) (((i + 1) * 4) + 10), (double) (2 * (100 - cpu[i + 1]) +10) );
		cairo_stroke (cr);
		}

	return FALSE;
}
			
