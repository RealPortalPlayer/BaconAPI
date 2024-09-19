// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/MessageBox.h>

int main(int argc, char** argv) {
    BA_MessageBox_Result result = BA_MessageBox_Open(BA_MESSAGEBOX_FLAG_BUTTON_YES_NO | BA_MESSAGEBOX_FLAG_ICON_QUESTION, "Question", "Is your fridge running?");
    
    if (result == BA_MESSAGEBOX_RESULT_YES) {
        BA_MessageBox_Open(BA_MESSAGEBOX_FLAG_BUTTON_OK | BA_MESSAGEBOX_FLAG_ICON_INFORMATION, "Information", "Well, you should go catch it");
        return 0;
    }
    
    BA_MessageBox_Open(BA_MESSAGEBOX_FLAG_BUTTON_OK | BA_MESSAGEBOX_FLAG_ICON_INFORMATION, "Information", "You should motivate it more");
}
