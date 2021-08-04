var PipeGroup = require("PipeGroup")

cc.Class({
    extends: cc.Component,

    properties: {
        //管道的预制体
        pipePrefab:cc.Prefab,
        // 产生频率
        spawnInterval:{
            default:0.00,
            type: cc.Float
        }
    },

    onLoad () {
        window.Global.PipeMgr = this;
    },

    // 创建管道
    startSpawnPipe:function(){
        this.spawnPipe();
        this.schedule(this.spawnPipe,this.spawnInterval)
    },

    // 停止创建
    stopSpawnPipe:function(){
        this.unschedule(this.spawnPipe,this);
    },

    spawnPipe:function(){
        var obj = Global.SceneManager.spawnObj(this.pipePrefab,PipeGroup,this.node);
    },

    // 管道重置
    resetPipe(){

        var children = this.node.children;
        
        var arr = [];

        for (var i = 0; i < children.length; ++i){
           
            var node = children[i];
            var comp = node.getComponent('PipeGroup');
            arr.push(comp);
        }

        arr.forEach(comp => {
            comp.removePipe();
        });
       
    }

    // update (dt) {},
});
