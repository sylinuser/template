#include "../include/global.h"
#include "../include/alloc.h"
#include "../include/battle.h"
#include "../include/event_data.h"
#include "../include/main.h"
#include "../include/overworld.h"
#include "../include/palette.h"
#include "../include/pokemon.h"
#include "../include/save.h"
#include "../include/sound.h"
#include "../include/sprite.h"
#include "../include/string_util.h"
#include "../include/task.h"
#include "../include/window.h"
#include "../include/constants/moves.h"

// enums
enum MapPopUp_Themes
{
    MAPPOPUP_THEME_WOOD,
    MAPPOPUP_THEME_MARBLE,
    MAPPOPUP_THEME_STONE,
    MAPPOPUP_THEME_BRICK,
    MAPPOPUP_THEME_UNDERWATER,
    MAPPOPUP_THEME_STONE2,
    NUM_MAPPOPUP_THEMES,
};

// static functions
void Task_MapNamePopUpWindow(u8 taskId);
static void ShowMapNamePopUpWindow(void);
static void LoadMapNamePopUpWindowBg(void);
void ShowMapNamePopup(void);
void HideMapNamePopUpWindow(void);
u8 GetMapNamePopUpWindowId(void);

// EWRAM
#define sPopupTaskId *(u8 *)(0x0203FF3F)
#define sMapNamePopupWindowId *(u8 *)(0x0203FF3E)
#define sStartMenuWindowId *(u8 *)(0x0203abe0)
#define TEXT_SKIP_DRAW 0xFF

extern const u8 sMapPopUp_Table[NUM_MAPPOPUP_THEMES][960];
extern const u8 sMapPopUp_OutlineTable[NUM_MAPPOPUP_THEMES][960];
extern const u16 sMapPopUp_PaletteTable[NUM_MAPPOPUP_THEMES][16];

static const u8 sRegionMapSectionId_To_PopUpThemeIdMapping[] =
{
    [0] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_OLDALE_TOWN] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_DEWFORD_TOWN] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_LAVARIDGE_TOWN] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_FALLARBOR_TOWN] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_VERDANTURF_TOWN] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_PACIFIDLOG_TOWN] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_PETALBURG_CITY] = MAPPOPUP_THEME_BRICK,
    //[MAPSEC_SLATEPORT_CITY] = MAPPOPUP_THEME_MARBLE,
    //[MAPSEC_MAUVILLE_CITY] = MAPPOPUP_THEME_MARBLE,
    //[MAPSEC_RUSTBORO_CITY] = MAPPOPUP_THEME_MARBLE,
    //[MAPSEC_FORTREE_CITY] = MAPPOPUP_THEME_BRICK,
    //[MAPSEC_LILYCOVE_CITY] = MAPPOPUP_THEME_MARBLE,
    //[MAPSEC_MOSSDEEP_CITY] = MAPPOPUP_THEME_BRICK,
    //[MAPSEC_SOOTOPOLIS_CITY] = MAPPOPUP_THEME_MARBLE,
    //[MAPSEC_EVER_GRANDE_CITY] = MAPPOPUP_THEME_BRICK,
    //[MAPSEC_ROUTE_101] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_102] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_103] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_104] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_105] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_106] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_107] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_108] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_109] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_110] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_111] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_112] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_113] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_114] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_115] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_116] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_117] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_118] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_119] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_120] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_121] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_122] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_123] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ROUTE_124] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_125] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_126] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_127] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_128] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_129] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_130] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_131] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_132] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_133] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_ROUTE_134] = MAPPOPUP_THEME_UNDERWATER,
    //[MAPSEC_UNDERWATER_124] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_UNDERWATER_126] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_UNDERWATER_127] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_UNDERWATER_128] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_UNDERWATER_SOOTOPOLIS] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_GRANITE_CAVE] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_MT_CHIMNEY] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_SAFARI_ZONE] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_BATTLE_FRONTIER] = MAPPOPUP_THEME_MARBLE,
    //[MAPSEC_PETALBURG_WOODS] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_RUSTURF_TUNNEL] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_ABANDONED_SHIP] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_NEW_MAUVILLE] = MAPPOPUP_THEME_MARBLE,
    //[MAPSEC_METEOR_FALLS] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_METEOR_FALLS2] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_MT_PYRE] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_AQUA_HIDEOUT_OLD] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_SHOAL_CAVE] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_SEAFLOOR_CAVERN] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_UNDERWATER_SEAFLOOR_CAVERN] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_VICTORY_ROAD] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_MIRAGE_ISLAND] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_CAVE_OF_ORIGIN] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_SOUTHERN_ISLAND] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_FIERY_PATH] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_FIERY_PATH2] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_JAGGED_PASS] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_JAGGED_PASS2] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_SEALED_CHAMBER] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_UNDERWATER_SEALED_CHAMBER] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_SCORCHED_SLAB] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_ISLAND_CAVE] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_DESERT_RUINS] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_ANCIENT_TOMB] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_INSIDE_OF_TRUCK] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_SKY_PILLAR] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_SECRET_BASE] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_DYNAMIC] = MAPPOPUP_THEME_MARBLE,
    //[MAPSEC_AQUA_HIDEOUT - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_MAGMA_HIDEOUT - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_MIRAGE_TOWER - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_BIRTH_ISLAND - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_FARAWAY_ISLAND - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_WOOD,
    //[MAPSEC_ARTISAN_CAVE - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_MARINE_CAVE - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_UNDERWATER_MARINE_CAVE - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_TERRA_CAVE - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_UNDERWATER_105 - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_UNDERWATER_125 - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_UNDERWATER_129 - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE2,
    //[MAPSEC_DESERT_UNDERPASS - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_ALTERING_CAVE - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_NAVEL_ROCK - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_STONE,
    //[MAPSEC_TRAINER_HILL - KANTO_MAPSEC_COUNT] = MAPPOPUP_THEME_MARBLE
};





