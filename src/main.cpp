#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_assert.h>

#include "window.hpp"
#include "utils.hpp"
#include "mouse.hpp"
#include "tiercell.hpp"
#include "tierrow.hpp"
#include "tierboard.hpp"
#include "tierevent.hpp"
#include "text.hpp"
#include "image.hpp"

#include <iostream>
#include <vector>

static struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    // std::vector<Tier::Cell*> cells;
    // std::vector<Tier::Row*> rows;
    Tier::Board board;
    Tier::Cell* clickedCell = nullptr;
    Tier::Row* cellParent = nullptr;
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
    const auto imgTypes = Graphics::imageInit();
    const auto imgTypeNames = Graphics::getImageTypeNames(imgTypes);
    std::cout << "Image types loaded: ";
    for (const auto& type : imgTypeNames)
    {
        std::cout << type << ", ";
    }
    std::cout << std::endl;
    Tier::initEvents();
    state.window = Window::createWindow({800, 600});
    state.renderer = Window::getRenderer();
    Tier::Row* row = new Tier::Row();
    row->setColor({230,220,21});
    row->setText("This Is The Very First Row");
    row->setTextSize(15);
    Tier::Cell* child1 = new Tier::Cell();
    child1->setBackgroundColor({0, 255, 0});
    child1->setFontColor({0, 0, 0, 255});
    child1->setText("BOX 1");
    child1->setFontSize(22);
    Tier::Cell* child2 = new Tier::Cell();
    child2->setBackgroundColor({128, 128, 0});
    child2->setFontColor({0, 0, 0, 255});
    child2->setText("BOX 2");
    child2->setFontSize(12);
    Tier::Cell* child3 = new Tier::Cell();
    child3->setBackgroundColor({255, 255, 0});
    child3->setFontColor({0, 0, 0, 255});
    child3->setText("BOX 3");
    child3->setFontSize(12);
    row->addChild(child1);
    row->addChild(child2);
    row->addChild(child3);
    state.board.addChild(row);
    // auto& row2 = state.rows.at(1);
    Tier::Row* row2 = new Tier::Row();
    row2->setColor({210,22,230});
    row2->setText("Second Row");
    row2->setTextSize(13);
    Tier::Cell* child21 = new Tier::Cell();
    child21->setBackgroundColor({0, 255, 0});
    child21->setFontColor({0, 0, 0, 255});
    child21->setText("BOX 4");
    child21->setFontSize(22);
    Tier::Cell* child22 = new Tier::Cell();
    child22->setBackgroundColor({128, 128, 0});
    child22->setFontColor({0, 0, 0, 255});
    child22->setText("BOX 5");
    child22->setFontSize(12);
    Tier::Cell* child23 = new Tier::Cell();
    child23->setBackgroundColor({255, 255, 0});
    child23->setFontColor({0, 0, 0, 255});
    child23->setText("BOX 6");
    child23->setFontSize(12);
    row2->addChild(child23);
    row2->addChild(child22);
    row2->addChild(child21);
    state.board.addChild(row2);
    state.board.resizeCells({150, 150});
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
    TRY(Window::renderWindow());
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
                if (row->getTitleRect().contains(pos))
                {
                    state.dragItem = row->toDrawing(state.renderer);
                    const auto corner = row->getCorner();
                    state.mousePosition = { pos.x() - corner.x(), pos.y() - corner.y() };
                    state.cellParent = row;
                    row->setHidden(true);
                }
                else
                {
                    for (auto& cell : row->children)
                    {
                        if (!cell->contains(pos))
                            continue;
                        // std::cout << "Child clicked" << std::endl;
                        state.clickedCell = cell;
                        state.cellParent = row;
                        cell->setHidden(true);
                        const auto corner = cell->getCorner();
                        state.mousePosition = { pos.x() - corner.x(), pos.y() - corner.y() };
                        state.dragItem = cell->toDrawing(state.renderer);
                        return SDL_APP_CONTINUE;
                    }
                }
            }
        }
        return SDL_APP_CONTINUE;
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        const auto& mev = event->button;
        const auto point = FPoint(mev.x, mev.y);
        if (mev.button == SDL_BUTTON_LEFT)
        {
            if (state.clickedCell != nullptr)
            {
                state.clickedCell->setHidden(false);
                for (auto row : state.board.children)
                {
                    if (row->contains(point))
                    {
                        const auto index = row->dropIndex(point);
                        // printf("Moving cell %d to row %d index %d\n", state.clickedCell->getId(), state.cellParent->getId(), index);
                        // printf("Row %d has %ld children\n", state.cellParent->getId(), state.cellParent->children.size());
                        Tier::cellMove(state.clickedCell->getId(), row->getId(), index);
                        break;
                    }
                }
            }
            else if (state.cellParent != nullptr)
            {
                state.cellParent->setHidden(false);
            }
            state.clickedCell = nullptr;
            state.cellParent = nullptr;
            state.dragItem = {};
        }
        else if (mev.button == SDL_BUTTON_RIGHT)
        {
            static FSize cellSize = {150, 200};
            cellSize = {cellSize.h(), cellSize.w()};
            Tier::cellResize(cellSize);
        }
        return SDL_APP_CONTINUE;
    }
    else if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        const auto& mev = event->motion;
        if (state.clickedCell == nullptr)
            return SDL_APP_CONTINUE;
        const auto pos = Mouse::getPosition();
    }
    else if (event->type == SDL_EVENT_KEY_UP)
    {
        const auto& kev = event->key;
        if (kev.keysym.sym == SDLK_ESCAPE)
        {
            return SDL_APP_SUCCESS;
        }
    }
    else if (event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        const auto& wev = event->window;
        state.board.arrangeChildren();
    }
    else if (event->type == SDL_EVENT_DROP_FILE)
    {
        const auto& dev = event->drop;
        std::cout << "Drop file " << dev.data << std::endl;
        for (auto row : state.board.children)
        {
            for (auto child : row->children)
            {
                if (child->contains(FPoint(dev.x, dev.y)))
                {
                    child->setImage(dev.data);
                    return SDL_APP_CONTINUE;
                }
            }
        }
    }
    else if (event->type == Tier::eventId(Tier::EventType::cellResize))
    {
        auto& uev = event->user;
        state.board.resizeCells(*static_cast<FSize*>(uev.data1));
        free(uev.data1);
    }
    else if (event->type == Tier::eventId(Tier::EventType::cellMove))
    {
        auto& uev = event->user;
        auto move = static_cast<Tier::EventData::CellMove*>(uev.data1);
        auto cellRow = state.board.findCell(move->cellId);
        auto row = state.board.findRow(move->targetRowId);
        const auto remIndex = cellRow.row->removeChild(cellRow.cell);
        if (row == cellRow.row && move->index > remIndex)
            move->index--;
        row->addChild(cellRow.cell, move->index);
        state.board.arrangeChildren();
        free(move);
    }
    else if (event->type == Tier::eventId(Tier::EventType::cellCreate))
    {
        auto* cell = new Tier::Cell();
        (*state.board.children.rbegin())->addChild(cell);
    }
    else if (event->type == Tier::eventId(Tier::EventType::cellRemove))
    {

    }
    else if (event->type == Tier::eventId(Tier::EventType::rowMove))
    {

    }
    else if (event->type == Tier::eventId(Tier::EventType::rowCreate))
    {

    }
    else if (event->type == Tier::eventId(Tier::EventType::rowDelete))
    {

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
