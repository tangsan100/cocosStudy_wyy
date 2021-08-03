
cc.Class({
    extends: cc.Component,

    properties: {
        // 每根柱子的初始偏移量
        offsetX:0,

        // 
        spaceOffset:0,

        // 移动速度
        moveSpeed:0,

        // 底部位置区间
        bottomVec:cc.v2(0,0),

        // 中间空白区域的区间
        spacingVec:cc.v2(0,0),

        // 顶部柱子
        topPipe:cc.Node,

        // 底部管道
        bottomPipe:cc.Node,

    },

    onEnable(){
        this.initPos();
    },

    initPos:function(){
        this.node.position = cc.v2(0,0);
        // 下管道的位置
        var posX = this.offsetX;
        var posBottomY = this.bottomVec.x + Math.random()*(this.bottomVec.y - this.bottomVec.x)
        this.bottomPipe.position = cc.v2(posX,posBottomY);

        // 上管道的位置
        var posTopY = posBottomY + this.spaceOffset+ Math.random()*(this.spacingVec.y - this.spacingVec.x);
        this.topPipe.position = cc.v2(posX,posTopY)
    },


    // onLoad () {},

    removePipe:function(){
        Global.SceneManager.despawnObj(this);
    },

    update (dt) {

        if (Global.Game.isGameOver()){
            return;
        }

        var dis = this.moveSpeed*dt;
        this.node.x += dis;

        // 转换成世界坐标系
        var rect =  this.node.getBoundingBoxToWorld();
        // 超出左边的屏幕，世界坐标，左下角为坐标原点
        if (rect.xMax < 0){
            this.removePipe();
        }
    },
});