u16 AddWindowParameterized(u8 bg, u8 left, u8 top, u8 width, u8 height, u8 paletteNum, u16 baseBlock)
{
    struct WindowTemplate template;
    SetWindowTemplateFields(&template, bg, left, top, width, height, paletteNum, baseBlock);
    return AddWindow(&template);
}

void InitStandardTextBoxWindows(void)
{
    sStartMenuWindowId = 0xFF;
    sMapNamePopupWindowId = 0xFF;
    InitWindows(0x0841f42c);
    MapNamePopupWindowIdSetDummy();
}






u8 AddMapNamePopUpWindow(void)
{
    if (sMapNamePopupWindowId == 0xFF)
        sMapNamePopupWindowId = AddWindowParameterized(0, 1, 1, 10, 3, 14, 0x107);
    return sMapNamePopupWindowId;
}

u8 GetMapNamePopUpWindowId(void)
{
    return sMapNamePopupWindowId;
}

void RemoveMapNamePopUpWindow(void)
{
    if (sMapNamePopupWindowId != 0xFF)
    {
        RemoveWindow(sMapNamePopupWindowId);
        sMapNamePopupWindowId = 0xFF;
    }
}




#define GPU_REG_BUF_SIZE 0x60

u8 sGpuRegBuffer[GPU_REG_BUF_SIZE];
#define sGpuRegBufferLocked *(u8 *)(0x030000c0)
u8 sGpuRegWaitingList[GPU_REG_BUF_SIZE];

#define GPU_REG_BUF(offset) (*(u16 *)(&sGpuRegBuffer[offset]))
#define GPU_REG(offset) (*(vu16 *)(REG_BASE + offset))

void SetGpuReg_ForcedBlank(u8 regOffset, u16 value)
{
    if (regOffset < GPU_REG_BUF_SIZE)
    {
        GPU_REG_BUF(regOffset) = value;

        if (REG_DISPCNT & DISPCNT_FORCED_BLANK)
        {
            CopyBufferedValueToGpuReg(regOffset);
        }
        else
        {
            s32 i;

            sGpuRegBufferLocked = TRUE;

            for (i = 0; i < GPU_REG_BUF_SIZE && sGpuRegWaitingList[i] != 0xFF; i++)
            {
                if (sGpuRegWaitingList[i] == regOffset)
                {
                    sGpuRegBufferLocked = FALSE;
                    return;
                }
            }

            sGpuRegWaitingList[i] = regOffset;
            sGpuRegBufferLocked = FALSE;
        }
    }
}






static bool8 StartMenu_ShowMapNamePopup(void)
{
    HideStartMenu();
    ShowMapNamePopup();
    return TRUE;
}

