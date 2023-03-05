echo "set title \"$1\"" > generated-script
echo "set terminal png size 1280, 480 font \"Source Code Varaible,30\"" >> generated-script
echo >> generated-script
echo "set xlabel \"byte\"" >> generated-script
echo "set ylabel \"count\"" >> generated-script
echo  >> generated-script
echo "set xrange[0:256]" >> generated-script
echo "set yrange[$3:$4]" >> generated-script
echo  >> generated-script
echo "set terminal png size 1200,900" >> generated-script
echo "set output \"image.png\"" >> generated-script
echo  >> generated-script
echo "set boxwidth 9.0 relative" >> generated-script
echo "set style data histograms" >> generated-script
echo "set style fill solid 1.0 border -1" >> generated-script
echo  >> generated-script
echo "set style histogram clustered gap 10" >> generated-script
echo  >> generated-script
echo "plot \"$2\" using 2" >> generated-script
