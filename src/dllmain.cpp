#include "dllmain.h"

SafetyHookInline _UI_click_or_playSound;
void UI_click_or_playSound(int64_t UI_PTR_MAYBE, char* sound, float volume) {
    Log::Info("sound: {} volume: {}", (std::string)sound, volume);
}

SafetyHookInline _playSound_SUB;
void playSound_SUB(char* a) {
    Log::Info("soundPath: {}", (std::string)(a + 6));
}

// Ran when the mod is loaded into the game by AmethystRuntime
ModFunction void Initialize(AmethystContext* ctx)
{
    // 40 55 56 57 41 56 48 83 EC ? 48 BD

    ctx->mHookManager.RegisterFunction<&playSound_SUB>("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 50 ? 44 0F 29 58 ? 44 0F 29 A0 ? ? ? ? 48 8B D9");
    ctx->mHookManager.CreateHook<&playSound_SUB>(_playSound_SUB, &playSound_SUB);

    ctx->mHookManager.RegisterFunction<&UI_click_or_playSound>("40 53 57 48 83 EC ? 0F 29 74 24 ? 0F 29 7C 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 0F 28 FB");
    ctx->mHookManager.CreateHook<&UI_click_or_playSound>(_UI_click_or_playSound, &UI_click_or_playSound);
}