// States and data defines for Task_MapNamePopUpWindow
enum {
    STATE_SLIDE_IN,
    STATE_WAIT,
    STATE_SLIDE_OUT,
    STATE_UNUSED,
    STATE_ERASE,
    STATE_END,
    STATE_PRINT, // For some reason the first state is numerically last.
};

#define POPUP_OFFSCREEN_Y  40
#define POPUP_SLIDE_SPEED  2

#define tState         data[0]
#define tOnscreenTimer data[1]
#define tYOffset       data[2]
#define tIncomingPopUp data[3]
#define tPrintTimer    data[4]

#define FLAG_HIDE_MAP_NAME_POPUP 0x4000

void ShowMapNamePopup(void)
{
    if (FlagGet(FLAG_HIDE_MAP_NAME_POPUP) != TRUE)
    {
        if (!FuncIsActiveTask(0x080981ac | 1)) // Task_MapNamePopUpWindow
        {
            // New pop up window
            sPopupTaskId = CreateTask(0x080981ac | 1, 90);
            SetGpuReg(REG_OFFSET_BG0VOFS, POPUP_OFFSCREEN_Y);
            gTasks[sPopupTaskId].tState = STATE_PRINT;
            gTasks[sPopupTaskId].tYOffset = POPUP_OFFSCREEN_Y;
        }
        else
        {
            // There's already a pop up window running.
            // Hurry the old pop up offscreen so the new one can appear.
            if (gTasks[sPopupTaskId].tState != STATE_SLIDE_OUT)
                gTasks[sPopupTaskId].tState = STATE_SLIDE_OUT;
            gTasks[sPopupTaskId].tIncomingPopUp = TRUE;
        }
    }
}

void Task_MapNamePopUpWindow(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case STATE_PRINT:
        // Wait, then create and print the pop up window
        if (++task->tPrintTimer > 30)
        {
            task->tState = STATE_SLIDE_IN;
            task->tPrintTimer = 0;
            ShowMapNamePopUpWindow();
        }
        break;
    case STATE_SLIDE_IN:
        // Slide the window onscreen.
        task->tYOffset -= POPUP_SLIDE_SPEED;
        if (task->tYOffset <= 0 )
        {
            task->tYOffset = 0;
            task->tState = STATE_WAIT;
            gTasks[sPopupTaskId].tOnscreenTimer = 0;
        }
        break;
    case STATE_WAIT:
        // Wait while the window is fully onscreen.
        if (++task->tOnscreenTimer > 120)
        {
            task->tOnscreenTimer = 0;
            task->tState = STATE_SLIDE_OUT;
        }
        break;
    case STATE_SLIDE_OUT:
        // Slide the window offscreen.
        task->tYOffset += POPUP_SLIDE_SPEED;
        if (task->tYOffset >= POPUP_OFFSCREEN_Y)
        {
            task->tYOffset = POPUP_OFFSCREEN_Y;
            if (task->tIncomingPopUp)
            {
                // A new pop up window is incoming,
                // return to the first state to show it.
                task->tState = STATE_PRINT;
                task->tPrintTimer = 0;
                task->tIncomingPopUp = FALSE;
            }
            else
            {
                task->tState = STATE_ERASE;
                return;
            }
        }
        break;
    case STATE_ERASE:
        //ClearStdWindowAndFrame(GetMapNamePopUpWindowId(), TRUE);
        task->tState = STATE_END;
        break;
    case STATE_END:
        HideMapNamePopUpWindow();
        return;
    }
    SetGpuReg(REG_OFFSET_BG0VOFS, task->tYOffset);
}

void HideMapNamePopUpWindow(void)
{
    if (FuncIsActiveTask(0x080981ac | 1))
    {
        if (GetMapNamePopUpWindowId() != 0xFF)
        {
            ClearStdWindowAndFrame(GetMapNamePopUpWindowId(), TRUE);
            RemoveMapNamePopUpWindow();
        }
        SetGpuReg_ForcedBlank(REG_OFFSET_BG0VOFS, 0);
        DestroyTask(sPopupTaskId);
    }
}

