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

var Dust = require ("Dust");

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
                this.updateSheepState();
           },
       },

       // 地面的高度
       groundY: 0,

       // 重量加速度
       gravity:0,

       // 起跳速度
       jumpSpeed:0,

       // 尘土预制体
       dustPrefab:cc.Prefab,

       // 起跳音效
       audioJump: {
            type: cc.AudioClip,
            default: null
        },

        // 得分音效
       audioScore: {
            type: cc.AudioClip,
            default: null
        },

        // 死亡音效
       audioDead: {
            type: cc.AudioClip,
            default: null
        },
    },

    onLoad () {
        
        this.touchEnabled(true);
    },

    resetSheep(){
        // this.node.y = 0;
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

        this.spawnDust('DustUp');
        cc.audioEngine.play(this.audioJump, false, 1);
    },

    spawnDust(aniName){
        var obj = Global.SceneManager.spawnObj(this.dustPrefab,Dust,this.node);
        obj.playAni(aniName);
    },

    init:function(){
        this.ani = this.node.getComponent(cc.Animation);
        this.currentSpeed = 0;
    },

    // 状态变更后改变动画
    updateSheepState:function(){
        var aniName = State[this._state];
        this.ani.stop();
        this.ani.play(aniName);

        if (this.state == State.Dead) {
            this.touchEnabled(false);
            cc.audioEngine.play(this.audioDead, false, 1);
        }

    },

    // 下落结束后的帧事件
    onDropFinished:function () {
        if (this.state !== State.Dead){
            this.state = State.Run;
        }
        
    },

    // 游戏开始
    runStart:function(){
        this.state = State.Run;
    },
    start () {

    },

     // 碰撞检测
     onSheepCollision:function (other, self){
        
        if (this.state !== State.Dead){
            var group = other.node.group;
            if (group == "obstacle"){
                // 碰撞，死亡
                this.state = State.Dead;
                
                Global.Game.gameOver();
               
            }else if (group == "level"){
                // 得分
                cc.audioEngine.play(this.audioScore, false, 1);
                userObj.addScore();
            }
        }
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
                    this.spawnDust('DustDown');
                }
                break;
            
        }
        
        if (this.state == State.Jump || this.state == State.Drop || this.node.y > this.groundY){
            // 减速
            this.currentSpeed -= dt*this.gravity;
            // 更新高度
            this.node.y += this.currentSpeed*dt;
        }
        
        
    },
});
