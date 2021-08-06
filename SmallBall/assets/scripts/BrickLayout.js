
cc.Class({
    extends: cc.Component,

    properties: {
        // 边距
       padding:0,

       // 砖块之间的间隔
       spacing:0,

       // 总共有几行
       column:0,

       // 总数量
       total:0,

       // 砖块的预制资源
       brickPrefab: cc.Prefab
    },

    
    init:function(){
        this.node.removeAllChildren();
        for (var i =0; i < this.total; ++i){
            var node = cc.instantiate(this.brickPrefab);
            node.parent = this.node;

            node.x = this.padding + (i%this.column)*(node.width+this.spacing);
            node.y = -this.padding - Math.floor(i/this.column)*(node.height + this.spacing);
        }
    },
    // onLoad () {},

    start () {

    },

    // update (dt) {},
});