static void ShowMapNamePopUpWindow(void)
{
    u8 mapDisplayHeader[24];
    u8 *withoutPrefixPtr;
    u8 x;
    const u8 *mapDisplayHeaderSource;

    withoutPrefixPtr = &(mapDisplayHeader[3]);
    GetMapName(withoutPrefixPtr, gMapHeader.regionMapSectionId, 0);
    AddMapNamePopUpWindow();
    LoadMapNamePopUpWindowBg();
    x = GetStringCenterAlignXOffset(0, withoutPrefixPtr, 80);
    mapDisplayHeader[0] = 0xFC;
    mapDisplayHeader[1] = 0x02;
    mapDisplayHeader[2] = 0x00;
    AddTextPrinterParameterized(GetMapNamePopUpWindowId(), 0, mapDisplayHeader, x, 3, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(GetMapNamePopUpWindowId(), COPYWIN_FULL);
}

#define TILE_TOP_EDGE_START 0x21D
#define TILE_TOP_EDGE_END   0x228
#define TILE_LEFT_EDGE_TOP  0x229
#define TILE_RIGHT_EDGE_TOP 0x22A
#define TILE_LEFT_EDGE_MID  0x22B
#define TILE_RIGHT_EDGE_MID 0x22C
#define TILE_LEFT_EDGE_BOT  0x22D
#define TILE_RIGHT_EDGE_BOT 0x22E
#define TILE_BOT_EDGE_START 0x22F
#define TILE_BOT_EDGE_END   0x23A

static void DrawMapNamePopUpFrame(u8 bg, u8 x, u8 y, u8 deltaX, u8 deltaY, u8 unused)
{
    s32 i;

    // Draw top edge
    for (i = 0; i < 1 + TILE_TOP_EDGE_END - TILE_TOP_EDGE_START; i++)
        FillBgTilemapBufferRect(bg, TILE_TOP_EDGE_START + i, i - 1 + x, y - 1, 1, 1, 14);

    // Draw sides
    FillBgTilemapBufferRect(bg, TILE_LEFT_EDGE_TOP,       x - 1,     y, 1, 1, 14);
    FillBgTilemapBufferRect(bg, TILE_RIGHT_EDGE_TOP, deltaX + x,     y, 1, 1, 14);
    FillBgTilemapBufferRect(bg, TILE_LEFT_EDGE_MID,       x - 1, y + 1, 1, 1, 14);
    FillBgTilemapBufferRect(bg, TILE_RIGHT_EDGE_MID, deltaX + x, y + 1, 1, 1, 14);
    FillBgTilemapBufferRect(bg, TILE_LEFT_EDGE_BOT,       x - 1, y + 2, 1, 1, 14);
    FillBgTilemapBufferRect(bg, TILE_RIGHT_EDGE_BOT, deltaX + x, y + 2, 1, 1, 14);

    // Draw bottom edge
    for (i = 0; i < 1 + TILE_BOT_EDGE_END - TILE_BOT_EDGE_START; i++)
        FillBgTilemapBufferRect(bg, TILE_BOT_EDGE_START + i, i - 1 + x, y + deltaY, 1, 1, 14);
}

static void LoadMapNamePopUpWindowBg(void)
{
    u8 popUpThemeId;
    u8 popupWindowId = GetMapNamePopUpWindowId();
    u16 regionMapSectionId = gMapHeader.regionMapSectionId;

    //if (regionMapSectionId >= KANTO_MAPSEC_START)
    //{
    //    if (regionMapSectionId > KANTO_MAPSEC_END)
    //        regionMapSectionId -= KANTO_MAPSEC_COUNT;
    //    else
    //        regionMapSectionId = 0; // Discard kanto region sections;
    //}
    popUpThemeId = MAPPOPUP_THEME_MARBLE;//sRegionMapSectionId_To_PopUpThemeIdMapping[regionMapSectionId];

    LoadBgTiles(GetWindowAttribute(popupWindowId, 0), sMapPopUp_OutlineTable[popUpThemeId], 0x400, 0x21D);
    CallWindowFunction(popupWindowId, DrawMapNamePopUpFrame);
    PutWindowTilemap(popupWindowId);
    LoadPalette(sMapPopUp_PaletteTable[popUpThemeId], BG_PLTT_ID(14), sizeof(sMapPopUp_PaletteTable[0]));
    BlitBitmapToWindow(popupWindowId, sMapPopUp_Table[popUpThemeId], 0, 0, 80, 24);
}
