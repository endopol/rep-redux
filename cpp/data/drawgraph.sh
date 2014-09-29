#!/bin/bash
export LD_LIBRARY_PATH=/lib/x86_64-linux-gnu/

# Clean directory if -f tag given
if [ "$1" == "-f" ]; then
	rm */*svg */*pdf
fi

# Run dot on all dotfiles
for DIR in $(ls -d */ 2> /dev/null); do
	cd $DIR;
	for DOT in $(ls *.dot 2> /dev/null); do

		NAME=$(basename $DOT .dot)
		SVG=${NAME}.svg
		PDF=${NAME}.pdf

		if [ -f $DOT ] && [ ! -f $PDF ]; then
			echo -n "Processing $DOT..."
			dot -Tsvg $DOT -o $SVG
			inkscape --without-gui --export-pdf="$PDF" $SVG
			pdfcrop $PDF $PDF > /dev/null
			echo " done."
			evince $PDF 2>/dev/null &
		fi
	done
	cd ..
done
