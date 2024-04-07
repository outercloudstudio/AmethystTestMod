#include "dllmain.h"

class ContentLog {
public:
    uintptr_t** vtable;
};

ContentLog* CONTENT_LOG = nullptr;

SafetyHookInline _ContentLog_CTOR;
ContentLog* ContentLog_CTOR(ContentLog* contentLog) {
    _ContentLog_CTOR.call<ContentLog*>(contentLog);

    CONTENT_LOG = contentLog;

    return contentLog;
}

SafetyHookInline _ContentLog__Log;
int ContentLog__Log(ContentLog* contentLog, bool doNotRepeat, unsigned int logArea, unsigned int logLevel, char* strings) {
    Log::Info("strings {}", strings);
    Log::Info("strings2 {}", *(&strings + 1));

    // char** messages = new char* [] { message, message2 };
    // char*** newStrings = &messages;

    return _ContentLog__Log.call<int>(contentLog, doNotRepeat, logArea, logLevel, strings);
}

// uint64_t WriteToLog(ContentLog* contentLog, bool a1, unsigned int logArea, unsigned int logLevel, char*** strings) {
//     using function = decltype(&WriteToLog);
//     static auto func = std::bit_cast<function>(SigScan("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 88 54 24"));

//     Log::Info("*strings[0] {}", (*strings)[0]);
//     Log::Info("*strings[1] {}", (*strings)[1]);

//     return _ContentLog__WriteToLog.call<int>(CONTENT_LOG, true, logArea, logLevel, strings);
// }

SafetyHookInline _TestHook;
int64_t TestHook(int64_t a1, int64_t* a2, int64_t a3, int64_t a4, int a5, int* a6, int8_t a7, int a8, int64_t* a9, char a10) {
    // char message[] = "[[Jigsaw Structure]] couldn't be constructed, no pool named `%s`";
    char message[] = "brooo";
    // char** messages = new char* [] { message, message2 };
    // char*** strings = &messages;

    // WriteToLog(CONTENT_LOG, true, 29, 3, strings);

    Log::Info("Test: {}", message);

    _ContentLog__Log.call<int>(CONTENT_LOG, false, 29, 3, message);

    return _TestHook.call<int64_t>(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

ModFunction void Initialize(AmethystContext* ctx)
{
    //ContentLog::Log 44 89 4C 24 ? 48 83 EC
    //ContentLog::SafeLog 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 49 8B F9 41 8B D8

    ctx->mHookManager.RegisterFunction<&TestHook>("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 4C 24 ? 4D 8B E0");
    ctx->mHookManager.CreateHook<&TestHook>(_TestHook, &TestHook);

    ctx->mHookManager.RegisterFunction<&ContentLog_CTOR>("48 89 5C 24 ? 48 89 4C 24 ? 57 48 83 EC ? 48 8B F9 48 8D 05 ? ? ? ? 48 89 01 48 89 4C 24 ? 48 83 C1 ? 48 8D 54 24 ? E8 ? ? ? ? 90 48 8D 05 ? ? ? ? 48 89 07 C6 47");
    ctx->mHookManager.CreateHook<&ContentLog_CTOR>(_ContentLog_CTOR, &ContentLog_CTOR);

    ctx->mHookManager.RegisterFunction<&ContentLog__Log>("44 89 4C 24 ? 48 83 EC");
    ctx->mHookManager.CreateHook<&ContentLog__Log>(_ContentLog__Log, &ContentLog__Log);
}