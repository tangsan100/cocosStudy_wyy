
cc.Class({
    extends: cc.Component,

    properties: {
        blackAni:cc.Animation
    },

    // LIFE-CYCLE CALLBACKS:

    onLoad () {
        cc.log("onLoad");
        // cc.log(this.blackAni)
        // this.blackAni.play("lrAni")
    },

    start () {

    },

    // update (dt) {},
});
