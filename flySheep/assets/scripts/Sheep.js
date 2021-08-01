var State = cc.Enum(
    {
        Node:-1,
        Run:-1,
        Jump:-1,
        Drop:-1,
        DropEnd:-1,
        Dead:-1,
        Run_invincible:-1
    }
)

cc.Class({
    extends: cc.Component,

    properties: {
       _state:{
           default:State.Node,
           type:State
       },
       state:{
           get:function(){
               return this._state;
           },

           set:function(val){
                if (val === this.state){
                    return ;
                }
                this._state = val;
                this.updateAnimation();
           },
       },

       // 地面的高度
       groundY: 0,

       // 重量加速度
       gravity:0,

       // 起跳速度
       jumpSpeed:0
    },

    onLoad () {
        
        this.touchEnabled(true);
    },

    touchEnabled:function(flag){
        if (flag){
            // 注册输入事件
            this.registerInput();
        }else {
            // 取消输入事件
            this.cancleInput();
        }
    },

    registerInput:function(){
        // 鼠标按下事件和触摸事件的注册
        cc.systemEvent.on(cc.SystemEvent.EventType.KEY_DOWN, this.jumpAction, this);

        // 触摸点击
        cc.find('Canvas').on(cc.Node.EventType.TOUCH_START,this.jumpAction,this);
    },

    cancleInput:function(){
        cc.systemEvent.off(cc.SystemEvent.EventType.KEY_DOWN, this.jumpAction,this)
        cc.find('Canvas').off(cc.Node.EventType.TOUCH_START,this.jumpAction,this);
    },

    jumpAction:function(){
        this.state = State.Jump;
        this.currentSpeed = this.jumpSpeed;
    },

    init:function(){
        this.ani = this.node.getComponent(cc.Animation);
        this.currentSpeed = 0;
    },

    // 状态变更后改变动画
    updateAnimation:function(){
        var aniName = State[this._state];
        this.ani.stop();
        this.ani.play(aniName);
    },

    // 游戏开始
    runStart:function(){
        this.state = State.Run;
    },
    start () {

    },

    update (dt) {

        switch (this.state) {
            case State.Jump:
                if (this.currentSpeed < 0) {
                    this.state = State.Drop;
                }
                break;
            case State.Drop:
                if (this.node.y <= this.groundY){
                    this.state = State.DropEnd;
                }
                break;
            
        }
        
        if (this.state == State.Jump || this.state == State.Drop){
            // 减速
            this.currentSpeed -= dt*this.gravity;
            // 更新高度
            this.node.y += this.currentSpeed*dt;
        }
        
        
    },
});
