<?php


    $donkey = new DonkeyId(0,"1460710318");
    $donkey->setNodeId(10);

    $id = $donkey->getNextId();
    $time = $donkey->parseTime($id);
    $node = $donkey->parseNodeId($id);
    $worker_id = $donkey->parseWorkerId($id);
    $sequence = $donkey->parseSequence($id);

    echo "id=".$id."\n";
    echo "node=".$node."\n";
    echo "workerid=".$worker_id."\n";
    echo "sequence=".$sequence."\n";

    $array = $donkey->getIdByTime(1460719318,101);
    print_r($array);

