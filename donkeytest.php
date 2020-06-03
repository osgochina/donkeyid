<?php

$nextid =  dk_get_next_id();
echo "nextid:".$nextid."\n";
print_r(dk_parse_id($nextid));

$tsid =  dk_get_ts_id();
echo "tsid:".$tsid."\n";
print_r(dk_parse_ts_id($tsid));

$dtid = dk_get_dt_id();
echo "dtid:".$dtid."\n";

echo "nextids:\n";
print_r(dk_get_next_ids(100,1470298401));
echo "nextids2:\n";
print_r(dk_get_next_ids(100));

echo "tsids:\n";
print_r(dk_get_ts_ids(100,1470298401));
echo "tsids2:\n";
print_r(dk_get_ts_ids(100));

