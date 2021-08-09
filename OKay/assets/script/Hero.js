
cc.Class({
    extends: cc.Component,

    properties: {
       speed:0
    },

    onLoad () {

        
        // 获取刚体组件
        this.rigidBody = this.node.getComponent(cc.RigidBody);

        // 触屏事件监听
        this.node.parent.on(cc.Node.EventType.TOUCH_START,this.onTouchStart,this)
        this.node.parent.on(cc.Node.EventType.TOUCH_MOVE,this.onTouchMove,this)
        this.node.parent.on(cc.Node.EventType.TOUCH_END,this.onTouchEnd,this)
        this.node.parent.on(cc.Node.EventType.TOUCH_CANCEL,this.onTouchCancel,this)

    },

    // 点击开始
    onTouchStart:function(Event){
        // TODO 1. 触点地方显示小球
        this.isMove = false
        cc.log('touch')
        var pos = Event.getLocation();
        this.startPos = this.node.parent.convertToNodeSpaceAR(pos);
        this.node.position = this.startPos;
        this.rigidBody.linearVelocity = cc.v2(0,0)
        this.node.opacity = 255;
        // this.node.active = true;
    },

    // 移动
    onTouchMove:function(Event){

    },

    // 取消
    onTouchCancel:function(Event){

    },

    // 结束
    onTouchEnd:function(Event){
        var pos = Event.getLocation();
        var endPos = this.node.parent.convertToNodeSpaceAR(pos);

        var disX = endPos.x - this.startPos.x;
        var disY = endPos.y - this.startPos.y;

        var dis = Math.sqrt(disX*disX + disY*disY);

        // 距离不够，不做移动处理
        if (dis < 50) {
            return;
        }

        this.isMove = true;

        this.rigidBody.linearVelocity = cc.v2(disX/dis*this.speed,disY/dis*this.speed);

    },

    // 碰撞监听
    // 只在两个碰撞体开始接触时被调用一次
    onBeginContact: function (contact, selfCollider, otherCollider) {

        if (!this.isMove) return;

        cc.log(otherCollider.tag)
        var tag = otherCollider.tag
        if (tag != 100){
            // 不是黑块
            Level.onCollision(tag,otherCollider.node);
        }else if (tag == 100){
            // 黑块
            Level.onCollisionBlack(tag,otherCollider.node);
        }
    },

    start () {

    },

    update (dt) {
        if (this.node.opacity == 0) return;

        var rect = this.node.getBoundingBoxToWorld();
        var size = cc.view.getVisibleSize()
        if (rect.xMax > size.width || rect.yMax > size.height || rect.xMax < 0 || rect.yMax < 0){
            // 移出了屏幕,判断是否过关
            Level.processEnd();
            this.node.opacity = 0
        }
    },
});
