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
        $this->donkeyid0 = new DonkeyId(0,$epoch,$node_id);
        $this->donkeyid1 = new DonkeyId(1,$epoch,$node_id);
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
        if($count < 1){
            return $this->error("parameter error!");
        }
        $type = isset($params[1])?$params[1]:0;
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
                    $d = $this->donkeyid0->parseId($id);
                } elseif ($type == 1) {
                    $d = $this->donkeyid1->parseId($id);
                }
                return $this->success($d);
            }
            case "getidbytime":{
                if(!isset($params[2]) || !isset($params[3])){
                    return $this->error("parameter error!");
                }
                $time = $params[2];
                $num = $params[3];

                if ( $type == 0) {
                    $ids = $this->donkeyid0->getIdByTime($time,$num);
                } elseif ($type == 1) {
                    $ids = $this->donkeyid1->getIdByTime($time,$num);
                }
                return $this->success($ids);
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

