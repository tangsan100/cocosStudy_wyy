
cc.Class({
    extends: cc.Component,

    properties: {
        scoreBg:cc.Label,
        score:cc.Label,
        recordBg:cc.Label,
        record:cc.Label
    },

    // LIFE-CYCLE CALLBACKS:

    // onLoad () {},

    start () {

    },

    update (dt) {
        this.scoreBg.string = ""+userScore.score;
        this.score.string = ""+userScore.score;
        this.recordBg.string = ""+ userScore.highest;
        this.record.string = ""+ userScore.highest;
    },
});
