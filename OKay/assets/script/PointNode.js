
cc.Class({
    extends: cc.Component,

    properties: {
       pointPrefab:cc.Prefab,
       bgPrefab:cc.Prefab,
       aimPointNum:0
    },

    
    onLoad () {
        // 创建一个节点池，
        this.pointPool = new cc.NodePool();

        // 背景块节电池
        this.bgNodePool = new cc.NodePool();

        for (var i = 0; i < 10; i ++){
            var node = cc.instantiate(this.bgPrefab);
            this.bgNodePool.put(node);
        }

        // 事先创建出来黑点
        for (var i = 0; i < 500; i++){
            var point = cc.instantiate(this.pointPrefab);

            this.pointPool.put(point);
        }

        this.aimPoints = Array();
        for (var i = 0; i < this.aimPointNum; ++i){
            var node = this.createPoint(this.node);
            node.active = false;
            node.scale = 0.2
            this.aimPoints.push(node);
        }
    },

    // 创建一个点
    createPoint: function (parentNode) {
        let point = null;
        if (this.pointPool.size() > 0) { // 通过 size 接口判断对象池中是否有空闲的对象
            point = this.pointPool.get();
        } else { // 如果没有空闲对象，也就是对象池中备用对象不够时，我们就用 cc.instantiate 重新创建
            point = cc.instantiate(this.pointPrefab);
        }
        point.parent = parentNode; // 将生成的敌人加入节点树
        return point;

    },

    onDestroyPiont(point){
        this.pointPool.put(point);
    },


    // 创建瞄准序列点
    createAimPoints(startPos,endPos){
        var offsetX = (endPos.x - startPos.x)/(this.aimPointNum+1);
        var offsetY = (endPos.y - startPos.y)/(this.aimPointNum+1);
        
        
        for (var i =0; i <  this.aimPoints.length; ++i){
            var node = this.aimPoints[i];
            node.position = cc.v2(startPos.x + offsetX*(i+1),startPos.y + offsetY*(i+1));
            node.active = true;
        }
    },

    resetAimPoints:function(){
        this.aimPoints.forEach(node=>{
            node.active = false;
        })
    },

    createTailPoint:function(pos){
        var node = this.createPoint(this.node);
        node.opacity = 255;
        node.active = true;
        node.parent = this.node;
        node.scale = 0.2;
        node.isTail = true;
        node.position = pos;

        var fade = cc.fadeOut(0.3);
        var call = cc.callFunc(function(){
            this.onDestroyPiont(node);
        }.bind(this));

        node.runAction(cc.sequence(fade,call));
    },

    createBgNode:function(parent){
        let node = null;
        if (this.bgNodePool.size() > 0) {
            node = this.pointPool.get();
        } else { 
            node = cc.instantiate(this.bgPrefab);
        }
        node.parent = parent; 
        return node;
    },

    onDestroyBgNode(node){
        this.bgNodePool.put(node)
    }
   

    // update (dt) {},
});
