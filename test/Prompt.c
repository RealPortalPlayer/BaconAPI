#include <BaconAPI/Prompt.h>
#include <BaconAPI/Logger.h>
#include <stdlib.h>

void Test(void) {
    BA_Prompt_Set("> ");

    char* result = BA_Prompt_Show();

    BA_LOGGER_INFO("You said: %s\n", result);
    free(result);
}