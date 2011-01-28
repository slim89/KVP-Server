#
# Regular cron jobs for the kvp-server package
#
0 4	* * *	root	[ -x /usr/bin/kvp-server_maintenance ] && /usr/bin/kvp-server_maintenance
