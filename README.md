# set-critical-thread

(From mobile, Don't forget to press 'View all of README.md' to open the full write-up..)

We all know the Set/Query Information<object> functions which can be used
to manipulate kernel objects members from user/kernel mode. These functions receive an "INFOCLASS" which
is basically the member we want to modify/query.

![Alt Text](/pictures/members.PNG)

Exploring these members can yield interesting results. These members can be explored by reverse engineering
the windows kernel, reading process hacker headers, or looking at the Windows Research Kernel source code.

The ProcessBreakOnTermination member of the process object is commonly
used by Malware. It allows someone to mark a certain process as critical, meaning that if you try
to kill this process via TerminateProcess, you'll cause a blue-screen.

![Alt Text](/pictures/critical.PNG)

Staring at some process hacker headers I found a lesser known member called ThreadBreakOnTermination 🤔 
Searching google I haven't found anything about it, I guess it may be useful.

![Alt Text](/pictures/breakon.PNG)

As the name implies, this member allows you to set a thread to be critical - so if someone tries to
terminate this thread, he'll cause a blue-screen. Interestingly, the process can still be terminated
by using NtTerminateProccess without any problem.

![Alt Text](/pictures/setinformation.PNG)

This can be useful for Malware - especially for anti-debugging.

SeDebugPrivilege is needed to set this. Otherwise, normal users could cause blue-screens.

This happened when I killed the thread:

![Alt Text](/pictures/bluescreen.PNG)


The code is [here](/SetCriticalThread/main.cpp).
btw It works on windows 7 build 7601 and windows 10 rs5. It's nothing too fancy but I hope it's still interesting 🤓
