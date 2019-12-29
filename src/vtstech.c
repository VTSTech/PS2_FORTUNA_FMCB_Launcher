//Working Title: FORTUNA FMCB Launcher
//Version: 0.2
//Credits:
//Written by VTSTech (veritas@vts-tech.org)
//Uses OSDInit.c/h & libcdvd_add.c/h from 'OSD Initialization Libraries' by SP193
//Compiled with current PS2SDK as of Nov 2019
//Packed with PS2-Packer v1.1.1-unofficial-09ac9c6 by Nicolas "Pixel" Noble 

#include <debug.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <stdio.h>
#include <unistd.h>
#include <input.h>
#include <time.h>
#include <string.h>
#include <libcdvd.h>
#include <fileXio.h>
#include <fileXio_rpc.h>
#include <sbv_patches.h>

#include "OSDInit.h"

u64 Timer(void)
{
	return (clock() / (CLOCKS_PER_SEC / 1000));
}

//thx sp193
void ResetIOP()
{
	SifInitRpc(0);
	while(!SifIopReset("", 0)){};
	while(!SifIopSync()){};
	SifInitRpc(0);
}

void InitPS2()
{
	init_scr();
	ResetIOP();
	SifInitIopHeap();
	SifLoadFileInit();
	fioInit();
	sbv_patch_disable_prefix_check();
	SifLoadModule("rom0:SIO2MAN", 0, NULL);
	SifLoadModule("rom0:MCMAN", 0, NULL);
	SifLoadModule("rom0:MCSERV", 0, NULL);
}

void LoadElf(const char *elf, char* path)
{
	char* args[1];
	t_ExecData exec;	
	SifLoadElf(elf, &exec);

	if(exec.epc > 0)
	{	
		// Reset IOP, since not all ELF's do it and we've loaded additional IOP
		// modules which need to be unloaded
		ResetIOP();

		if(path != 0)
		{
			args[0] = path;
			ExecPS2((void*)exec.epc, (void*)exec.gp, 1, args);
		}
		else
		{
			ExecPS2((void*)exec.epc, (void*)exec.gp, 0, NULL);
		}
	}
}

void banner()
{
	scr_clear();
  scr_printf("============================================== \n");
  scr_printf("=FORTUNA FMCB Launcher v0.2========12-29-2019= \n");
  scr_printf("=BOOT.ELF Written by VTSTech of PSX-Place.com= \n");
  scr_printf("=FORTUNA Project by krat0s of PS2-Home.com==== \n");
  scr_printf("=www.vts-tech.org============================= \n\n");	
}


int getFileSize(int fd) {
	int size = fioLseek(fd, 0, SEEK_END);
	fioLseek(fd, 0, SEEK_SET);
	return size;
}

int main(int argc, char *argv[], char **envp)
{
	char *target, *path, *SYSEXEC;
	char device[50];
	char folder[50];
	int CdStatus;
	int fd;
	int FORTUNA = 0;
	int FMCB = 0;
	int BOOT2 = 0;
	int MC0T = 0;
	int MC1T = 0;
	int MC0F = 0;
	int MC1F = 0;
	char *MC0FMCBDIR = "mc0:/";
	char *MC1FMCBDIR = "mc1:/";
	char *MC0FMCBEXEC[100];
	char *MC1FMCBEXEC[100];
	char *FORTUNA_VER[4];
	InitPS2();
	banner();
	CdStatus = sceCdStatus();
	OSDInitSystemPaths();
	SYSEXEC = OSDGetSystemExecFolder();
	strncpy(SYSEXEC,OSDGetSystemExecFolder(),13);
	strncat(SYSEXEC,"/osdmain.elf",12);
	fioClose(fd);
	fd = fioOpen("mc0:/FORTUNA/icon.icn", O_RDONLY);
	if (getFileSize(fd) == 51040){
		FORTUNA = 1;
		MC0T = 1;
		strcpy(FORTUNA_VER,"rev1");
	} else if (getFileSize(fd) == 16876){
		FORTUNA = 1;
		MC0T = 1;
		strcpy(FORTUNA_VER,"rev2");
	}
	fioClose(fd);
	fd = fioOpen("mc1:/FORTUNA/icon.icn", O_RDONLY);
	if (getFileSize(fd) == 51040){
		FORTUNA = 1;
		MC1T = 1;
		strcpy(FORTUNA_VER,"rev1");
	} else if (getFileSize(fd) == 16876){
		FORTUNA = 1;
		MC1T = 1;
		strcpy(FORTUNA_VER,"rev2");
	}
	if(MC0T == 1){ scr_printf("FORTUNA %s detected on Memory Card 1! \n",FORTUNA_VER); }
	if(MC1T == 1){ scr_printf("FORTUNA %s detected on Memory Card 2! \n",FORTUNA_VER); }
	fioClose(fd);
	strncat(MC0FMCBDIR,SYSEXEC,25);
	fd = fioOpen(MC0FMCBDIR, O_RDONLY);
	if (fd <= 0){
	} else {
		scr_printf("FMCB detected on Memory Card 1! \n");
		FMCB = 1;
		MC0F = 1;
	}
	fioClose(fd);
	strncat(MC1FMCBDIR,SYSEXEC,25);
	fd = fioOpen(MC1FMCBDIR, O_RDONLY);
	if (fd <= 0){
	} else {
		scr_printf("FMCB detected on Memory Card 2! \n");
		FMCB = 1;
		MC1F = 1;
	}
	fioClose(fd);
	fd = fioOpen("mc0:/FORTUNA/BOOT2.ELF", O_RDONLY);
	if (fd <= 0){
	} else {
		scr_printf("BOOT2.ELF available on Memory Card 1! \n");
		MC0T = 1;
		BOOT2 = 1;
	}
	fioClose(fd);
	fd = fioOpen("mc1:/FORTUNA/BOOT2.ELF", O_RDONLY);
	if (fd <= 0){
	} else {
		scr_printf("BOOT2.ELF available on Memory Card 2! \n");
		MC1T = 1;
		BOOT2 = 1;
	}
	if (FMCB == 1){
		if (MC0F == 1){	
			char *args[3];
			strncpy(MC0FMCBEXEC,"-x ",3);
			strncat(MC0FMCBEXEC,MC0FMCBDIR,33);
			args[0] = "-m rom0:SIO2MAN";
			args[1] = "-m rom0:MCMAN";
			args[2] = MC0FMCBEXEC;
			scr_printf("Launching FMCB ... \n");
			LoadExecPS2("moduleload", 3, args);
			}
		if (MC1F == 1){
			char *args[3];
			strncpy(MC1FMCBEXEC,"-x ",3);
			strncat(MC1FMCBEXEC,MC1FMCBDIR,33);
			args[0] = "-m rom0:SIO2MAN";
			args[1] = "-m rom0:MCMAN";
			args[2] = MC1FMCBEXEC;
			scr_printf("Launching FMCB ... \n");
			LoadExecPS2("moduleload", 3, args);
			}

	}
	if (BOOT2 == 1 && FMCB == 0){
		if (MC0T == 1){
			strncpy(device,"mc0:",5);
			strncpy(folder,"/FORTUNA/",10);
			target = device;
			strncat(target,folder,14);
			path = target;
			strcat(target,"BOOT2.ELF");
			}
		if (MC1T == 1){
			strncpy(device,"mc1:",5);
			strncpy(folder,"/FORTUNA/",10);
			target = device;
			strncat(target,folder,14);
			path = target;
			strcat(target,"BOOT2.ELF");
			}
		scr_printf("Launching BOOT2.ELF ... \n");
		LoadElf(target,path);	
	}
  return 0;
}
