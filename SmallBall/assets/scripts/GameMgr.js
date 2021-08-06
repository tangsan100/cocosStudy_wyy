var BrickLayout = require("BrickLayout")
var UserScore = require("UserScore");
var OverPanel = require("OverPanel");
var Paddle = require("Paddle");
var Loading = require("Loading")


var State = cc.Enum({
    Menu:-1,
    Start:-1,
    Over:-1
})

cc.Class({
    extends: cc.Component,

    properties: {
        // 砖块容器
        bricks:BrickLayout,
        // 小球
        ball:cc.Node,

        // 分数
        score:UserScore,

        // 游戏结束面板
        overPanel:OverPanel,

        // 挡板
        paddle:Paddle,

        // 加载页面
        loading:Loading,

        // 背景音乐
        bgm:{
            type:cc.AudioClip,
            default:null
        },

        overEffect:{
            type:cc.AudioClip,
            default:null
        },

        socreEffect:{
            type:cc.AudioClip,
            default:null
        }
    },

    

    onLoad () {

        this.state = State.Menu;
       
        // 物理系统句柄
       this.physicsMgr =  cc.director.getPhysicsManager();
       this.physicsMgr.enabled = true;

       this.ball = this.ball.getComponent("Ball")

       this.loading.init(this);

       

    },

    start () {
        // 开启物理引擎
        // cc.director.getPhysicsManager().enabled = true;
        // this.gameStart();
    },

    // 游戏开始
    gameStart(){

        this.state = State.Start;

        // 开启物理引擎
        this.physicsMgr.enabled = true;

        // 绘制砖块
        this.bricks.init();

        // 小球初始化
        this.ball.init(this);

        // 挡板初始化
        this.paddle.init();

        // 结束面板初始化
        this.overPanel.init(this);

        this.score.init();

        this.bgmId = cc.audioEngine.play(this.bgm,true,1);
    },

    onBrickCollision:function(otherCollider){
        var node = otherCollider.node;
        node.removeFromParent();

        this.score.addScore(1);

        cc.audioEngine.play(this.socreEffect,false,1);

        if (this.score.score >= this.bricks.total){
            this.overPanel.show(this.score.score,true);
            this.gameOver()
        }

    },

    // 撞击地板，游戏结束
    onGroundCollision(otherCollider){
        this.overPanel.show(this.score.score,false);
        this.gameOver()
    },

    gameOver:function(){
        this.state = State.Over;
        this.physicsMgr.enabled = false;
        cc.audioEngine.pause(this.bgmId);
        cc.audioEngine.play(this.overEffect,false,1);
        // this.bricks.reset();
        // this.ball.reset();
        // this.paddle.reset();
    },

    isGameStart:function(){
        return this.state == State.Start;
    }

   
});
