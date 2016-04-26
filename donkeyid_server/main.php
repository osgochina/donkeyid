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




class Main
{
    static private $host = 0;
    static private $port = 0;
    static private $pid_file;
    static private $log_path;
    static private $options = "h:dp:d:s:l";
    static private $longopts = array("help", "daemon","host:","port:", "pid:", "log:");
    static private $help = <<<EOF

  帮助信息:
  Usage: /path/to/php main.php [options] -- [args...]

  --help             显示帮助信息
  -h  [--host]       ip地址,默认监听127.0.0.1
  -p  [--port]       端口,默认9521
  -pd [--pid]        指定pid文件位置(默认pid文件保存在当前目录)
  -l  [--log]        log文件夹的位置
  -s  start          启动进程
  -s  stop           停止进程
  -d  [--daemon]     是否后台运行
EOF;

    /**
     * 运行入口
     */
    static public function run()
    {
        $opt = getopt(self::$options, self::$longopts);
        self::spl_autoload_register();
        self::params_help($opt);
        self::params_host($opt);
        self::params_port($opt);
        self::params_l($opt);
        self::params_d($opt);
        self::params_pd($opt);
        self::params_s($opt);

    }

    /**
     * 注册类库载入路径
     */
    static public function spl_autoload_register()
    {
        spl_autoload_register(function($class){
            $file = $class.".class.php";
            if(file_exists(ROOT_PATH.$file)){
                require ROOT_PATH.$file;
            }
        });
    }

    /**
     * 解析帮助参数
     * @param $opt
     */
    static public function params_help($opt)
    {
        if (empty($opt) || isset($opt["help"])) {
            die(self::$help."\n");
        }
    }

    static public function params_host($opt)
    {
        if (isset($opt["h"])) {
            self::$host = $opt["h"];
        }
        if (isset($opt["host"])) {
            self::$host = $opt["host"];
        }
    }

    static public function params_port($opt)
    {
        if (isset($opt["p"])) {
            self::$port = $opt["p"];
        }
        if (isset($opt["port"])) {
            self::$port = $opt["port"];
        }
    }

    /**
     * 解析运行模式参数
     * @param $opt
     */
    static public function params_d($opt)
    {
        if (isset($opt["d"]) || isset($opt["daemon"])) {
            HttpServer::$daemon = true;
        }
    }


    /**
     * 解析pid参数
     * @param $opt
     */
    static public function params_pd($opt)
    {
        //记录pid文件位置
        if (isset($opt["pd"]) && $opt["pd"]) {
            HttpServer::$pid_file = $opt["pd"] . "/pid";
        }
        //记录pid文件位置
        if (isset($opt["pid"]) && $opt["pid"]) {
            HttpServer::$pid_file  = $opt["pid"] . "/pid";
        }
        if (empty(HttpServer::$pid_file )) {
            HttpServer::$pid_file  = ROOT_PATH . "/pid";
        }
    }

    /**
     * 解析日志路径参数
     * @param $opt
     */
    static public function params_l($opt)
    {
        if (isset($opt["l"]) && $opt["l"]) {
            self::$log_path = $opt["l"];
        }
        if (isset($opt["log"]) && $opt["log"]) {
            self::$log_path = $opt["log"];
        }
        if (empty(self::$log_path)) {
            self::$log_path = ROOT_PATH . "logs/";
        }
    }



    /**
     * 解析启动模式参数
     * @param $opt
     */
    static public function params_s($opt)
    {
        //判断传入了s参数但是值，则提示错误
        if ((isset($opt["s"]) && !$opt["s"]) || (isset($opt["s"]) && !in_array($opt["s"], array("start", "stop")))) {
            self::log_write("Please run: path/to/php main.php -s [start|stop]");
        }

        if (isset($opt["s"]) && in_array($opt["s"], array("start", "stop"))) {
            switch ($opt["s"]) {
                case "start":
                    $config = array(
                        "host" => self::$host == 0?getConfig("host"):self::$host,
                        "port"  => self::$port == 0 ? getConfig("port"):self::$port,
                    );
                    new HttpServer($config);
                    break;
                case "stop":
                    self::stop();
                    break;
            }
        }
    }
    /**
     * 停止服务
     * @param bool $output
     */
    static public function stop($output = true)
    {
        $pid = @file_get_contents(HttpServer::$pid_file);
        if ($pid) {
            if (swoole_process::kill($pid, 0)) {
                swoole_process::kill($pid, SIGTERM);
                @unlink(HttpServer::$pid_file);
                Main::log_write("进程" . $pid . "已结束");
            } else {
                @unlink(HttpServer::$pid_file);
                Main::log_write("进程" . $pid . "不存在,删除pid文件");
            }
        } else {
            $output && Main::log_write("需要停止的进程未启动");
        }
    }
    /**
     * 记录日志
     * @param $message
     */
    static public function log_write($message)
    {
        $now = date("H:i:s");
        if (HttpServer::$daemon) {
            $destination = self::$log_path . "log_" . date("Y-m-d") . ".log";
            error_log("{$now} : {$message}\r\n", 3, $destination, '');
        }
        echo "{$now} : {$message}\r\n";
    }
}

//运行
Main::run();
