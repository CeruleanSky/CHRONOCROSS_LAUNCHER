// Compile with:
// gcc -mwindows CHRONOCROSS_LAUNCHER.c -o CHRONOCROSS_LAUNCHER.exe

#include <windows.h>
#include <stdio.h>

HANDLE ghMutex;


int main( void )
{
    STARTUPINFOW si = { 0 };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = { 0 };

    // Chrono Cross tries to launch the launcher after exit, so if we are already running, quit.
    if (OpenMutex(SYNCHRONIZE,FALSE,"Global\\CHRONOCROSS_LAUNCHER") != NULL){
    return 0;
    }

    // Named Mutex shared by the Launcher and Chrono Cross
    ghMutex = CreateMutex(NULL,FALSE,"Global\\CHRONOCROSS_LAUNCHER");

    if (ghMutex == NULL)
    {
    printf("CreateMutex error: %d\n", GetLastError());
    return 1;
    }

    // Launch Chrono Cross, must be ran in same directory
    BOOL result = CreateProcessW(L".\\CHRONOCROSS.exe",NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);

    if (result)
    {
    //remain running until Chrono Cross exits
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    }

    // Prevents infinit launching and looping by waiting a second to ensure
    // that when Chrono Cross launches the launcher upon exit, the second copy
    // sees this copy's mutex and exits as per the code at the beginning.
    Sleep(1000);

    CloseHandle(ghMutex);

    return 0;
}

