#include "../include/global.h"
#include "../include/alloc.h"
#include "../include/event_data.h"
#include "../include/main.h"
#include "../include/overworld.h"
#include "../include/pokemon.h"
#include "../include/save.h"
#include "../include/sound.h"
#include "../include/sprite.h"
#include "../include/string_util.h"
#include "../include/task.h"

#define FLAG_BUG_CATCHING_CONTEST (0x2342)
#define IS_IN_BUG_CATCHING_CONTEST (FlagGet(FLAG_BUG_CATCHING_CONTEST))
#define gRemainingParkBalls (*(u8 *)0x0203FEC8)
#define SECONDS_IN_CONTEST (20 * 60) // 20 minute total time

struct BugCatchingContestSwapScreen
{
    u8 cursorPos;
    u8 backgroundScrollTaskId;
    struct Pokemon *newMon;
    MainCallback returnCallback;
    struct Sprite *cursorSprite;
    struct Sprite *monIconSprites[2];
};

extern struct Pokemon *gBugContestMon;
extern struct BugCatchingContestSwapScreen *gBCCSwapScreen;

// change start menu behavior:
// get rid of save, add script to quit out

// stop bug catching contest

// if day ends, stop bug catching contest

// functions to spawn the swap screen

// score caught mon in gBugContestMon
// max score is 400:
// level as percentage of max that can be found
// iv's relative to max as percentage (186)
// hp relative to max as percentage
// rarity factor -- caterpie metapod weedle kakuna wurmple silcoon cascoon kricketot are all at 60, scyther pinsir are at 100, 80 everything else
// scyther/pinsir with perfect iv's and no damage dealt is all that can get 400


// functions to handle storing bug catching contest mon direct from party as well as giving the player the bug catching contest mon
u32 StoreCaughtBCCMon(void)
{
    u32 ret = FALSE;
    if (gPlayerPartyCount > 1)
    {
        if (gBugContestMon == NULL)
            gBugContestMon = Alloc(sizeof(struct Pokemon));
        memcpy(gBugContestMon, &gPlayerParty[1], sizeof(struct Pokemon));
        memset(&gPlayerParty[1], 0, sizeof(struct Pokemon));
        ret = TRUE;
    }
    return ret;
}

u32 FreeCaughtBCCMonAndDeposit(void)
{
    u32 ret = GiveMonToPlayer(gBugContestMon);
    if (ret != 2) // can't give to player
    {
        Free(gBugContestMon);
        gBugContestMon = NULL;
    }
    return ret;
}
