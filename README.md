# RPIPicoOnboardNVS
A library to store Key/Value pairs on the Raspberry PI Pico or Pico-W flash.

This is a C++ LIbrary using the Raspberry PI Pico SDK.

The library has been written to manage different models:

* Bare-metal Single Core: Handle a single core startegy on bare metal. 
It will turn off interrupt during writting to flash. Then reenable.
* Bare-metal Multi Core: Handle both cores being run on the Pico. To write to flash interrupts
are disabled and the other core is temporarily halted while the write occurs. 
* FreeRTOS kernel Single Core: The scheduler and other tasks are temporarily halted during write to flash


## API
API is driven through the singleton object *NVSOnboard*, Non Volatice Storage Onboard the Pico.

The API uses *set* methods to store the key value pairs, and *get* methods to retrieve them. 
Changes to Flash are localy cached in a *dirty* list before the write to flash is done with the command  *commit*.
It is also possible to roll back the changes, get a count of keys in use, and check for key already being in use.

```
NVSOnboard * nvs = NVSOnboard::getInstance();
if (nvs->contains(key)){
	 nvs->get_u16(key, &v);
	 printf("%s = %u\n", key, v);
} else {
	v = 0;
	printf("%s Key not present\n", key);
}

nvs->set_u16(key, v+1);
nvs->commit();
```

## Bare Metal Multicore
The global definition *NVS_CORES* should be set to *2* for the build. I do that through cmake.

The core not running the  Commit must allow itself to be a lock victim with the command
```
multicore_lockout_victim_init()
```



## FreeRTOS Kernel
The library should pick up on the global definition *LIB_FREERTOS_KERNEL* and add dependencies on the FreeRTOS Kernel

## FreeRTOS Kernel SMP
The library now works with FreeRTOS SMP too. For SMP support you need to also using the additional class NVSAgent. This is a singleton and requires to be started as FreeRTOS Tasks.

Commit should only be run on NVSAgent in SMP mode as this class basically controls the flash write so that core0 always undertakes the write and core1 is paused during the process.

There is a seperate test application and unit tests for SMP under the SMP folder.


## CPPUTests
I have included CPUTests in three folders to test the different platform capabilities:
* test: The bare-metal single core tests
* test2Core: The bare-metal two core tests
* testFreeRTOS: The FreeRTOS kernel tests


