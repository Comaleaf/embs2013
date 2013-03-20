setMode -bs
setMode -bs
setMode -bs
setMode -bs
setCable -port auto
Identify -inferir 
identifyMPM 
setMode -pff
setMode -pff
addConfigDevice  -name "Untitled" -path "/home/iang/Dropbox/Work/EMBS/practicalsExamples"
setSubmode -pffserial
setAttribute -configdevice -attr multibootBpiType -value ""
addDesign -version 0 -name "0"
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr autoSize -value "FALSE"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr swapBit -value "FALSE"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
addPromDevice -p 1 -size 0 -name xcf04s
setMode -pff
setMode -pff
setSubmode -pffserial
setMode -pff
addDeviceChain -index 0
setMode -pff
setMode -pff
setMode -pff
addDeviceChain -index 0
addDevice -p 1 -file "/home/iang/Dropbox/Work/EMBS/practicalsExamples/ethernet.bit"
setMode -pff
setSubmode -pffserial
generate
setCurrentDesign -version 0
setMode -bs
setMode -bs
setMode -bs
assignFile -p 2 -file "/home/iang/Dropbox/Work/EMBS/practicalsExamples/Untitled.mcs"
setAttribute -position 2 -attr packageName -value ""
Program -p 2 -e -v 
Verify -p 2 
setMode -bs
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -bs
saveProjectFile -file "/home/iang//auto_project.ipf"
setMode -bs
deleteDevice -position 1
deleteDevice -position 1
deleteDevice -position 1
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
deletePromDevice -position 1
setCurrentDesign -version 0
deleteDevice -position 1
deleteDesign -version 0
setCurrentDesign -version -1
