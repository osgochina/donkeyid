<?php


    echo "1--id=".($id1 = dk_get_next_id())."\n";
    print_r(dk_parse_id($id1));
    echo "\n";

    $donkey = new DonkeyId(1);
    $id2 = $donkey->getNextId();


    echo "2--id=".$id2."\n";
    print_r($donkey->parseId($id2));
    echo "\n";

   // $array = $donkey->getIdByTime(1,10000);
   // print_r($array);

    for($i=0;$i<100;$i++){
        $id1 = dk_get_next_id();
        //print_r(dk_parse_id($id1));
        echo $id1;
        echo "\n";
    }

