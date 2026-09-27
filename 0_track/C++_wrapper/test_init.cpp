// Minimal native test of MotiveAPI Initialize() - isolates Python from the equation.
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <MotiveAPI.h>

int main()
{
    std::printf("CanConnectToDevices: %d\n", (int) MotiveAPI::CanConnectToDevices());
    std::fflush(stdout);

    std::printf("Calling Initialize...\n");
    std::fflush(stdout);

    try
    {
        MotiveAPI::eResult r = MotiveAPI::Initialize();
        std::printf("Initialize returned: %d\n", (int) r);
        std::fflush(stdout);

        if (r == MotiveAPI::kApiResult_Success)
        {
            std::printf("MarkerCount: %d\n", (int) MotiveAPI::MarkerCount());
            std::fflush(stdout);
            MotiveAPI::Shutdown();
        }
    }
    catch (const std::exception& e)
    {
        std::printf("C++ exception caught: %s\n", e.what());
        std::fflush(stdout);
    }
    catch (...)
    {
        std::printf("Unknown C++ exception caught.\n");
        std::fflush(stdout);
    }

    return 0;
}
