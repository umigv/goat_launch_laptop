while :
do
	echo "Refreshing"
	rosservice call /move_base/clear_costmaps
	sleep 1
done
