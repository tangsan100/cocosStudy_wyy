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
        
        this.sheep.runStart(); 
        // 创建管道
        Global.PipeMgr.startSpawnPipe();
    },

    gameOver:function () {
        // 
        this.state = state.Over;
        Global.PipeMgr.stopSpawnPipe();
        cc.audioEngine.play(this.audioOver, false, 1);
    },

    isGameOver:function(){
        return this.state == state.Over;
    }

    // update (dt) {},
});
