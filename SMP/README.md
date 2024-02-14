# FreeRTOS kernel SMP
FreeRTOS Kernel SMP approach allows both cores to be used by the Kernel Scheduler.  This folder is a seperate test project for the SMP configuration of NVSOnboard.

To run under SMP we need t ous the aditional Agent NVSAgent and use commit on the NVSAgent rather than the commit via the NVSOnboard class which is not safe in SMP mode.