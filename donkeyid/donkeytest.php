<?php


    $donkey = new DonkeyId();
    //$donkey->setNodeId(10);

    $id = $donkey->getNextId();
    $time = $donkey->parseTime($id);
    $node = $donkey->parseNodeId($id);
    $sequence = $donkey->parseSequence($id);

    echo "id=".$id."\n";
    echo "node=".$node."\n";
    echo "sequence=".$sequence."\n";

   // $array = $donkey->getIdByTime(1,10000);
   // print_r($array);

