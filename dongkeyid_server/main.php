<?php
/**
 * Created by PhpStorm.
 * User: ClownFish 187231450@qq.com
 * Date: 16-4-11
 * Time: 下午10:44
 */

date_default_timezone_set('Asia/Shanghai');
define('APP_DEBUG', true);
define('DS', DIRECTORY_SEPARATOR);
define('ROOT_PATH', realpath(dirname(__FILE__)) . DS);

function loadClass($class){
    $file = $class.".class.php";

    if(file_exists(ROOT_PATH.$file)){
        require ROOT_PATH.$file;
    }
}
spl_autoload_register("loadClass");

function getConfig($name = ""){
    static $config = 0;
    if($config === 0){
        $config = include ROOT_PATH."config.php";
    }
    if(empty($name)){
        return $config;
    }
    if(isset($config[$name])){
        return $config[$name];
    }
    return "";
}

$config = array(
    "host" => getConfig("host"),
    "port"  => getConfig("port"),
);
$donkey = new HttpServer($config);