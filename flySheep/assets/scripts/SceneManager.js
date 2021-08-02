

cc.Class({
    extends: cc.Component,

    properties: {
       _pool:cc.js.Pool
    },

    // LIFE-CYCLE CALLBACKS:

    onLoad () {

        Global.SceneManager = this;

        // 创建节点对象池
        this._pool = new cc.js.Pool(function(obj){
            cc.log("obj destroy");
        },10)

        // get 方法为自定义方法
        this._pool.get = this.getComObj;
    },

    /*
    * 取出节点对象
    * comp 对象类型
    */
    getComObj:function(comp) {
        // 变量池子，找出对应的类型的节点对象
        var comArr = [];
        for (var i = 0; i < this.count; ++i){
            var obj = this._get();
            if (obj instanceof comp){

                // 归还节点
                comArr.forEach(function(o) {
                    this.put(o);
                }.bind(this))
                // 返回节点
                return obj;
            }

            comArr.push(obj);
        }

        return null;
    },


    /*
    * 创建节点对象
    */
   spawnObj:function(prefab,comType,parent) {
        var obj = this._pool.get(comType);
        if (obj == null){
            obj = cc.instantiate(prefab);
            obj = obj.getComponent(comType);
        }

        if (parent){
            obj.node.parent = parent;
        }else {
            obj.node.parent = this.node;
        }

        obj.node.active = true;

        return obj;
   },

   /*
   * 回收节点
   */
   despawnObj(comp){
        comp.node.removeFromParent(false);
        comp.node.active = false;
        this.putIntoPool(comp);
   },

   /*
   * 回收节点对象
   */
   putIntoPool:function(comp) {
       var oldCount = this._pool.count;
       this._pool.put(comp);

       return this._pool.count > oldCount;
   },


    start () {

    },

    // update (dt) {},
});
