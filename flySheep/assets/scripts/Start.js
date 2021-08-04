

cc.Class({
    extends: cc.Component,

    properties: {
       
    },

    // LIFE-CYCLE CALLBACKS:

    // onLoad () {},

    start () {

    },

    onStartClick:function(){
        this.node.active = false;
        Global.Game.gameStart();
    }

    // update (dt) {},
});
