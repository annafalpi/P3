#!/bin/bash

# Put here the program (maybe with path)

for filename in proves/train/*.wav; do 
    #sox $inputfile $outputfile lowpass 500.0
    #$GETF0 $fwav $ff0 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    EXEC = "sox  $proves/$origina/$filename.wav $proves/$filtered/$filename.wav lowpass 500.0"
    echo $EXEC && $EXEC || exit 1
done

exit 0
