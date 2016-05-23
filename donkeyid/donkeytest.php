<?php


    $donkey = new DonkeyId(0,0,21);

    $id = $donkey->getNextId();
    $time = $donkey->parseTime($id);
    $node = $donkey->parseNodeId($id);
    $sequence = $donkey->parseSequence($id);

    echo "id=".$id."\n";
    echo "time=".$time."\n";
    echo "node=".$node."\n";
    echo "sequence=".$sequence."\n";

   // $array = $donkey->getIdByTime(1,10000);
   // print_r($array);

