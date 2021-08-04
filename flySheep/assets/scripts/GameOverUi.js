

cc.Class({
    extends: cc.Component,

    properties: {
      score:cc.Label,
      scoreBg:cc.Label
    },

    // LIFE-CYCLE CALLBACKS:

    onLoad () {
        // 播放动画
        var ani = this.node.getComponent(cc.Animation);
        ani.play("Rotate");

        this.showScore();

    },

    showScore:function(){
        this.score.string = ""+userScore.score;
        this.scoreBg.string = ""+userScore.score;
    },


    onBtnClick:function(eventType){
        // 关闭窗口
        this.node.removeFromParent();
        Global.Game.restart();
    }

    // update (dt) {},
});
