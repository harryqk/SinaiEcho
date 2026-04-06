//
// Created by harryqk on 3/27/26.
//

#ifndef SINAIECHO_PIPEWAKEUP_H
#define SINAIECHO_PIPEWAKEUP_H

#pragma once
#include "Wakeup.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
namespace SinaiEcho
{
    class PipeWakeup : public Wakeup
    {
    public:
        PipeWakeup()
        {
            if (pipe(fds) < 0)
            {
                perror("pipe");
                abort();
            }

            // 设置非阻塞
            SetNonBlock(fds[0]);
            SetNonBlock(fds[1]);
        }

        ~PipeWakeup()
        {
            close(fds[0]);
            close(fds[1]);
        }

        int GetFd() const override
        {
            return fds[0]; // 读端给 Poller
        }

        void Wake() override
        {
            uint64_t one = 1;
            ssize_t n = write(fds[1], &one, sizeof(one));
            (void)n;
        }

        void HandleRead() override
        {
            char buf[1024];
            //read(fds[0], buf, sizeof(buf));
            //非阻塞
            while (true)
            {
                ssize_t n = read(fds[0], buf, sizeof(buf));
                if (n > 0)
                {
                    continue; // 继续读，直到读空
                }
                else if (n == 0)
                {
                    // pipe 被关闭（一般不该发生）
                    break;
                }
                else // n < 0
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        // 已经读干净了
                        break;
                    }
                    else if (errno == EINTR)
                    {
                        // 被信号打断，继续读
                        continue;
                    }
                    else
                    {
                        // 真错误
                        perror("read error");
                        break;
                    }
                }
            }
        }

    private:
        int fds[2];

        void SetNonBlock(int fd)
        {
            int flags = fcntl(fd, F_GETFL, 0);
            fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    };
}


#endif //SINAIECHO_PIPEWAKEUP_H
