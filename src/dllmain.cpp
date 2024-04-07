#include "dllmain.h"

class ContentLog {
public:
    uintptr_t** vtable;
};

uint64_t WriteToLog(ContentLog* contentLog, bool a1, unsigned int logArea, unsigned int logLevel, char** strings) {
    using function = decltype(&WriteToLog);
    static auto func = std::bit_cast<function>(SigScan("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 88 54 24"));
    return (*func)(contentLog, a1, logArea, logLevel, strings);
}

ContentLog* CONTENT_LOG = nullptr;

SafetyHookInline _ContentLog_CTOR;
ContentLog* ContentLog_CTOR(ContentLog* contentLog) {
    _ContentLog_CTOR.call<ContentLog*>(contentLog);

    Log::Info("Content Log Pointer: {}", (uint64_t)contentLog);

    CONTENT_LOG = contentLog;

    char message = '\0';
    char* message_ptr = &message;
    char* messages_ptr[] = { message_ptr };

    WriteToLog(CONTENT_LOG, true, 3, 6, messages_ptr);

    return contentLog;
}

// SafetyHookInline _ContentLog__WriteToLog;
// void ContentLog__WriteToLog(ContentLog* contentLog, bool a1, unsigned int logLevel, unsigned int logArea, ...) {
//     Log::Info("Content Log Pointer: {}", (uint64_t)contentLog);

//     uintptr_t vtableAddr = reinterpret_cast<uintptr_t>(contentLog->vtable);

//     Log::Info("0x{:x}", vtableAddr);
// }

void OnJoin(ClientInstance* client) {

}

ModFunction void Initialize(AmethystContext* ctx)
{
    ctx->mHookManager.RegisterFunction<&ContentLog_CTOR>("48 89 5C 24 ? 48 89 4C 24 ? 57 48 83 EC ? 48 8B F9 48 8D 05 ? ? ? ? 48 89 01 48 89 4C 24 ? 48 83 C1 ? 48 8D 54 24 ? E8 ? ? ? ? 90 48 8D 05 ? ? ? ? 48 89 07 C6 47");
    ctx->mHookManager.CreateHook<&ContentLog_CTOR>(_ContentLog_CTOR, &ContentLog_CTOR);

    // ctx->mEventManager.onStartJoinGame.AddListener(&OnJoin);

    // ctx->mHookManager.RegisterFunction<&ContentLog__WriteToLog>("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 88 54 24");
    // ctx->mHookManager.CreateHook<&ContentLog__WriteToLog>(_ContentLog__WriteToLog, &ContentLog__WriteToLog);
}