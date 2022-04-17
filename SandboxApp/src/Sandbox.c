#include "Sandbox.h"

#include <Utils/SimpleStandardLibrary.h>

#include "WorldRender.h"

#define ImVec2(x, y) (ImVec2) {x, y}
#define ImVec4(x, y, z, w) (ImVec4) {x, y, z, w}

static NativeWindow CurrentWindow;
static ImFont** g_Fonts = NULL;

ImVec2 VisualsSize;

void dockspace_on_ui();

void
sandbox_on_attach(NativeWindow window)
{
    CurrentWindow = window;

    window_set_vsync(5);

    world_render_init(&CurrentWindow);
}

void
sandbox_on_update(f32 timestep)
{
    NativeWindow* window = &CurrentWindow;
    if (window_is_key_pressed(window, KEY_W))
    {
	world_render_camera_move_up(timestep);
    }
    if (window_is_key_pressed(window, KEY_S))
    {
	world_render_camera_move_bot(timestep);
    }
    if (window_is_key_pressed(window, KEY_A))
    {
	world_render_camera_move_left(timestep);
    }
    if (window_is_key_pressed(window, KEY_D))
    {
	world_render_camera_move_right(timestep);
    }

    world_render_flush(timestep);
}

void
sandbox_on_ui_render()
{
}

void
sandbox_on_event(Event* event)
{
    switch (event->Category)
    {

    case KeyCategory:
    {
	if (event->Type != KeyPressed)
	    break;

	KeyPressedEvent* keyEvent = (KeyPressedEvent*) event;
	if (keyEvent->KeyCode == KEY_ESCAPE)
	{
	    application_close();
	    event->IsHandled = 1;
	}
	break;
    }

    case  WindowCategory:
    {
	if (event->Type == WindowShouldBeClosed)
	{
	    application_close();
	}
	break;
    }

    case MouseCategory:
    {
	if (event->Type == MouseScrolled)
	{
	    MouseScrolledEvent* mevent = (MouseScrolledEvent*)event;
	    EditorCamera* editorCamera = world_render_get_camera();

	    const f32 lowerThreshold = 0.1f;
	    if (editorCamera->ZoomLevel >= lowerThreshold)
	    {
		f32 val = 1.0f;
		if (mevent->YOffset < 0.0f)
		{
		    val *= -1;
		}
		editorCamera->ZoomLevel -= 0.25 * val;
	    }

	    editorCamera->ZoomLevel = MINMAX(editorCamera->ZoomLevel, lowerThreshold, 100.0f);
	    update_camera_positions();

	    event->IsHandled = 1;
	}
	break;
    }
    }

    world_render_on_event(event);
}

void
sandbox_on_destroy()
{
    world_render_destroy();
}
