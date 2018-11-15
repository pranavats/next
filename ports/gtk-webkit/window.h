/*
Copyright © 2018 Atlas Engineer LLC.
Use of this file is governed by the license that can be found in LICENSE.
*/
#define APPNAME "Next"

#include <webkit2/webkit2.h>
#include "buffer.h"

typedef struct {
	Buffer *buffer;
	GtkWidget *base;
	const char *identifier;
} Window;

static void destroy_window(GtkWidget *widget, GtkWidget *window) {
	// TODO: Call only on last window.
	gtk_main_quit();
}

static gboolean close_web_view(WebKitWebView *webView, GtkWidget *window) {
	// TODO: Call window_delete with identifier.
	gtk_widget_destroy(window);
	return true;
}

Window *window_init() {
	Window *window = calloc(1, sizeof (Window));
	// Create an 800x600 window that will contain the browser instance
	window->base = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_default_size(GTK_WINDOW(window->base), 800, 600);
	// TODO: Make title customizable from Lisp.
	gtk_window_set_title(GTK_WINDOW(window->base), APPNAME);
	// TODO: Deprecated?
	/* gtk_window_set_wmclass(GTK_WINDOW(main_window), APPNAME, APPNAME); */

	Buffer *buffer = buffer_init();

	// Put the browser area into the main window
	gtk_container_add(GTK_CONTAINER(window->base), GTK_WIDGET(buffer->web_view));

	// Set up callbacks so that if either the main window or the browser
	// instance is closed, the program will exit
	g_signal_connect(window->base, "destroy", G_CALLBACK(destroy_window), NULL);
	g_signal_connect(buffer->web_view, "close", G_CALLBACK(close_web_view), window->base);

	// Make sure that when the browser area becomes visible, it will get
	// mouse and keyboard events
	gtk_widget_grab_focus(GTK_WIDGET(buffer->web_view));

	// Make sure the main window and all its contents are visible
	gtk_widget_show_all(window->base);

	window->buffer = buffer;
	return window;
}

void window_close(Window *window) {
	gtk_widget_destroy(window->base);
	// TODO: Kill buffer.
}