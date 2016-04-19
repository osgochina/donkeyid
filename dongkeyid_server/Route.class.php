<?php
/**
 * Created by PhpStorm.
 * User: ClownFish   187231450@qq.com
 * Date: 16-4-7
 * Time: 下午3:56
 */


class Route
{
    public $error;
    public $donkeyid0;
    public $donkeyid1;

    public function __construct(){
        $epoch = getConfig("epoch");
        $node_id = getConfig("node_id");
        if($epoch <= 0){
            $epoch = 0;
        }
        $this->donkeyid0 = new DonkeyId(0,$epoch);
        $this->donkeyid0->setNodeId($node_id);
        $this->donkeyid1 = new DonkeyId(1,$epoch);
        $this->donkeyid1->setNodeId($node_id);
    }

    public function run($request)
    {
        $retval = $this->exec($request->server["path_info"]);
        return json_encode($retval);
    }

    private function exec($path_info){
        $path_info = trim($path_info,"/");
        $params = explode("/",$path_info);
        $count = count($params);
        if($count < 2){
            return $this->error("parameter error!");
        }
        $type = $params[1];
        switch(strtolower($params[0])){
            case "getnextid": {

                if ( $type == 0) {
                    $id = $this->donkeyid0->getNextId();
                } elseif ($type == 1) {
                    $id = $this->donkeyid1->getNextId();
                }
                return $this->success($id);
            }
            case "parseid":{
                $id = $params[2];
                if(strlen($id) > 20 || !is_numeric($id)){
                    return $this->error("parameter error!");
                }
                if ($type == 0) {
                    $d["time"] = $this->donkeyid0->parseTime($id);
                    $d["node_id"] = $this->donkeyid0->parseNodeId($id);
                    $d["worker_id"] = $this->donkeyid0->parseWorkerId($id);
                    $d["sequence"] = $this->donkeyid0->parseSequence($id);
                } elseif ($type == 1) {
                    $d["time"] = $this->donkeyid1->parseTime($id);
                    $d["node_id"] = $this->donkeyid1->parseNodeId($id);
                    $d["worker_id"] = $this->donkeyid1->parseWorkerId($id);
                    $d["sequence"] = $this->donkeyid1->parseSequence($id);
                }
                return $this->success($d);
            }
        }

    }

    private function success($data){
        return array("code"=>0,"data"=>$data);
    }
    private function error($data){
        return array("code"=>-1,"data"=>$data);
    }

}

