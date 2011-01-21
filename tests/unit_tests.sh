#! /bin/bash
for f in ut_*; do
	if [ $f == "ut_template" ]; then
		continue
	fi

	if [ ! -e $f/disabled -a ! -e $f/deleted ]; then
		echo $f
	fi
done
