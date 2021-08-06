
cc.Class({
    extends: cc.Component,

    properties: {
       ani:cc.Animation
    },

    onLoad () {
        // var ani = this.node.getComponent(cc.Animation);
        this.ani.play("scale")
    },

    init:function(game){
        this.game = game;
        this.node.on(cc.Node.EventType.MOUSE_DOWN,this.touchStart,this)
        this.node.on(cc.Node.EventType.TOUCH_START,this.touchStart,this)
    },

    touchStart:function(event){
        this.node.active = false;
        this.game.gameStart();
    },

    start () {

    },

    // update (dt) {},
});
