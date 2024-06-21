#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_timer.h>

#include "window.hpp"
#include "utils.hpp"
#include "mouse.hpp"
#include "tiercell.hpp"
#include "tierrow.hpp"
#include "tierboard.hpp"
#include "text.hpp"

#include <iostream>
#include <vector>

Uint32 cellResizeEventType;
void cellResize(FSize size)
{
    SDL_UserEvent userEvent
    {
        .type = cellResizeEventType,
        .timestamp = SDL_GetTicksNS(),
        .data1 = new FSize(size),
    };
    SDL_Event event;
    event.user = userEvent;
    SDL_PushEvent(&event);
}

static struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    // std::vector<Tier::Cell*> cells;
    // std::vector<Tier::Row*> rows;
    Tier::Board board;
    Tier::Cell* clickedCell = nullptr;
    Graphics::Drawing dragItem;
    FPoint mousePosition;
} state;

int SDL_AppInit(void **appstate, int argc, char **argv)
{
    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (Text::init("../assets/fonts/Roboto-Regular.ttf"))
    {
        std::cerr << "Text init error: " << SDL_GetError() << std::endl;
        return SDL_APP_FAILURE;
    }
    Uint32 cellResizeEventType = SDL_RegisterEvents(1);
    state.window = Window::createWindow();
    state.renderer = Window::getRenderer();
    // Text::setFont("../assets/fonts/Roboto-Regular.ttf", 12);
    // state.cells.emplace_back(new Tier::Cell({300, 300, 100, 100}));
    // state.cells.at(0)->setBackgroundColor({0, 255, 0});
    // state.cells.at(0)->setFontColor({0, 0, 0, 255});
    // state.cells.at(0)->setText("A BOX");
    // state.cells.emplace_back(new Tier::Cell({200, 200, 100, 100}));
    // state.cells.at(1)->setBackgroundColor({128, 128, 0});
    // state.cells.at(1)->setFontColor({0, 0, 0, 255});
    // state.cells.at(1)->setText("A SECOND BOX");
    // state.rows.emplace_back(new Tier::Row());
    Tier::Row* row = new Tier::Row();
    Tier::Cell* child1 = new Tier::Cell({300, 300, 100, 100});
    child1->setBackgroundColor({0, 255, 0});
    child1->setFontColor({0, 0, 0, 255});
    child1->setText("A BOX");
    child1->setFontSize(22);
    Tier::Cell* child2 = new Tier::Cell({200, 200, 100, 100});
    child2->setBackgroundColor({128, 128, 0});
    child2->setFontColor({0, 0, 0, 255});
    child2->setText("A SECOND BOX");
    child2->setFontSize(12);
    Tier::Cell* child3 = new Tier::Cell({200, 200, 100, 100});
    child3->setBackgroundColor({255, 255, 0});
    child3->setFontColor({0, 0, 0, 255});
    child3->setText("A THIRD BOX");
    child3->setFontSize(12);
    row->addChild(child1);
    row->addChild(child2);
    row->addChild(child3);
    state.board.addChild(row);
    // auto& row2 = state.rows.at(1);
    Tier::Row* row2 = new Tier::Row();
    Tier::Cell* child21 = new Tier::Cell({300, 300, 100, 100});
    child21->setBackgroundColor({0, 255, 0});
    child21->setFontColor({0, 0, 0, 255});
    child21->setText("A BOX");
    child21->setFontSize(22);
    Tier::Cell* child22 = new Tier::Cell({200, 200, 100, 100});
    child22->setBackgroundColor({128, 128, 0});
    child22->setFontColor({0, 0, 0, 255});
    child22->setText("A SECOND BOX");
    child22->setFontSize(12);
    Tier::Cell* child23 = new Tier::Cell({200, 200, 100, 100});
    child23->setBackgroundColor({255, 255, 0});
    child23->setFontColor({0, 0, 0, 255});
    child23->setText("A THIRD BOX");
    child23->setFontSize(12);
    row2->addChild(child21);
    row2->addChild(child22);
    row2->addChild(child23);
    state.board.addChild(row2);
    // state.rows.at(1)->setCorner({0, row->getSize().h()});
    return SDL_APP_CONTINUE;
}

int SDL_AppIterate(void *appstate)
{
    Window::fillWindow();
    auto renderer = Window::getRenderer();
    // for (auto& row : state.rows)
    // {
    //     row->draw(Window::getRenderer());
    // }
    state.board.draw(state.renderer);
    const auto pos = Mouse::getPosition();
    state.dragItem.drawAt(renderer, {pos.x() - state.mousePosition.x(), pos.y() - state.mousePosition.y()});
    if (Window::renderWindow())
    {
        throw(SDL_GetError());
    }
    SDL_DelayNS(1000*1000*1000 / 60);
    return SDL_APP_CONTINUE;
}

int SDL_AppEvent(void *appstate, const SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        const auto& mev = event->button;
        if (mev.button == SDL_BUTTON_LEFT)
        {
            const auto pos = Mouse::getPosition();
            for (auto& row : state.board.children)
            {
                for (auto& cell : row->children)
                {
                    if (!cell->contains(pos))
                        continue;
                    // std::cout << "Child clicked" << std::endl;
                    state.clickedCell = cell;
                    cell->setHidden(true);
                    const auto corner = cell->getCorner();
                    state.mousePosition = { pos.x() - corner.x(), pos.y() - corner.y() };
                    state.dragItem = cell->toDrawing(state.renderer);
                    return SDL_APP_CONTINUE;
                }
            }
        }
        return SDL_APP_CONTINUE;
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        const auto& mev = event->button;
        
        if (mev.button == SDL_BUTTON_LEFT)
        {
            if (state.clickedCell != nullptr)
                state.clickedCell->setHidden(false);
            state.clickedCell = nullptr;
            state.dragItem = Graphics::Drawing();
        }
        else if (mev.button == SDL_BUTTON_RIGHT)
        {
            for (auto& row : state.board.children)
            {
                row->arrangeChildren();
            }
        }
        return SDL_APP_CONTINUE;
    }
    else if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        const auto& mev = event->motion;
        if (state.clickedCell == nullptr)
            return SDL_APP_CONTINUE;
        // std::cout << "Motion Event" << std::endl;
        const auto pos = Mouse::getPosition();
        // state.clickedCell->moveTo({mev.x - state.mousePosition.x(), mev.y - state.mousePosition.y()});
        // state.mousePosition = {mev.x, mev.y};
    }
    else if (event->type == SDL_EVENT_KEY_UP)
    {
        const auto& kev = event->key;
        if (kev.keysym.sym == SDLK_ESCAPE)
        {
            return SDL_APP_SUCCESS;
        }
    }
    else if (event->type == cellResizeEventType)
    {
        auto& uev = event->user;
        state.board.resizeCells(*static_cast<FSize*>(uev.data1));
        free(uev.data1);
    }
    else
    {
        return SDL_APP_CONTINUE;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate)
{
    Window::destroyWindow();
}
