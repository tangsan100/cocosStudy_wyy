var Level = require("Level")
var Hero = require("Hero")
var PointNode = require("PointNode")


cc.Class({
    extends: cc.Component,

    properties: {
        // 当前关卡
       currentLevel:0,

       // 总关卡
       totalLevel:0,

       // 关卡
       LevelMgr:Level,

       // 影子点 节点管理器
       pointNode:PointNode,

       // 小球
       hero:Hero,

       // 热更新
       hotUpdatePanel:cc.Prefab,

       // 提示框
       resetNode:cc.Node,
    },


    onLoad () {

        window.Game = this;

        this.resetNode.active = false;

        // 开启物理引擎
        cc.director.getPhysicsManager().enabled = true;

        this.LevelMgr.setLevel(this.currentLevel);

        this.hero.init(this.pointNode)
        this.LevelMgr.init(this.pointNode)

        var hot = cc.instantiate(this.hotUpdatePanel);

        if (hot){
            hot.parent = this.node;
        }

        
    },
    
    nextLevel:function(){
        if (this.currentLevel < this.totalLevel){
            this.currentLevel ++;
            this.LevelMgr.setLevel(this.currentLevel);
        }else {
            this.resetNode.active = true;
        }
    },

    onBtnResetClick:function(event){
        cc.log("reset click !!!")
        this.resetNode.active = false;
        this.currentLevel = 1;
        this.LevelMgr.setLevel(this.currentLevel);
    },

    start () {

    },

    // update (dt) {},
});
