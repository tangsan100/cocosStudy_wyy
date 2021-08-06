
cc.Class({
    extends: cc.Component,

    properties: {
        speed:cc.v2(0,0),
        hitEffect:{
            default:null,
            type:cc.AudioClip
        }
    },

     onLoad () {
        this.rigidbody = this.node.getComponent(cc.RigidBody);
        this.rigidbody.enabledContactListener = true;

        this.orignPos = this.node.position;
     },

     init:function(game){
        this.game = game;
        this.node.position = this.orignPos;
        this.rigidbody.linearVelocity = this.speed;
     },

    // 碰撞回调
    onBeginContact: function (contact, selfCollider, otherCollider) {

        if (!this.game || !this.game.isGameStart())
            return;

        var tag = otherCollider.tag;
        switch (tag){
            case 1: 
                //墙壁
                cc.audioEngine.play(this.hitEffect,false,1);
                break;
            case 2:
                // 地板
                cc.audioEngine.play(this.hitEffect,false,1);
                this.game.onGroundCollision(otherCollider);
                break;
            case 3:
                // 挡板
                cc.audioEngine.play(this.hitEffect,false,1);
                break;
            case 4:
                // 砖块
                this.game.onBrickCollision(otherCollider);
                break;
        }
    },
   

    start () {

    },

    // update (dt) {},
});
