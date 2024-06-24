#include "tierevent.hpp"
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_assert.h>

namespace Tier
{
    static Uint32 eventOffset;
    void initEvents()
    {
        eventOffset = SDL_RegisterEvents(EventTypeCount);
    }
    Uint32 eventId(EventType type)
    {
        SDL_assert(static_cast<Uint32>(type) < EventTypeCount);
        return eventOffset + static_cast<Uint32>(type);
    }
    void cellResize(FSize size)
    {
        SDL_UserEvent userEvent
        {
            .type = eventId(EventType::cellResize),
            .timestamp = SDL_GetTicksNS(),
            .data1 = new FSize(size),
        };
        SDL_Event event;
        event.user = userEvent;
        SDL_PushEvent(&event);
    }
    void cellMove(uint32_t cellId, uint32_t targetRowId, int index)
    {
        SDL_Event event
        {
            .user = 
            {
                .type = eventId(EventType::cellMove),
                .timestamp = SDL_GetTicksNS(),
                .data1 = new EventData::CellMove(cellId, targetRowId, index),
            },
        };
        SDL_PushEvent(&event);
    }
    void cellCreate()
    {
        SDL_Event event
        {
            .user = 
            {
                .type = eventId(EventType::cellCreate),
            }
        };
        SDL_PushEvent(&event);
    }
    void cellDelete(uint32_t cellId)
    {

    }
    void rowMove(uint32_t rowId, int index)
    {

    }
    void rowCreate()
    {

    }
    void rowDelete(uint32_t rowId)
    {

    }
}
