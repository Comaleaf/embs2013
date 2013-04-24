#!/bin/bash
set -e
set +x

cflags="-Wall -O2 -std=gnu99"
sourcefiles="software/main.c software/ethernet.c software/gui.c software/hc.c software/utilities.c software/vlab.c software/interrupts.s"
handelcfiles="main.hcc dac.hcc fsl.hcc vlab.hcc"

mhsfile=system.mhs
part="xc3s500efg320-4"
bitfile=implementation/system.bit
bmmfile=implementation/system_bd.bmm
finalbitfile=system-programmed.bit
libusbdriver=/opt/usb-driver/libusb-driver.so
pdklocation=/usr/share/mentor/pdk

function usage {
	echo "FPGA Usage:"
	grep "Number of Slices" implementation/system.par
	grep "Number of RAMB16s" implementation/system.par
	echo -n "   " 
	grep -Eo "Maximum frequency:[ ]*([0-9\.]*MHz)" implementation/system.twr
	echo "   (If less than 50MHz the system may be unreliable)"
	
	if grep -qE "([0-9]*) constraint[s]? not met" implementation/system.par
	then
		echo
		echo "WARNING! Timing constraint(s) have not been met. Your design may not operate as expected!"
		echo "You have generated a deep logic path in your Handel-C design. Try splitting up large expressions, avoiding wide multipliers or dividers, and adding delay statements."
		echo
	fi
}

function compile {
	echo \* Compiling...
	mb-gcc $cflags $sourcefiles -o software/microblaze_0.elf
		
	set +e
	elfcheck -hw system.xml -p $part -pe microblaze_0 software/microblaze_0.elf > elfcheck.log 2>&1
	if grep --quiet "elfcheck passed." elfcheck.log
	then
		echo \* elfcheck passed.
	else
		echo "ERROR: elfcheck failed!"
		cat elfcheck.log
		exit 1
	fi
	

	echo \* Executable size:
	mb-size software/microblaze_0.elf

	echo \* Programming bitfile \"$bitfile\" with \"software/microblaze_0.elf\"...
	data2mem -bm $bmmfile -bt $bitfile -bd software/microblaze_0.elf tag microblaze_0 -o b $finalbitfile
	
	if [ "$?" -ne 0 ]; then echo data2mem failed; exit 1; fi
	set -e

	echo \* Bitfile preparation complete - \"$finalbitfile\" generated.
}


case "$1" in

	'' )
		echo "EMBS synthesis script. Available modes:
    implement
        Rebuild the hardware (for if you have changed 
        your Handel-C, or XPS design)
		
    compile
        Compile software for the Microblaze, execute after 
        'implement'. Produces $finalbitfile

    program
        Does 'compile' but also programs an attached FPGA 
        board with the final bitfile.

    usage
        Display device usage
		
    clean
        Clean all generated files (useful if you have moved
        the location of your project)"
	;;

	'implement' )
		#Check the user has built their Handel-C project
		if [ ! -f implementation/userhandelc.edf ]; then
			echo "The EDIF file implementation/userhandelc.edf cannot been found. Ensure you have built your Handel-C project with EDIF as the output format."
			exit
		fi

		#Delete any previously implemented bitfiles so that if implementation fails then people are forced to notice
		rm -f implementation/system.bit
		rm -f $finalbitfile
		
		
		#Delete enough files to force reimplementation
		set +e
		rm -rf implementation/system_handelc*
		rm -rf implementation/cache/*handelc*
		set -e
		
		#Make the backslashes of XILINX_EDK into forward slashes (for Windows)
		XILINX_EDK_FS=$(echo $XILINX_EDK | sed 's@\\@\/@g')
		#Update system_incl.make with this path
		sed -ir 's@XILINX_EDK_DIR = .*@XILINX_EDK_DIR = '$XILINX_EDK_FS'@g' system_incl.make 
		
		#Turn off timing errors
		sed -ir 's@OBSERVE_PAR_OPTIONS = .*@OBSERVE_PAR_OPTIONS = -error no@g' system_incl.make 
		
		#Clean up sed's temp files that seem to get left around on Windows
		rm -rf sed*
		
		#Copy the appropriate makefile (assume that the presence of backslashes in XILINX_EDK means we are on Windows)
		rm -f system.make
		set +e
		echo "$XILINX_EDK" | grep '\\'
		if [ $? -eq 0 ]
		then
			echo "Using Windows makefile"
			cp system.makewin system.make
		else
			echo "Using Linux makefile"
			cp system.makelin system.make
		fi
		set -e
		
		#Run the tools
		make -f system.make bits

		#Create the system.xml file for elfcheck
		psf2Edward -inp system.xmp -exit_on_error -dont_add_loginfo -make_inst_lower -edwver 1.2 -xml system.xml >> /dev/null 2>&1
		
		#Display device usage
		usage;
	;;

	'clean' )
		set +e
		rm -f $finalbitfile
		rm -f bitinit.log
		rm -f software/*.o
		rm -f software/*.elf		
		rm -rf implementation synthesis xst hdl
		rm -rf xst.srp system.srp
		rm -f *.log
		rm -rf handelc/EDIF
		mv -f __xps/platgen.opt platgen.opt
		mv -f __xps/xpsxflow.opt xpsxflow.opt
		rm -rf __xps
		mkdir __xps
		mv -f platgen.opt __xps/platgen.opt
		mv -f xpsxflow.opt __xps/xpsxflow.opt
		set -e
	;;
	
	'handelc' )
		cd handelc
		handelc -edif -f XilinxSpartan3E -p $part -L $pdklocation/hardware/lib -I $pdklocation/hardware/include stdlib.hcl -o ../implementation/userhandelc.edf $handelcfiles
		cd ..
	;;
	
	'program' )
		compile
		if [ "$2" = "pport" ]; then
			export LD_PRELOAD=$libusbdriver
		fi
		echo \* Programming FPGA...
		
		set +e
		impact -batch impact.cmd > /dev/null 2>&1
		set -e
		
		if grep --quiet "Programmed successfully." _impactbatch.log
		then
			echo \* FPGA programmed.
		else
			echo "ERROR: Programming failed. Examine _impactbatch.log for error details."
		fi
	;;
	
	'compile' )
		compile
	;;

	'usage' )
		usage;
	;;
	
	'createpack' )
		if [ "$2" = "" ]; then
			exit 1
		fi
		mkdir ../$2
		mkdir ../$2/__xps
		cp __xps/*.opt ../$2/__xps
		cp -r data ../$2
		cp -r etc ../$2
		mkdir ../$2/handelc
		cp handelc/vlab.* ../$2/handelc
		cp handelc/userhandelc-default.dkp ../$2/handelc/userhandelc.dkp
		cp handelc/userhandelc-default.dkw ../$2/handelc/userhandelc.dkw
		cp handelc/userhandelc-default.pref ../$2/handelc/userhandelc.pref
		cp handelc/main-default.hcc ../$2/handelc/main.hcc
		cp -r hdl ../$2
		mkdir ../$2/implementation
		cp handelc/userhandelc-default.edf ../$2/implementation/userhandelc.edf
		cp -r pcores ../$2
		mkdir ../$2/software
		cp software/*.s software/vlab.* ../$2/software
		cp software/microblaze_0-default.c ../$2/software/microblaze_0.c
		cp _impact.cmd bash.exe embs.sh impact.cmd system.bsb system.makewin system.makelin system.mhs system.xmp system_incl.make ../$2
		echo "Complete."
	;;
	
esac

