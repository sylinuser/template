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
#include "../include/trainer_card.h"
#include "../include/task.h"
#include "../include/window.h"
#include "../include/constants/moves.h"
#include "../include/constants/species.h"

#define LAND_WILD_COUNT     12
#define WATER_WILD_COUNT    5
#define ROCK_WILD_COUNT     5
#define FISH_WILD_COUNT     10

#define NUM_ALTERING_CAVE_TABLES 9

struct WildPokemon
{
    u8 minLevel;
    u8 maxLevel;
    u16 species;
};

struct WildPokemonInfo
{
    u8 encounterRate;
    const struct WildPokemon *wildPokemon;
};

struct WildPokemonHeader
{
    u8 mapGroup;
    u8 mapNum;
    const struct WildPokemonInfo *landMonsInfo;
    const struct WildPokemonInfo *waterMonsInfo;
    const struct WildPokemonInfo *rockSmashMonsInfo;
    const struct WildPokemonInfo *fishingMonsInfo;
};

bool32 IsSpeciesInEncounterTable(const struct WildPokemonInfo * info, s32 species, s32 count)
{
    if ((u32)info & 0x02000000)
        return FALSE;
    s32 i;
    if (info != NULL)
    {
        for (i = 0; i < count; i++)
        {
            if (info->wildPokemon[i].species == species)
                return TRUE;
        }
    }
    return FALSE;
}

u16 GetKantoPokedexCount(u8 caseID)
{
    u16 count = 0;
    u16 i;

    for (i = 1; i <= 256; i++)
    {
        switch (caseID)
        {
        case 0:
            if (GetSetPokedexFlag(i, 0))
                count++;
            break;
        case 1:
            if (GetSetPokedexFlag(i, 1))
                count++;
            break;
        }
    }
    return count;
}

bool16 HasAllKantoMons(void)
{
    return GetKantoPokedexCount(1) == 256;
}

u16 DexScreen_GetDexCount(u8 caseId, bool8 whichDex)
{
    u16 count = 0;
    u16 i;

    switch (whichDex)
    {
    case 0: // Kanto
        for (i = 1; i <= 256; i++)
        {
            if (GetSetPokedexFlag(i, caseId))
                count++;
        }
        break;
    case 1: // National
        for (i = 0; i < 386; i++)
        {
            if (GetSetPokedexFlag(i + 1, caseId))
                count++;

        }
        break;
    }
    return count;
}

u8 GetTrainerStarCount(struct TrainerCard *trainerCard)
{
    u8 stars = 0;

    // has debuted in HoF at all, can keep this as-is i think
    if (trainerCard->rse.hofDebutHours != 0 || trainerCard->rse.hofDebutMinutes != 0 || trainerCard->rse.hofDebutSeconds != 0)
        stars++;

    // has beaten red i believe this is
    if (FlagGet(0x844))
        stars++;

    // has captured the first 256 mons
    if (HasAllKantoMons())
        stars++;

    // has beaten rematch battle tower either single and double
    if (VarGet(0x43CF) > 100 || VarGet(0x43D3) > 100)
        stars++;

    if (stars >= 4) // set a flag if all 4 stars are collected
        FlagSet(0x2340);

    return stars;
}

extern const u16 sWhiteoutRespawnHealCenterMapIdxs[][2];

void SetWhiteoutRespawnWarpAndHealerNpc(struct WarpData * warp)
{
    u32 healLocationIdx;

    healLocationIdx = GetHealLocationIndexFromMapGroupAndNum(gSaveBlock1->lastHealLocation.mapGroup, gSaveBlock1->lastHealLocation.mapNum);
    warp->mapGroup = sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][0];
    warp->mapNum = sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][1];
    warp->warpId = 0xFF;

    if (sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][0] == 4 && sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][1] == 0) // kanto player indoor
    {
        warp->x = 8;
        warp->y = 5;
    }
    else if (sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][0] == 13 && sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][1] == 0)
    {
        warp->x = 13;
        warp->y = 12;
    }
    else if (32 && sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][1] == 0)
    {
        warp->x = 5;
        warp->y = 4;
    }
    else if (sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][0] == 2 && sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][1] == 10)
    {
        warp->x = 4;
        warp->y = 11;
        VarSet(0x4082, 0);
    }
    else if (sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][0] == 44 && sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][1] == 47)
    {
        warp->x = 5;
        warp->y = 4;
    }
    else if (sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][0] == 44 && sWhiteoutRespawnHealCenterMapIdxs[healLocationIdx - 1][1] == 1)
    {
        warp->x = 7;
        warp->y = 5;
    }
    else
    {
        warp->x = 7;
        warp->y = 4;
    }
    SetWhiteoutRespawnHealerNpcAsLastTalked(healLocationIdx);
}

void AddShuckleFunctionality(void)
{
    u32 species, heldItem;
    for (int i = 0; i < PARTY_SIZE; i++)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);
        heldItem = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
        if ((Random() & 0x7) == 0 && species == SPECIES_SHUCKLE && heldItem == 139) // ITEM_ORAN_BERRY
        {
            heldItem = 44; // ITEM_BERRY_JUICE
            SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &heldItem);
        }
    }
}
