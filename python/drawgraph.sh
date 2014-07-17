#!/bin/bash
export LD_LIBRARY_PATH=/lib/x86_64-linux-gnu/
echo
for FILE in $(ls *_orig.dot 2> /dev/null); do
	NAME=`echo "$FILE" | cut -d'_' -f1`

	# Process file 1
	DOT1=${NAME}_orig.dot
	PSF1=${NAME}_orig.ps
	PDF1=${NAME}_orig.pdf
	echo -n "Processing $DOT1..."
	dot -Tps $DOT1 -o $PSF1
	ps2pdf $PSF1 $PDF1
	pdfcrop $PDF1 $PDF1 > /dev/null
	echo " done."

	# Process file 2
	DOT2=${NAME}_reduced.dot
	PSF2=${NAME}_reduced.ps
	PDF2=${NAME}_reduced.pdf
	echo -n "Processing $DOT2..."
	dot -Tps $DOT2 -o $PSF2
	ps2pdf $PSF2 $PDF2
	pdfcrop $PDF2 $PDF2 > /dev/null
	echo " done."

	# Join files
	OUTFILE=${NAME}.pdf
	echo -n "Joining files..."
	gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -o $NAME.pdf $PDF1 $PDF2
	echo " done."

	rm -f ${NAME}_*.???
	evince $OUTFILE 2>/dev/null &
done
