#include "Sandbox.h"
#include <EntryPoint.h>

void create_user_application()
{
    linux_set_current_stack_size(MB(500), MB(528));

    application_init(2000, 1600, "GameDemo");

    Layer sandboxLayer;
    sandboxLayer.Name = "Game Demo Layer";
    sandboxLayer.OnAttach = sandbox_on_attach;
    sandboxLayer.OnUpdate = sandbox_on_update;
    sandboxLayer.OnUIRender = sandbox_on_ui_render;
    sandboxLayer.OnEvent = sandbox_on_event;
    sandboxLayer.OnDestoy = sandbox_on_destroy;

    application_push_layer(sandboxLayer);
}
