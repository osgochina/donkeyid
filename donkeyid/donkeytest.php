<?php

    echo "1--id=".($id1 = dk_get_next_id())."\n";
    echo "1--time=".dk_parse_time($id1)."\n";
    echo "1--node=".dk_parse_node_id($id1)."\n";
    echo "1--sequence=".dk_parse_sequence($id1)."\n";

    $donkey = new DonkeyId(1,123,0);
    $id2 = $donkey->getNextId();
    $time2 = $donkey->parseTime($id2);
    $node2 = $donkey->parseNodeId($id2);
    $sequence2 = $donkey->parseSequence($id2);

    echo "2--id=".$id2."\n";
    echo "2--time=".$time2."\n";
    echo "2--node=".$node2."\n";
    echo "2--sequence=".$sequence2."\n";

   // $array = $donkey->getIdByTime(1,10000);
   // print_r($array);

