
cc.Class({
    extends: cc.Component,

    properties: {
       result:cc.Label,
       score: cc.Label
    },


    init:function(game){
        this.node.active = false;
        this.game = game;
    },

    start () {

    },

    show(score,isWin){
        if (isWin){
            this.result.string = "YOU WIN !"
        }else {
            this.result.string = "YOU LOSE !";
        }

        this.score.string = score;

        this.node.active = true;
    },

    onBtnClick(event){
        this.game.gameStart();
    }

    // update (dt) {},
});
