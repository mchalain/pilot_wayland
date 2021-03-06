#ifndef __PILOT_WTK_INTERNAL_H__
#define __PILOT_WTK_INTERNAL_H__

struct pilot_surface {
	struct pilot_display *display;
	struct pilot_buffer *buffers[2];
	struct pilot_buffer *onscreenbuffer;
	struct pilot_buffer *offscreenbuffer;
	pilot_bool_t offscreenbufferid:2;
	pilot_bool_t ready:1;
	pilot_bool_t regionning:1;
	uint32_t size;
	uint32_t stride;
	pilot_length_t width;
	pilot_length_t height;
	pilot_pixel_format_t format;
	_pilot_mutex(lock);
	_pilot_cond(cond);
	struct {
		void (*destroy)(struct pilot_surface *surface);
	} action;
	void *platform;
};

struct pilot_buffer {
	struct pilot_surface *surface;
	pilot_rect_t region;
	unsigned long long size;
	void *data;
	pilot_bool_t id:2;
	pilot_bool_t ready:1;
	pilot_bool_t busy:1;
	_pilot_mutex(lock);
	_pilot_cond(cond);
	struct {
		void (*destroy)(struct pilot_buffer *surface);
		int (*paint)(struct pilot_buffer *surface);
	} action;
	void *platform;
};


struct pilot_input
{
	pilot_key_t id;
	struct pilot_display *display;
	struct pilot_window *window;
	void *platform;
};

int
pilot_window_redraw(struct pilot_window *thiz);

int
pilot_widget_redraw(struct pilot_widget *thiz, struct pilot_blit *blit);

/**
 * pilot_surface
 * **/
struct pilot_surface *
pilot_surface_create(struct pilot_display *display, pilot_rect_t rect);
void
pilot_surface_destroy(struct pilot_surface *thiz);
int
pilot_surface_paint(struct pilot_surface *thiz);
int
pilot_surface_lock(struct pilot_surface *thiz, void **image);
int
pilot_surface_unlock(struct pilot_surface *thiz);
int
pilot_surface_flip(struct pilot_surface *thiz);
/**
 * pilot_buffer
 * **/
struct pilot_buffer *
pilot_buffer_create(struct pilot_surface *surface,
				int size, int id);
void
pilot_buffer_destroy(struct pilot_buffer *thiz);
void
pilot_buffer_busy(struct pilot_buffer *thiz, int busy);

/**
 * pilot_input
 * **/
struct pilot_input *
pilot_input_create(struct pilot_display *display);
struct pilot_input *
pilot_inputkeyboard_create(struct pilot_display *display);
struct pilot_input *
pilot_inputmouse_create(struct pilot_display *display);
void
pilot_input_destroy(struct pilot_input *thiz);
int
pilot_input_focus(struct pilot_input *thiz, struct pilot_window *window);

#endif
