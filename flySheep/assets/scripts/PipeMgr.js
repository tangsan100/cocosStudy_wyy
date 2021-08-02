var PipeGroup = require("PipeGroup")

cc.Class({
    extends: cc.Component,

    properties: {
        //管道的预制体
        pipePrefab:cc.Prefab,
        // 产生频率
        spawnInterval:0
    },

    onLoad () {
        window.Global.PipeMgr = this;
    },

    // 创建管道
    startSpawnPipe:function(){
        this.spawnPipe();
        this.schedule(this.spawnPipe,this.spawnInterval)
    },

    spawnPipe:function(){
        var obj = Global.SceneManager.spawnObj(this.pipePrefab,PipeGroup,this.node);
    },

    start () {

    },

    // update (dt) {},
});
