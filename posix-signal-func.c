/* posix-signal-func.c 
~~~AshuMeow~~~
--- A signal is a notification to a process that an event has occured. They are also known as S/W interrupts & occurs in async way.
Signals can be sent as,
*** One to another process
		or
*** By the kernel to a process
Every signal has a disposition which is termed as action. The disposition of a signal is set by calling sigaction function.
There are 3 choices for the disposition:-
Choice 1: A function that can be called whenever any specific signal occurs. This function is termed as signal handler & this action is termed as catching a signal.
But, the two signals SIGKILL and SIGSTOP can't be caught.
				void handler(int signo);
Choice 2: By setting the disposition to SIG_IGN, the signal can be ignored. Yet, SIGKILL & SIGSTOP are two troublesome morons which can't be ignored.
Choice 3: SIG_DFL can be set as default disposition. But, SIGCHLD and SIGURG will be encountered.
*/
/*Let's start coding!*/ /* (^_^) */
#include "meow.h"
Sigfunc *signal(int signo, Sigfunc *func)
{
	struct sigaction act, oact;
	act.sa_handler=func;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	if(signo==SIGALRM)
	{
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT; /*SunOS 4.x*/
#endif
	}
	else
	{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART; /*SVR4, 4.4BSD*/
#endif
	}
	if(sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}

/* Syntax used:-

void(*signal(int signo,void(*func)(int)))(int);

Defining SigFunc type for my customized header "meow.h":
typedef void Sigfunc(int);
Then, signal handlers with arguments turns the above line as:
Sigfunc *signal(int signno, Sigfunc *func);
*/
/* Other descriptions:
--- sa_handler from sigaction structure is set to func argument.
--- SA_RESTART is an optional flag. When the flag is set, the system call gets interrupted & the signal will be automatically restarted by kernel.
--- SA_INTERRUPT - When this flag is set, then SIGALRM signal is caught.
*/ /*xoxo*/
/*------------------------------------------------------------------------------------------------------------------*/
