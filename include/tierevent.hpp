#ifndef H_0973679C8D504185A0317B06830C3375
#define H_0973679C8D504185A0317B06830C3375

#include <SDL3/SDL_events.h>
#include "utils.hpp"
#include <type_traits>

namespace Tier
{
    enum class EventType
    {
        cellResize,
        cellCreate,
        cellMove,
        cellRemove,
        rowCreate,
        rowRemove,
        rowDelete,
        rowMove,
    };
    //must be manually updated
    constexpr std::underlying_type_t<EventType> EventTypeCount = 8;
    void initEvents();
    Uint32 eventId(EventType type);
    void cellResize(FSize size);
    void cellMove(uint32_t cellId, uint32_t targetRowId, int index);
    void cellCreate();
    void cellDelete(uint32_t cellId);
    void rowMove(uint32_t rowId, int index);
    void rowCreate();
    void rowDelete(uint32_t rowId);
    namespace EventData
    {
        struct CellMove
        {
            uint32_t cellId;
            uint32_t targetRowId;
            int index;
        };
        struct RowMove
        {
            uint32_t rowId;
            int index;
        };
    }
}

#endif //H_0973679C8D504185A0317B06830C3375
