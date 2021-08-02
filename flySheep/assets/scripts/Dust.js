

cc.Class({
    extends: cc.Component,

    properties: {
       anim:cc.Animation
    },

    

    onLoad () {

    },

    playAni:function(aniName){
        // 播放动画
        this.anim.play(aniName);
    },

    finish:function() {
      // 移除对象
    //   this.node.removeFromParent();  
      Global.SceneManager.despawnObj(this);
    },

    start () {

    },
});
