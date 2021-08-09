var Level = require("Level")


cc.Class({
    extends: cc.Component,

    properties: {
        // 当前关卡
       currentLevel:0,

       // 总关卡
       totalLevel:0,

       // 关卡
       LevelMgr:Level
    },


    onLoad () {

        window.Game = this;

        // 开启物理引擎
        cc.director.getPhysicsManager().enabled = true;

        this.LevelMgr.setLevel(this.currentLevel);
    },
    
    nextLevel:function(){
        if (this.currentLevel < this.totalLevel){
            this.currentLevel ++;
            this.LevelMgr.setLevel(this.currentLevel);
        }
    },

    start () {

    },

    // update (dt) {},
});
