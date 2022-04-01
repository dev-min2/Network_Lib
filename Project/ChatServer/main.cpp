
#include "Acceptor.h"
#include "User.h"
#include "RemoveSession.h"
#include "ThreadManager.h"
#include "RemoveSession.h"
int main()
{


    {
        Acceptor<User> ac;
        ac.Init("127.0.0.1", 9199);
        CompletionHandler::GetInstance()->Init(&ac);

        DWORD waitTick = 5000;
        RemoveSession::GetInstance()->InitRemoveSessionThread(waitTick);
        ThreadManager::GetInstance()->Join(); // 모든 쓰레드 종료 대기.
    }


    return 0;
}