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
#include "../include/constants/map_sec.h"

// enums
enum MapPopUp_Themes
{
    MAPPOPUP_THEME_WOOD,
    MAPPOPUP_THEME_MARBLE,
    MAPPOPUP_THEME_STONE,
    MAPPOPUP_THEME_BRICK,
    MAPPOPUP_THEME_UNDERWATER,
    MAPPOPUP_THEME_STONE2,
    //MAPPOPUP_THEME_GRASS,
    //MAPPOPUP_THEME_SCREEN,
    NUM_MAPPOPUP_THEMES,
};

#define MAPPOPUP_THEME_GRASS MAPPOPUP_THEME_WOOD
#define MAPPOPUP_THEME_SCREEN MAPPOPUP_THEME_MARBLE

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
    [MAPSEC_PALLET_TOWN ] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_VIRIDIAN_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_PEWTER_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_CERULEAN_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_LAVENDER_TOWN] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_VERMILION_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_CELADON_CITY] = MAPPOPUP_THEME_MARBLE,
    [MAPSEC_FUCHSIA_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_CINNABAR_ISLAND] = MAPPOPUP_THEME_STONE,
    [MAPSEC_INDIGO_PLATEAU] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_SAFFRON_CITY] = MAPPOPUP_THEME_MARBLE,
    [MAPSEC_ROUTE_26] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_27] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_1] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_2] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_3] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_4] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_5] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_6] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_7] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_8] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_9] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_10] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_11] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_12] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_13] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_14] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_15] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_16] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_17] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_18] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_19] = MAPPOPUP_THEME_UNDERWATER,
    [MAPSEC_ROUTE_20] = MAPPOPUP_THEME_UNDERWATER,
    [MAPSEC_ROUTE_21] = MAPPOPUP_THEME_UNDERWATER,
    [MAPSEC_ROUTE_22] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_23] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_24] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_25] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_VIRIDIAN_FOREST] = MAPPOPUP_THEME_GRASS,
    [MAPSEC_MT_MOON] = MAPPOPUP_THEME_STONE,
    [MAPSEC_ROUTE_28] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_UNDERGROUND_PATH] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_PAL_PARK] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_DIGLETTS_CAVE] = MAPPOPUP_THEME_STONE,
    [MAPSEC_VICTORY_ROAD] = MAPPOPUP_THEME_STONE,
    [MAPSEC_ROCKET_HIDEOUT] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_SILPH_CO] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_TOHJO_FALLS] = MAPPOPUP_THEME_STONE,
    [MAPSEC_SAFARI_ZONE] = MAPPOPUP_THEME_GRASS,
    [MAPSEC_POKEMON_LEAGUE] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_ROCK_TUNNEL] = MAPPOPUP_THEME_STONE,
    [MAPSEC_SEAFOAM_ISLANDS] = MAPPOPUP_THEME_STONE,
    [MAPSEC_KANTO_RADIO_TOWER] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_CERULEAN_CAVE] = MAPPOPUP_THEME_STONE,
    [MAPSEC_POWER_PLANT] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_GOLDENROD_CITY] = MAPPOPUP_THEME_MARBLE,
    [MAPSEC_CHERRYGROVE_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_VIOLET_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_AZALEA_TOWN] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_BLACKTHORN_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_ECRUTEAK_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_OLIVINE_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_CIANWOOD_CITY] = MAPPOPUP_THEME_BRICK,
    [MAPSEC_MAHOGANY_TOWN] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_NEW_BARK_TOWN] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_SAFARI_ZONE_GATE] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_46] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_47] = MAPPOPUP_THEME_UNDERWATER,
    [MAPSEC_ROUTE_48] = MAPPOPUP_THEME_UNDERWATER,
    [MAPSEC_ROUTE_29] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_30] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_31] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_32] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_33] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_34] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_35] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_36] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_37] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_38] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_39] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_40] = MAPPOPUP_THEME_UNDERWATER,
    [MAPSEC_ROUTE_41] = MAPPOPUP_THEME_UNDERWATER,
    [MAPSEC_ROUTE_42] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_43] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_44] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_ROUTE_45] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_GOLDENROD_TUNNEL] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_DARK_CAVE] = MAPPOPUP_THEME_STONE,
    [MAPSEC_SPROUT_TOWER] = MAPPOPUP_THEME_STONE,
    [MAPSEC_RUINS_OF_ALPH] = MAPPOPUP_THEME_STONE,
    [MAPSEC_UNION_CAVE] = MAPPOPUP_THEME_STONE,
    [MAPSEC_BATTLE_TOWER] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_SLOWPOKE_WELL] = MAPPOPUP_THEME_STONE,
    [MAPSEC_ILEX_FOREST] = MAPPOPUP_THEME_GRASS,
    [MAPSEC_RADIO_TOWER] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_LAKE_OF_RAGE] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_DRAGONS_DEN] = MAPPOPUP_THEME_STONE,
    [MAPSEC_LAKE_OFRAGE_2] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_Mt_SILVER] = MAPPOPUP_THEME_STONE,
    [MAPSEC_SILVER_CAVE] = MAPPOPUP_THEME_STONE,
    [MAPSEC_NATIONAL_PARK] = MAPPOPUP_THEME_WOOD,
    [MAPSEC_BELL_TOWER] = MAPPOPUP_THEME_STONE,
    [MAPSEC_BURNED_TOWER] = MAPPOPUP_THEME_STONE,
    [MAPSEC_GLITTER_LIGHTHOUSE] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_WHIRL_ISLANDS] = MAPPOPUP_THEME_STONE,
    [MAPSEC_MT_MORTAR] = MAPPOPUP_THEME_STONE,
    [MAPSEC_TEAM_ROCKET_HQ] = MAPPOPUP_THEME_SCREEN,
    [MAPSEC_ICE_PATH] = MAPPOPUP_THEME_STONE,
    [MAPSEC_CELADON_DEPART] = MAPPOPUP_THEME_SCREEN,
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
            sMapNamePopupWindowId = 0xFF; // initialize window id to 0xFF in case
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
    x = 40 - (GetStringCenterAlignXOffset(0, withoutPrefixPtr, -1) / 2);
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
    popUpThemeId = sRegionMapSectionId_To_PopUpThemeIdMapping[regionMapSectionId];

    LoadBgTiles(GetWindowAttribute(popupWindowId, 0), sMapPopUp_OutlineTable[popUpThemeId], 0x400, 0x21D);
    CallWindowFunction(popupWindowId, DrawMapNamePopUpFrame);
    PutWindowTilemap(popupWindowId);
    LoadPalette(sMapPopUp_PaletteTable[popUpThemeId], BG_PLTT_ID(14), sizeof(sMapPopUp_PaletteTable[0]));
    BlitBitmapToWindow(popupWindowId, sMapPopUp_Table[popUpThemeId], 0, 0, 80, 24);
}
