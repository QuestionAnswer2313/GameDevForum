#ifndef EVENT
#define EVENT

#include "Utils/SimpleStandardLibrary.h"

/*
  TODO(vez): remove this code in the future
*/
//Category for events ?
typedef enum EventType
{
    None = 0,
    WindowShouldBeClosed, WindowResized, WindowFocused, WindowUnfocused, WindowMoved, WindowMinimized, WindowMaximized, WindowRestored,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyRepeatPressed, KeyRealeased, KeyTyped,
    CharTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

    EventType_Count
} EventType;

typedef enum EventCategory
{
    NoneCategory = 1 << 0,
    WindowCategory = 1 << 1,
    AppCategory = 1 << 2,
    KeyCategory = 1 << 3,
    MouseCategory = 1 << 4
} EventCategory;

typedef struct Event
{
    i8 IsHandled;
    EventType Type;
    EventCategory Category;
} Event;

typedef struct KeyPressedEvent
{
    Event Base;
    i32 KeyCode;
    i32 ScanCode;
    i32 Action;
    i32 Modificator;
    i32 RepeatCount;
} KeyPressedEvent;

typedef struct CharEvent
{
    Event Base;
    u32 Char;
} CharEvent;

typedef struct KeyReleasedEvent
{
    Event Base;
    i32 KeyCode;
} KeyReleasedEvent;

typedef struct WindowResizedEvent
{
    Event Base;
    i32 Width;
    i32 Height;
} WindowResizedEvent;

typedef struct MouseMovedEvent
{
    Event Base;
    f32 X;
    f32 Y;
} MouseMovedEvent;

typedef struct MouseScrolledEvent
{
    Event Base;
    f64 XOffset;
    f64 YOffset;
} MouseScrolledEvent;

typedef struct MouseButtonEvent
{
    Event Base;
    i32 MouseCode;
    i32 Action;
    i32 Modificator;
} MouseButtonEvent;

// NOTE(vez): for this type event don't use struct
// only Event .Type =  WindowClosedEvent

#endif
