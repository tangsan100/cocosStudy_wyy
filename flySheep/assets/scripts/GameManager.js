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
        cc.director.getCollisionManager().enabled = true;
        
        this.sheep.runStart(); 
        // 创建管道
        Global.PipeMgr.startSpawnPipe();
    },

    gameOver:function () {
        // 
        this.state = state.Over;
        Global.PipeMgr.stopSpawnPipe();
    },

    isGameOver:function(){
        return this.state == state.Over;
    }

    // update (dt) {},
});
