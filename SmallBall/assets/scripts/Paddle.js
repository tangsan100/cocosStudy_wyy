
cc.Class({
    extends: cc.Component,

    properties: {
      
    },

   
    onLoad () {

        // 注册触屏事件
        this.node.parent.on(cc.Node.EventType.TOUCH_MOVE,this.onTouchMove,this)

        this.orignPos = this.node.position;

    },

    init:function(){
        this.node.position = this.orignPos;
    },

    reset:function(){

    },

    onTouchMove:function(event){
        // 获取位置
        var pos = event.getDelta();
        // cc.log(pos)

        this.node.x += pos.x;

    },

    start () {

    },

    // update (dt) {},
});
