<?php


$donkeyid = new DonkeyId();
for($i=0;$i<2000;$i++)
echo $donkeyid->get_next_id(),"\n";
