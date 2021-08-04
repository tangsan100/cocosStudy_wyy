var state = cc.Enum({
    Menu:-1,
    Start:-1,
    Over:-1
});

var Sheep = require ("Sheep");

var Game = cc.Class({
    extends: cc.Component,

    properties: {
       sheep:Sheep,
       audioOver: {
            type: cc.AudioClip,
            default: null
        },
        audioBgm: {
            type: cc.AudioClip,
            default: null
        },
        overPrefab:cc.Prefab,

    },
    statics:{
        state
    },

    onLoad () {
        window.Global.GameManager = Game;
        window.Global.Game = this;
        this.state = state.Menu;
        

        // 小样初始化
        this.sheep.init();

        
        
    },

    start () {

        // 开启碰撞检测
        var manager = cc.director.getCollisionManager();
        manager.enabled = true;
        // manager.enabledDebugDraw = true;
        // manager.enabledDrawBoundingBox = true;
        
       
    },

    gameStart:function(){

        this.state = state.Start;

        this.sheep.runStart(); 
        // 创建管道
        Global.PipeMgr.startSpawnPipe();

        this.bgmId =  cc.audioEngine.play(this.audioBgm, true, 1);
    },

    gameOver:function () {
        // 
        this.state = state.Over;
        Global.PipeMgr.stopSpawnPipe();
        cc.audioEngine.play(this.audioOver, false, 1);

        var gameOverUI = cc.instantiate(this.overPrefab);
        if (gameOverUI){
            var parent = cc.find("Canvas"); 
            gameOverUI.parent = parent;
        }

        cc.audioEngine.pause(this.bgmId);
       
    },

    isGameOver:function(){
        return this.state == state.Over;
    },

    isGameStart:function(){
        return this.state == state.Start;
    },

    restart(){
        // 重置管道
        Global.PipeMgr.resetPipe();

        //重置分数
        userScore.score = 0;

        // 游戏开始
        this.state = state.Start;

        // 羊开始跑
        this.sheep.resetSheep();
        this.sheep.runStart();

        // 重新开始新建管道
        Global.PipeMgr.startSpawnPipe();

        cc.audioEngine.resume(this.bgmId)
    },

    // update (dt) {},
});
