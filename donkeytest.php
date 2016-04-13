<?php


    $donkey = new DonkeyId();
    $donkey->setNodeId(11);
    $id = $donkey->getNextId();
    $time = $donkey->parseTime($id);
    $node = $donkey->parseNodeId($id);
    
    echo "donkeyid=".$id."\n";
    echo "time=".date("Y-m-d H:i:s",$time/1000)."\n";
    echo "node=".$node."\n";
    boo

