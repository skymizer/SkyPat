#
# Regular cron jobs for the pat package
#
0 4	* * *	root	[ -x /usr/bin/pat_maintenance ] && /usr/bin/pat_maintenance
