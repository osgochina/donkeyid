<?php


$donkeyid = new DonkeyId();
for($i=0;$i<20;$i++){
    $donkeyid->set_node_id($i);
    echo $id = $donkeyid->get_next_id(),"\n";
    echo $time = $donkeyid->get_time_by_donkeyid($id),"\n";
    echo date("Y-m-d H:i:s",$time/1000),"\n";
    echo $donkeyid->get_node_by_donkeyid($id),"\n";
}


