
cc.Class({
    extends: cc.Component,

    properties: {
      curLevel:cc.Label,
      nextLevel:cc.Label,
      ani:cc.Animation,
      effect:{
          type:cc.AudioClip,
          default:null
      }
    },

    setLevel(curLevel,nextLevel){
        this.curLevel.string = curLevel
        this.nextLevel.string = nextLevel
    },

    onLoad () {
        // 播放
        this.ani.play('passLevel');
       
    },

    aniShowEnd:function(){
        // 关卡播放结束 ，进入下一关

        var act = cc.delayTime(0.5);
        var call = cc.callFunc(function(){
            Game.nextLevel();
            this.node.removeFromParent();
        }.bind(this))
        
        this.node.runAction(cc.sequence(act,call))
    },

    playEffect:function(){
        cc.audioEngine.play(this.effect,false,1)
    },


    start () {

    },

    // update (dt) {},
});
