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
                return $this->success(dk_get_next_id());
            }
            case "gettsid": {
                return $this->success(dk_get_ts_id());
            }
            case "getdtid": {
                return $this->success(dk_get_dt_id());
            }
            case "parseid":{
                $id = $params[1];
                if(strlen($id) > 20 || !is_numeric($id)){
                    return $this->error("parameter error!");
                }
                return $this->success(dk_parse_id($id));
            }
            case "parsetsid":{
                $id = $params[1];
                if(strlen($id) > 20 || !is_numeric($id)){
                    return $this->error("parameter error!");
                }
                return $this->success(dk_parse_ts_id($id));
            }
             case "getnextids":{
                if(!isset($params[1]) || !isset($params[2])){
                    return $this->error("parameter error!");
                }
                $time = $params[1];
                $num = $params[2];
                return $this->success(dk_get_next_ids($time,$num));
            }
            case "gettsids":{
                if(!isset($params[1]) || !isset($params[2])){
                    return $this->error("parameter error!");
                }
                $time = $params[1];
                $num = $params[2];
                return $this->success(dk_get_ts_ids($time,$num));
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

