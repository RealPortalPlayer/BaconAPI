#include <BaconAPI/ArgumentHandler.h>

void Test(void);

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);
    Test();
}