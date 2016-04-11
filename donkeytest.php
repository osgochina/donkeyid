<?php


    $donkey = new DonkeyId();
    $donkey->set_node_id(11);
    $id = $donkey->get_next_id();
    $time = $donkey->get_time_by_donkeyid($id);
    $node = $donkey->get_node_by_donkeyid($id);
    
    echo "donkeyid=".$id."\n";
    echo "time=".date("Y-m-d H:i:s",$time/1000)."\n";
    echo "node=".$node."\n";

